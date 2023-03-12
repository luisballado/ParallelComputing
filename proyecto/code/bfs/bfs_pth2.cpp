#include <iostream>
#include <fstream>
#include <vector>
#include <queue>
#include <pthread.h>

using namespace std;

// Struct to represent a position
struct Position {
  int x;
  int y;
};

// Struct to represent a BFS node
struct BFSNode {
  Position pos;
  int dist;
};

// Global variables for the path and the starting and ending positions
vector<vector<char>> dungeon;
Position start_pos, end_pos;

// Function to read in the grid file and initialize the path
void init_dungeon(string filename) {
  ifstream file(filename);
  string line;
  while (getline(file, line)) {
    vector<char> row;
    for (char c : line) {
      row.push_back(c);
      if (c == 'S') {
	start_pos = {row.size() - 1, dungeon.size()};
      } else if (c == 'E') {
	end_pos = {row.size() - 1, dungeon.size()};
      }
    }
    dungeon.push_back(row);
  }
}

// Function to check if a position is within the bounds of the path
bool in_bounds(Position pos) {
  return pos.x >= 0 && pos.x < dungeon[0].size() && pos.y >= 0 && pos.y < dungeon.size();
}

// Function to check if a position is a valid move (not a wall)
bool is_valid_move(Position pos) {
  return dungeon[pos.y][pos.x] != '#';
}

// Function to perform BFS from the starting position
void* bfs(void* arg) {
  // Extract the thread ID from the argument
  int id = *((int*)arg);
  
  // Initialize the visited and frontier sets
  vector<vector<bool>> visited(dungeon.size(), vector<bool>(dungeon[0].size()));
  queue<BFSNode> frontier;
  frontier.push({start_pos, 0});
  
  // Loop until the frontier is empty or the goal is found
  while (!frontier.empty()) {
    // Dequeue the next node from the frontier
    BFSNode curr_node = frontier.front();
    frontier.pop();
    
    // Check if the goal has been reached
    if (curr_node.pos.x == end_pos.x && curr_node.pos.y == end_pos.y) {
      cout << "Thread " << id << " found the goal at distance " << curr_node.dist << endl;
      pthread_exit(NULL);
      exit(-1);
    }
    
    // Visit the current node
    if (visited[curr_node.pos.y][curr_node.pos.x]) {
      continue;
    }
    visited[curr_node.pos.y][curr_node.pos.x] = true;
    
    // Enqueue the neighboring nodes
    vector<Position> neighbors = {{curr_node.pos.x - 1, curr_node.pos.y},
                                  {curr_node.pos.x + 1, curr_node.pos.y},
                                  {curr_node.pos.x, curr_node.pos.y - 1},
                                  {curr_node.pos.x, curr_node.pos.y + 1}};

    for (Position neighbor : neighbors) {
      if (in_bounds(neighbor) && is_valid_move(neighbor) && !visited[neighbor.y][neighbor.x]) {
	frontier.push({neighbor, curr_node.dist + 1});
      }
    }
  }
  
  // If the goal was not found, print a message indicating so
  cout << "Thread " << id << " did not find the goal" << endl;
  pthread_exit(NULL);
}

int main(int argc, char* argv[]) {
  // Initialize the dungeon from the grid file
  init_dungeon("grid1.txt");
  
  // Create an array of thread IDs
  pthread_t threads[4];
  
  // Create each thread and pass in its ID as an argument
  int thread_ids[] = {0, 1, 2, 3};
  for (int i = 0; i < 4; i++) {
    pthread_create(&threads[i], NULL, bfs, (void*)&thread_ids[i]);
  }
  
  // Wait for all threads to finish
  for (int i = 0; i < 4; i++) {
    pthread_join(threads[i], NULL);
  }
  
  return 0;
}
