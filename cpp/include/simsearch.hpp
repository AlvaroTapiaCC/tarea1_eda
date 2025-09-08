#ifndef _SIM_SEARCH_HPP
#define _SIM_SEARCH_HPP

#include "matrix.hpp"
#include <vector>

using namespace std;

struct queryResult {
    long long comparisons;
    long long time_no_sort;
    long long time_total;
    vector<const float*> neighbors;
};

struct experimentResult {
    int m;
    int k;
    long long total_comparisons;
    long long time_no_sort;
    long long time_total;
    vector<queryResult> queries;
};

class SimSearch{
private:
    const Matrix &mat_data;         // data to be processed
    const Matrix &mat_queries;      // queries  
    const Matrix &mat_clusters;     // centroids to be used

public:
    SimSearch(const Matrix &data, const Matrix &queries, const Matrix &clusters) : mat_data(data), mat_queries(queries), mat_clusters(clusters) {};
    queryResult directSearch (const float* query, int m);
    queryResult searchWithClusters (const float*, int m, int k);
    experimentResult runExperiment (int m, int k);
};

#endif 