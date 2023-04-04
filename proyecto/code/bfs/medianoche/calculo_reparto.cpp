#include <iostream>
#include <unistd.h>
#include <vector>

std::ostream &operator<<(std::ostream &os, const std::vector<int> &input)
{
    for (auto const &i: input) {
        os << i << " ";
    }
    return os;
}

int main(){

  int robots;
  int threads;

  double work;
  int sum_work = 0;
  int sobra;
  std::cin >> robots >> threads;

  work = robots / threads;

  std::vector<int> workers;
  //si work es entero
  if(work*threads == robots){
    std::cout << "todo fine " << work << std::endl; 
  }else{
    //me quedo con el resultado de la parte entera y completo las piedras
    work = int (work);
    for(int i=0;i<threads;i++){
      std::cout << "repartiendo de a " << work << std::endl; 
      sum_work = sum_work + work;
      workers.push_back(work);
    }
    sobra = robots - sum_work;
    std::cout << "me sobra " << sobra  << std::endl;
    
    
  }
  // imprimir vector
  for(int w=0;w<workers.size();++w){

    workers[w] = workers[w] + 1;
    sobra = sobra - 1;
    //std::cout << workers[w] << std::endl;
    if(sobra==0)
      break;
    
  }

  std::cout << workers << std::endl;
    
}
