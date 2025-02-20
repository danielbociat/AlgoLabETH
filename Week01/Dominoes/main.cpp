#include <iostream>

int main(){
  int t;
  std::cin>>t;
  
  while(t--){
    unsigned int n;
    std::cin>>n;
    
    unsigned int maxi, h, sol = 1;
    std::cin>>maxi;
    bool flag = true;
    for(unsigned int i = 1; i < n; ++i){
      std::cin>>h;
      
      if(maxi > i && flag){
        sol++;
      }
      else flag = false;
      
      maxi = std::max(i+h, maxi);
    }
    
    std::cout << sol << '\n';
  }
  
  return 0;
}