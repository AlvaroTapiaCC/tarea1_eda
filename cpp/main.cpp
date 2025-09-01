#include <iostream>
#include "matrix.hpp"
#include "simsearch.hpp"

using namespace std;

int main() {
    Matrix data_eda("../../data_eda.npy");
    Matrix queries_eda("../../queries_eda.npy");

    SimSearch sim(data_eda, queries_eda);

    int m_values[] = {8, 16, 32, 64, 128};
    int k_values[] = {0};

    for (int i = 0; i < 5; i++) {
        int m = m_values[i];
        for (int j = 0; j < 1; j++) {
            int k = k_values[j];
            experimentResult exp = sim.runExperiment(m, k);
            cout << endl;
            cout << "EXPERIMENT PARAMS: m=" << m << ", k=" << k << endl;
            cout << "search time = " << exp.time_no_sort << " microseconds" << endl;
            cout << "total time = " << exp.time_total << " microseconds" << endl;
            cout << "total comparisons = " << exp.total_comparisons << endl;
        }
    }
    return 0;
} 