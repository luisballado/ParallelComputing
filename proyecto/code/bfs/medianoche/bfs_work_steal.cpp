#include <iostream>
#include <vector>
#include <queue>
#include <omp.h>

using namespace std;

void bfs(int start_node, vector<vector<int>>& graph, vector<int>& distances, int num_threads) {
    int n = graph.size();
    vector<bool> visited(n, false);
    queue<int> work_queues[num_threads];

    // Add the start node to the queue of each thread
    #pragma omp parallel num_threads(num_threads)
    {
        int tid = omp_get_thread_num();
        work_queues[tid].push(start_node);
    }

    while (true) {
        // Process the work queue of each thread
        #pragma omp parallel num_threads(num_threads)
        {
            int tid = omp_get_thread_num();
            queue<int>& work_queue = work_queues[tid];
            while (!work_queue.empty()) {
                int node = work_queue.front();
                work_queue.pop();

                if (!visited[node]) {
                    visited[node] = true;
                    distances[node] = 0;

                    // Visit neighbors
                    for (int i = 0; i < graph[node].size(); i++) {
                        int neighbor = graph[node][i];
                        if (!visited[neighbor]) {
                            distances[neighbor] = distances[node] + 1;

                            // Add neighbor to the work queue of a random thread
                            int target_tid = rand() % num_threads;
                            #pragma omp critical
                            {
                                work_queues[target_tid].push(neighbor);
                            }
                        }
                    }
                }
            }
        }

        // Check if all nodes have been visited
        bool all_visited = true;
        for (int i = 0; i < n; i++) {
            if (!visited[i]) {
                all_visited = false;
                break;
            }
        }

        if (all_visited) {
            break;
        }
    }
}

int main() {
    int num_threads = 4;
    omp_set_num_threads(num_threads);

    vector<vector<int>> graph = {
        {1, 2},
        {0, 2, 3},
        {0, 1, 4},
        {1},
        {2, 5},
        {4}
    };

    int start_node = 0;
    vector<int> distances(graph.size(), -1);

    bfs(start_node, graph, distances, num_threads);

    for (int i = 0; i < distances.size(); i++) {
        cout << "Distance from " << start_node << " to " << i << ": " << distances[i] << endl;
    }

    return 0;
}
