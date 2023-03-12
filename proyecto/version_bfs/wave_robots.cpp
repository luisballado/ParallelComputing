#include <iostream>
#include <vector>
#include <string>
#include <regex>
#include <unordered_map>
#include <map>
#include <fstream>
#include <queue>
#include <limits>

// Read a file
// m n - represent the numbers of row and columns
// get the given matrix
// given a matrix pass it to a adjacency list
// run the bfs
// make a draw rutine

class Grafo{

private:
  std::vector<std::vector<int>> adj;

public:
  Grafo(int n){
    adj.resize(n);
  }

  void agregar(int u,int v){
    adj[u].push_back(v);
    adj[v].push_back(u);
  }

  void imprimir(){
    for(int i = 0;i<adj.size();i++){
      if(adj[i].size() > 0){
	std::cout << "Nodo " << i << std::endl;
	for(int j=0;j<adj[i].size();j++){
	  std::cout << adj[i][j] << '-';
	}
	std::cout << std::endl;
      }
    }
  }
};
  
int main(){
  int m;
  int n;

  std::cin >> m >> n;

  std::cout << "m: " << m << "n: " << n << std::endl;

  //grafo de m*n
  Grafo grafo(n*m);
  
  grafo.agregar(0,1);
  grafo.agregar(0,1);
  grafo.agregar(0,1);
  //grafo.agregar(0,10);
  //grafo.agregar(1,0);
  //grafo.agregar(1,11);
  //grafo.agregar(1,2);
  grafo.imprimir();
  
  return 0;
  
}
