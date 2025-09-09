import numpy as np
import matplotlib
matplotlib.use('Agg')
import matplotlib.pyplot as plt
import glob

data = np.load('../data_eda.npy')
queries = np.load('../queries_eda.npy')
query = queries[0]

# Plot neighbors (your original code)
csv_files = sorted(glob.glob('../metrics/neighbors/neighbors_m*_k*.csv'))

for f in csv_files:
    neighbors = np.loadtxt(f, dtype = int)
    plt.figure()
    plt.scatter(data[:,0], data[:,1], color = 'red')
    plt.scatter(queries[:,0], queries[:,1], color = 'blue')
    plt.scatter(query[0], query[1], color = 'blue', s = 100, marker = 'x', label = 'Test Query')
    plt.scatter(data[neighbors, 0], data[neighbors, 1], color = 'green', label = 'Neighbors')
    base_name = f.split('/')[-1]
    plot_name = base_name.replace('neighbors_', 'plot_').replace('.csv', '.png')
    plt.title(base_name)
    plt.legend()
    plt.savefig(f'../metrics/plots/experiment_plots/{plot_name}')
    plt.close()

# Plot clusters (new code)
cluster_files = sorted(glob.glob('../metrics/clusters/clusters_k*.csv'))

for f in cluster_files:
    clusters = []
    centroids = []
    current_pts = []
    with open(f) as fin:
        for line in fin:
            if line.startswith('# cluster'):
                if current_pts:
                    clusters.append(np.array(current_pts))
                    current_pts = []
                centroid = [float(x) for x in line.strip().split(',')[1:]]
                centroids.append(centroid)
            elif line.startswith('#'):
                continue
            else:
                parts = line.strip().split(',')
                coords = [float(x) for x in parts[1:]]
                current_pts.append(coords)
        if current_pts:
            clusters.append(np.array(current_pts))

    plt.figure(figsize=(6, 6))
    plt.scatter(data[:, 0], data[:, 1], color='gray', alpha=0.3)
    colors = plt.cm.get_cmap('tab10', len(clusters))
    for i, pts in enumerate(clusters):
        if len(pts) > 0:
            plt.scatter(pts[:, 0], pts[:, 1], color=colors(i))
    centroids = np.array(centroids)
    if len(centroids) > 0:
        plt.scatter(centroids[:, 0], centroids[:, 1], color='black', marker='x', s=100)
    plt.title(f.split('/')[-1])
    base_name = f.split('/')[-1]
    plot_name = base_name.replace('clusters_', 'plot_').replace('.csv', '.png')
    plt.savefig(f'../metrics/plots/cluster_plots/{plot_name}')
    plt.close()