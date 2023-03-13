#include <climits>
#include <iostream>
#include <queue>
#include <pthread.h>
#include <vector>
#include <algorithm>

#define NUM_THREADS 4

using namespace std;

struct Graph {
    int V; 
    vector<int> *adj; 
};

Graph* createGraph(int V) {
    Graph* graph = new Graph;
    graph->V = V;
    graph->adj = new vector<int>[V];
    return graph;
}

void addEdge(Graph* graph, int v, int w) {
    graph->adj[v].push_back(w);
    graph->adj[w].push_back(v);
}

struct thread_data {
    int thread_id;
    int* distance;
    bool* visited;
    queue<int>* q;
};

void* bfs_parallel(void* arg) {
    thread_data* data = (thread_data*) arg;
    int thread_id = data->thread_id;
    int* distance = data->distance;
    bool* visited = data->visited;
    queue<int>* q = data->q;
    int V = q->size();

    while (!q->empty()) {
        int u = q->front();
        q->pop();

        for (int i = 0; i < V; i++) {
            if (visited[i] == false && distance[i] == thread_id) {
                visited[i] = true;
                distance[i] = thread_id + 1;
                q->push(i);
            }
        }
    }

    pthread_exit(NULL);
}

void bfs(Graph* graph, int src, int* distance, bool* visited) {
    queue<int> q;
    q.push(src);
    visited[src] = true;

    for (int i = 0; i < graph->V; i++) {
        distance[i] = INT_MAX;
    }
    distance[src] = 0;

    pthread_t threads[NUM_THREADS];
    thread_data thread_data_array[NUM_THREADS];

    for (int i = 0; i < NUM_THREADS; i++) {
        thread_data_array[i].thread_id = i;
        thread_data_array[i].distance = distance;
        thread_data_array[i].visited = visited;
        thread_data_array[i].q = &q;
        int rc = pthread_create(&threads[i], NULL, bfs_parallel, (void*) &thread_data_array[i]);
        if (rc) {
            cout << "Error:unable to create thread," << rc << endl;
            exit(-1);
        }
    }

    for (int i = 0; i < NUM_THREADS; i++) {
        pthread_join(threads[i], NULL);
    }
}

int main() {
    Graph* graph = createGraph(7);
    addEdge(graph, 0, 1);
    addEdge(graph, 0, 2);
    addEdge(graph, 1, 3);
    addEdge(graph, 1, 4);
    addEdge(graph, 2, 5);
    addEdge(graph, 2, 6);

    int distance[graph->V];
    bool visited[graph->V];

    bfs(graph, 0, distance, visited);

    cout << "Distance from vertex 0: ";
    for (int i = 0; i < graph->V; i++) {
        cout << distance[i] << " ";
    }
    cout << endl;

    return 0;
}
