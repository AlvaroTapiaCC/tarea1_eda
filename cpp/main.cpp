#include <iostream>
#include "matrix.hpp"
#include "cluster.hpp"
#include "simsearch.hpp"

using namespace std;

int main() {
    Matrix data_eda("../../data_eda.npy");
    Matrix queries_eda("../../queries_eda.npy");

    int m_values[] = {8, 16, 32, 64, 128};
    int k_values[] = {0, 8, 16, 32, 64, 128};

    for (int i = 0; i < 5; i++) {
        int m = m_values[i];
        for (int j = 0; j < 1; j++) {
            int k = k_values[j];

            if (k > 0) {
                Cluster clus(data_eda, k);
                clus.applyClustering();
                Matrix clusters(k, data_eda.getDim());
                for (int c = 0; c < k; c++) {
                    const float* centroid = clus.getCentroid(c);
                    clusters.setRow(c, const_cast<float*>(centroid));
                }
                SimSearch sim(data_eda, queries_eda, clusters);
                experimentResult exp = sim.runExperiment(m, k);
                cout << endl;
                cout << "EXPERIMENT PARAMS: m=" << m << ", k=" << k << endl;
                cout << "search time = " << exp.time_no_sort << " microseconds" << endl;
                cout << "total time = " << exp.time_total << " microseconds" << endl;
                cout << "total comparisons = " << exp.total_comparisons << endl;
            } else {
                Matrix no_clusters(0, data_eda.getDim());
                SimSearch sim(data_eda, queries_eda, no_clusters);
                experimentResult exp = sim.runExperiment(m, k);
                cout << endl;
                cout << "EXPERIMENT PARAMS: m=" << m << ", k=" << k << endl;
                cout << "search time = " << exp.time_no_sort << " microseconds" << endl;
                cout << "total time = " << exp.time_total << " microseconds" << endl;
                cout << "total comparisons = " << exp.total_comparisons << endl;
            }
        }
    }
    return 0;
} 