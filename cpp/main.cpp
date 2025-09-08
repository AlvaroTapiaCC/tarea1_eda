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
        for (int j = 0; j < 6; j++) {
            int k = k_values[j];
            Matrix clusters(k, data_eda.getDim());

            if (k > 0) {
                Cluster clus(data_eda, k);
                clus.applyClustering();
                for (int c = 0; c < k; c++) {
                    const float* centroid = clus.getCentroid(c);
                    clusters.setRow(c, const_cast<float*>(centroid));
                }
                SimSearch sim(data_eda, queries_eda, clusters, clus);
                experimentResult exp = sim.runExperiment(m, k);
                cout << endl;
                cout << "EXPERIMENT PARAMS: m=" << m << ", k=" << k << endl;
                cout << "search time = " << exp.time_no_sort << " microseconds" << endl;
                cout << "total time = " << exp.time_total << " microseconds" << endl;
                cout << "total comparisons = " << exp.total_comparisons << endl;
                cout << "avg distance = " << exp.avg_distance << endl;
            } else {
                Cluster no_clus(data_eda, k);
                SimSearch sim(data_eda, queries_eda, clusters, no_clus);
                experimentResult exp = sim.runExperiment(m, k);
                cout << endl;
                cout << "EXPERIMENT PARAMS: m=" << m << ", k=" << k << endl;
                cout << "search time = " << exp.time_no_sort << " microseconds" << endl;
                cout << "total time = " << exp.time_total << " microseconds" << endl;
                cout << "total comparisons = " << exp.total_comparisons << endl;
                cout << "avg distance = " << exp.avg_distance << endl;
            }
        }
    }
    return 0;
} 