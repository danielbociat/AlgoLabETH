#include <iostream>
#include <vector>

int main(){
  std::ios_base::sync_with_stdio(false);
  
  int t;
  std::cin >> t;
  
  while(t--){
    int n, m;
    std::cin >> n >> m;
    
    int r,b;
    std::cin >> r >> b;
    
    std::vector<std::vector<int>> trans (n+1, std::vector<int>());
    
    for(int i = 0; i < m; ++i){
      int u, v;
      std::cin >> u >> v;
      
      trans[u].push_back(v);
    }
    
    std::vector<int> mini(n+1, 999999999);
    std::vector<int> maxi(n+1, -1);
    mini[n] = 0, maxi[n] = 0;
    
    for(int i = n - 1; i >= 0; --i){
      for(int j : trans[i]){
        mini[i] = std::min(mini[i], 1 + maxi[j]);
        maxi[i] = std::max(maxi[i], 1 + mini[j]);
      }
    }
    
    if(mini[r] < mini[b]){
      std::cout << 0;
    }
    else if(mini[r] == mini[b] && mini[r] % 2 == 1){
      std::cout << 0;
    }
    else{
      std::cout << 1;
    }
    std::cout << '\n';
    
  }
  
  return 0;
}
