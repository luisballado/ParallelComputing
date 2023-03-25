#include <iostream>
#include <queue>
#include <vector>
#include <fstream>
#include <unistd.h>

using namespace std;

const int MAXN = 1000; // maximum size of grid
const int INF = 1000000000; // large enough value for uninitialized nodes

int N, M; // dimensions of grid
int Sx, Sy; // coordinates of source node
vector<int> adj[MAXN*MAXN]; // adjacency list
int dist[MAXN*MAXN]; // distance from source node to each node in grid

// read grid file and initialize adjacency list
void read_grid(string filename) {
    ifstream fin(filename);
    fin >> N >> M >> Sx >> Sy;
    for (int i = 0; i < N; i++) {
        for (int j = 0; j < M; j++) {
            char c;
            fin >> c;
            int u = i*M + j;
            if (c == '#') {
                continue; // skip obstacles
            }
            if (i > 0) {
                int v = (i-1)*M + j;
                adj[u].push_back(v);
                adj[v].push_back(u);
            }
            if (j > 0) {
                int v = i*M + (j-1);
                adj[u].push_back(v);
                adj[v].push_back(u);
            }
            if (i < N-1) {
                int v = (i+1)*M + j;
                adj[u].push_back(v);
                adj[v].push_back(u);
            }
            if (j < M-1) {
                int v = i*M + (j+1);
                adj[u].push_back(v);
                adj[v].push_back(u);
            }
        }
    }
}

// print distance from source node to each node in grid
void print_dist() {
  //sleep(1);
  system("clear");
  for (int i = 0; i < N; i++) {
        for (int j = 0; j < M; j++) {
            int u = i*M + j;
            if (dist[u] == INF) {
                cout << "-1 ";
            } else {
                cout << dist[u] << " ";
            }
        }
        cout << endl;
    }
}

// perform wavefront expansion from source node using BFS
void bfs(int source) {
    queue<int> q;
    q.push(source);
    dist[source] = 0;
    while (!q.empty()) {
        int u = q.front();
        q.pop();
        for (int v : adj[u]) {
            if (dist[v] == INF) {
                dist[v] = dist[u] + 1;
                q.push(v);
            }
	    print_dist();
        }
    }
}

int main() {
    string filename = "mapa.txt";
    read_grid(filename);
    for (int i = 0; i < N*M; i++) {
        dist[i] = INF; // initialize all distances to infinity
    }
    int source = Sx*M + Sy;
    bfs(source);
    print_dist();
    return 0;
}
