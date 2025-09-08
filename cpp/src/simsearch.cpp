#include <iostream>
#include <chrono>
#include <vector>
#include <fstream>
#include "matrix.hpp"
#include "utils.hpp"
#include "simsearch.hpp"

using namespace std;
using namespace std::chrono;

queryResult SimSearch::directSearch(const float* query, int m) {
    size_t N = mat_data.getN();
    size_t dim = mat_data.getDim();
    float* distances = new float[N];
    size_t* indexes = new size_t[N];

    queryResult result;
    result.comparisons = 0;
    result.time_no_sort = 0;
    result.time_total = 0;
    result.neighbors.reserve(m);
    result.neighbors_idxs.reserve(m);
    result.avg_distance = 0.0;

    auto start_srch = high_resolution_clock::now();

    for (size_t i = 0; i < N; i++) {
        const float* vector_i = mat_data.getRow(i);
        distances[i] = vec_compute_distance(query, vector_i, dim);
        indexes[i] = i;
        result.comparisons++;
    }

    auto end_srch = high_resolution_clock::now();
    auto srch_time = duration_cast<microseconds>(end_srch - start_srch).count();

    auto start_sort = high_resolution_clock::now();

    mainSort(distances, indexes, N, m);

    auto end_sort = high_resolution_clock::now();
    auto sort_time = duration_cast<microseconds>(end_sort - start_sort).count();

    result.time_no_sort = srch_time;
    result.time_total = srch_time + sort_time;
    float total_dist = 0.0;
    for (int i = 0; i < m; i++) {
        result.neighbors.push_back(mat_data.getRow(indexes[i]));
        result.neighbors_idxs.push_back(indexes[i]);
        total_dist += distances[i];
    }
    result.avg_distance = total_dist / m;
    delete[] distances;
    delete[] indexes;

    return result;
}

queryResult SimSearch::searchWithClusters(const float* query, int m) {
    size_t N = mat_data.getN();
    size_t K = mat_clusters.getN();
    size_t dim = mat_data.getDim();
    float* cluster_distances = new float[K];
    size_t* cluster_indexes = new size_t[K];

    int count = 0;
    int clusters_needed = 0;
    int sizeof_clusters_needed = 0;
    float total_dist = 0.0;
    
    queryResult result;
    result.comparisons = 0;
    result.time_no_sort = 0;
    result.time_total = 0;
    result.neighbors.reserve(m);
    result.neighbors_idxs.reserve(m);
    result.avg_distance = 0.0;

    auto start_srch = high_resolution_clock::now();

    for (size_t c = 0; c < K; c++) {
        const float* centroid = mat_clusters.getRow(c);
        cluster_distances[c] = vec_compute_distance(query, centroid, dim);
        cluster_indexes[c] = c;
        result.comparisons++;
    }
    quickArgsort(cluster_distances, cluster_indexes, K);

    for (size_t c = 0; c < K; c++) {
        vector<size_t> inds = clus.getInds(cluster_indexes[c]);
        count += inds.size();
        clusters_needed = c + 1;
        if (count >= m) {
            break;
        }
    }
    for (int c = 0; c < clusters_needed; c++) {
        vector<size_t> inds = clus.getInds(cluster_indexes[c]);
        sizeof_clusters_needed += inds.size();
    }
    float* distances = new float[sizeof_clusters_needed];
    size_t* indexes = new size_t[sizeof_clusters_needed];
    int idx = 0;

    if (m <= clus.getInds(cluster_indexes[0]).size()) {
        vector<size_t> inds = clus.getInds(cluster_indexes[0]);
        for (size_t i = 0; i < inds.size(); i++) {
            const float* vector_i = mat_data.getRow(inds[i]);
            distances[i] = vec_compute_distance(query, vector_i, dim);
            indexes[i] = inds[i];
            result.comparisons++;
        }
        auto end_srch = high_resolution_clock::now();
        auto srch_time = duration_cast<microseconds>(end_srch-start_srch).count();
        auto start_sort = high_resolution_clock::now();

        mainSort(distances, indexes, inds.size(), m);
        
        auto end_sort = high_resolution_clock::now();
        auto sort_time = duration_cast<microseconds>(end_sort-start_sort).count();

        result.time_no_sort = srch_time;
        result.time_total = srch_time + sort_time;
        
        for (int i = 0; i < m; i++) {
            result.neighbors.push_back(mat_data.getRow(indexes[i]));
            result.neighbors_idxs.push_back(indexes[i]);
            total_dist += distances[i];
        }
    } else {
        int remaining = m;
        for (int c = 0; c < clusters_needed; c++) {
            vector<size_t> inds = clus.getInds(cluster_indexes[c]);
            int take;
            if (remaining < inds.size()) {
                take = remaining;
            } else {
                take = inds.size();
            }
            float* temp_distances = new float[inds.size()];
            size_t* temp_indexes = new size_t[inds.size()];

            for (size_t i = 0; i < inds.size(); i++) {
                const float* vector_i = mat_data.getRow(inds[i]);
                temp_distances[i] = vec_compute_distance(query, vector_i, dim);
                temp_indexes[i] = inds[i];
                result.comparisons++;
            }
            if (take < inds.size()) {
                mainSort(temp_distances, temp_indexes, inds.size(), take);
            }
            for (int i = 0; i < take; i++) {
                distances[idx] = temp_distances[i];
                indexes[idx] = temp_indexes[i];
                idx++;
            }
            remaining -= take;
            delete[] temp_distances;
            delete[] temp_indexes;
        }
        auto end_srch = high_resolution_clock::now();
        auto srch_time = duration_cast<microseconds>(end_srch-start_srch).count();
        auto start_sort = high_resolution_clock::now();

        mainSort(distances, indexes, idx, m);

        auto end_sort = high_resolution_clock::now();
        auto sort_time = duration_cast<microseconds>(end_sort-start_sort).count();

        result.time_no_sort = srch_time;
        result.time_total = srch_time + sort_time;

        for (int i = 0; i < m; i++) {
            result.neighbors.push_back(mat_data.getRow(indexes[i]));
            result.neighbors_idxs.push_back(indexes[i]);
            total_dist += distances[i];
        }
    }

    result.avg_distance = total_dist / m;

    delete [] cluster_distances;
    delete [] cluster_indexes;
    delete [] distances;
    delete [] indexes;

    return result;
}

experimentResult SimSearch::runExperiment(int m, int k) {
    experimentResult result;
    result.m = m;
    result.k = k;
    result.total_comparisons = 0;
    result.time_total = 0.0;
    result.time_no_sort = 0.0;
    result.queries.reserve(mat_queries.getN());
    result.avg_distance = 0.0;

    size_t num_queries = mat_queries.getN();
    float global_dist = 0.0;

    for (size_t q = 0; q < num_queries; q++) {
        const float* query = mat_queries.getRow(q);
        queryResult qr;

        if (k == 0) {
            qr = directSearch(query, m);
        } else {
            qr = searchWithClusters(query, m);
        }

        if (q == 0) {
            ofstream fout("../../metrics/neighbors.csv");
            for (size_t i = 0; i < qr.neighbors_idxs.size(); i++) {
                fout << qr.neighbors_idxs[i] << "\n";
            }
            fout.close();
        }

        result.total_comparisons += qr.comparisons;
        result.time_no_sort += qr.time_no_sort;
        result.time_total += qr.time_total;
        result.queries.push_back(qr);
        global_dist += qr.avg_distance;
    }
    result.avg_distance = global_dist / num_queries;
    return result;
}
   
