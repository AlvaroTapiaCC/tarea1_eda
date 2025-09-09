#include <iostream>
#include <fstream>
#include "matrix.hpp"
#include "cluster.hpp"
#include "simsearch.hpp"

using namespace std;

int main() {
    Matrix data_eda("../../data_eda.npy");
    Matrix queries_eda("../../queries_eda.npy");

    size_t m_values[] = {8, 16, 32, 64, 128};
    size_t k_values[] = {0, 8, 16, 32, 64, 128};
    float base_avg_distance[5] = {0};

    for (size_t i = 0; i < 5; i++) {
        size_t m = m_values[i];
        for (size_t j = 0; j < 6; j++) {
            size_t k = k_values[j];
            Matrix clusters(k, data_eda.getDim());

            if (k > 0) {
                Cluster clus(data_eda, k);
                clus.applyClustering();
                ofstream fout("../../metrics/cluster/clusters_k" + to_string(k) + ".csv");
                for (size_t c = 0; c < k; c++) {
                    const float* centroid = clus.getCentroid(c);
                    clusters.setRow(c, const_cast<float*>(centroid));
                    vector<size_t> inds = clus.getInds(c);
                    fout << "# cluster " << c << " centroid";
                    for (size_t d = 0; d < data_eda.getDim(); ++d) {
                        fout << "," << centroid[d];
                    }
                    fout << "\n";
                    fout << "# index";
                    for (size_t d = 0; d < data_eda.getDim(); ++d) {
                        fout << ",x" << d;
                    }
                    fout << "\n";
                    for (size_t i = 0; i < inds.size(); ++i) {
                        fout << inds[i];
                        const float* pt = data_eda.getRow(inds[i]);
                        for (size_t d = 0; d < data_eda.getDim(); ++d) {
                            fout << "," << pt[d];
                        }
                        fout << "\n";
                    }
                }
                fout.close();
                SimSearch sim(data_eda, queries_eda, clusters, clus);
                experimentResult exp = sim.runExperiment(m, k);
                
                if (base_avg_distance[i] > 0) {
                    exp.error = ((exp.avg_distance - base_avg_distance[i]) / base_avg_distance[i]) * 100.0f;
                } else {
                    exp.error = 0.0f;
                }
                cout << endl;
                cout << "EXPERIMENT PARAMS: m=" << m << ", k=" << k << endl;
                cout << "search time = " << exp.time_no_sort << " microseconds" << endl;
                cout << "total time = " << exp.time_total << " microseconds" << endl;
                cout << "total comparisons = " << exp.total_comparisons << endl;
                cout << "avg distance = " << exp.avg_distance << endl;
                cout << "error = " << exp.error << "%" << endl;
            } else {
                Cluster no_clus(data_eda, k);
                SimSearch sim(data_eda, queries_eda, clusters, no_clus);
                experimentResult exp = sim.runExperiment(m, k);

                base_avg_distance[i] = exp.avg_distance;
                exp.error = 0.0f;

                cout << endl;
                cout << "EXPERIMENT PARAMS: m=" << m << ", k=" << k << endl;
                cout << "search time = " << exp.time_no_sort << " microseconds" << endl;
                cout << "total time = " << exp.time_total << " microseconds" << endl;
                cout << "total comparisons = " << exp.total_comparisons << endl;
                cout << "avg distance = " << exp.avg_distance << endl;
                cout << "error = " << exp.error << "%" << endl;
            }
        }
    }
    return 0;
} 