#include <iostream>
#include <vector>
#include <algorithm>

typedef std::pair<int, int> bomb;

void sol(int n, std::vector<bomb> &bombs, std::vector<int> &t){
  for(int i = 0; i < (n-1)/2; ++i){
    t[2*i+1] = std::min(t[2*i+1], t[i] - 1);
    t[2*i+2] = std::min(t[2*i+2], t[i] - 1);
  }
  sort(t.begin(), t.end());
  
  bool flag = true;
  
  for(int i = 0; i < n; ++i){
    if(i >= t[i]){
      flag = false;
      break;
    }
  }
  
  if(flag) std::cout << "yes\n";
  else std::cout << "no\n";
}

int main() {
  std::ios_base::sync_with_stdio(false);
  
  int c; 
  std::cin >> c;
  
  while(c--) {
    int n; 
    std::cin >> n;
    
    std::vector<int> t(n);
    std::vector<bomb> bombs(n);
  
    for(int i = 0; i < n; i++) {
      std::cin >> t[i];
      bombs[i] = {t[i], i};
    }
    
    sol(n, bombs, t);
  }
  
  return 0;
}