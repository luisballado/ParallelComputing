#include <iostream>
#include <queue>
#include <vector>
using namespace std;

void bfs(vector<vector<int>>& adj_list, int start_node, vector<int>& distance) {
    queue<int> q;
    distance[start_node] = 0;
    q.push(start_node);

    while (!q.empty()) {
        int node = q.front();
        q.pop();

        for (int neighbor : adj_list[node]) {
            if (distance[neighbor] == -1) {
                distance[neighbor] = distance[node] + 1;
                q.push(neighbor);
            }
        }
    }
}

int main() {
    int num_nodes = 7;
    int start_node = 0;
    vector<vector<int>> adj_list(num_nodes);
    adj_list[0] = {1, 2};
    adj_list[1] = {0, 3, 4};
    adj_list[2] = {0, 4};
    adj_list[3] = {1, 5};
    adj_list[4] = {1, 2, 5};
    adj_list[5] = {3, 4, 6};
    adj_list[6] = {5};

    vector<int> distance(num_nodes, -1);
    bfs(adj_list, start_node, distance);

    for (int i = 0; i < num_nodes; i++) {
        cout << "Distance from " << start_node << " to " << i << " is " << distance[i] << endl;
    }

    return 0;
}
