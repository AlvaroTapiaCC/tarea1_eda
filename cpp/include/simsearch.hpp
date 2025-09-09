#ifndef _SIM_SEARCH_HPP
#define _SIM_SEARCH_HPP

#include "matrix.hpp"
#include "cluster.hpp"
#include <vector>

using namespace std;

struct queryResult {
    long long comparisons;
    long long time_no_sort;
    long long time_total;
    vector<const float*> neighbors;
    vector<size_t> neighbors_idxs;
    float avg_distance;
};

struct experimentResult {
    size_t m;
    size_t k;
    long long total_comparisons;
    long long time_no_sort;
    long long time_total;
    vector<queryResult> queries;
    float avg_distance;
    float error;

};

class SimSearch{
private:
    const Matrix &mat_data;         // data to be processed
    const Matrix &mat_queries;      // queries  
    const Matrix &mat_clusters;     // centroids to be used
    Cluster &clus;

public:
    SimSearch(const Matrix &data, const Matrix &queries, const Matrix &centroids, Cluster &clusters) : mat_data(data), mat_queries(queries), mat_clusters(centroids), clus(clusters) {};
    queryResult directSearch (const float* query, size_t m);
    queryResult searchWithClusters (const float*, size_t m);
    experimentResult runExperiment (size_t m, size_t k);
};

#endif 