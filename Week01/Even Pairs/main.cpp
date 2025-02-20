#include <iostream>

int main(){
  std::ios_base::sync_with_stdio(false);
  
  int t;
  std::cin>>t;
  
  while(t--){
    int n;
    std::cin>>n;
    
    int odd = 0, even = 1;
    int curr_sum = 0;
    
    int x;
    while(n--){
      std::cin>>x;
      
      curr_sum += x;
      
      if(curr_sum%2) odd++;
      else even++;
    }
    
    std::cout << odd*(odd-1)/2 + even*(even-1)/2 << '\n';
  }
  
  return 0;
}