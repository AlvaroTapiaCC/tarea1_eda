#include <iostream>
#include <chrono>
#include <vector>
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

    auto start_srch = high_resolution_clock::now();

    for (size_t i = 0; i < N; i++) {
        const float* vector_i = mat_data.getRow(i);
        float dist = vec_compute_distance(query, vector_i, dim);
        distances[i] = dist;
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

    for (int i = 0; i < m; i++) {
        result.neighbors.push_back(mat_data.getRow(indexes[i]));
    }
    delete[] distances;
    delete[] indexes;

    return result;
}

//funcion que busca usando clusters
//agrupar en k clusters
//calcular cuantos clusters necesito dependiendo de m
//realizar el ordenamiento y busqueda pero usando solo los vectores de los n clusters necesarios
//calcular tiempos

queryResult SimSearch::searchWithClusters(const float* query, int m, int k) {
    queryResult result;
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

    size_t num_queries = mat_queries.getN();

    for (size_t q = 0; q < num_queries; q++) {
        const float* query = mat_queries.getRow(q);
        queryResult qr;

        if (k == 0) {
            qr = directSearch(query, m);
        } else {
            qr = searchWithClusters(query, m, k);
        }
        result.total_comparisons += qr.comparisons;
        result.time_no_sort += qr.time_no_sort;
        result.time_total += qr.time_total;
        result.queries.push_back(qr);
    }
    return result;
}
   
