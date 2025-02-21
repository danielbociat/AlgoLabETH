#include <iostream>
#include <vector>
#include <set>
#include <queue>

int n,m,k;
std::vector<int> stk;
std::set <int> sol;
std::multiset<int> brightness;

void DFS(int curr, std::vector<std::vector<int>> &hairs, std::vector<int> &h){
  brightness.insert(h[curr]);
  stk.push_back(curr);
  
  if((int) brightness.size() == m){
    int lst = stk[stk.size() - m];
    
    if(*brightness.rbegin() - *brightness.begin() <= k){
      sol.insert(lst);
    }
    
    brightness.erase(brightness.find(h[lst]));
  }
  
  for(auto el : hairs[curr]){
    DFS(el, hairs, h);
  }
  
  if(brightness.find(h[curr]) != brightness.end()){
    brightness.erase(brightness.find(h[curr]));
  }
  if((int)brightness.size() < m && (int)stk.size() >= m){
    brightness.insert(h[stk[stk.size() - m]]);
  }
  stk.pop_back();
}

int main(){
  std::ios_base::sync_with_stdio(false);
  
  int t;
  std::cin >> t;
  while(t--){
    std::cin >> n >> m >> k;   
    
    std::vector <int> h(n);
    for(int i = 0; i < n; ++i){
      std::cin >> h[i];
    }
    
    int u,v;
    std::vector<std::vector<int>> hairs(n, std::vector<int>());
    for(int i = 0; i < n-1; ++i){
      std::cin >> u >> v;
      
      hairs[u].push_back(v);
    }
    
    brightness.clear(); sol.clear(); stk.clear(); 
    stk.reserve(n);
    
    DFS(0, hairs, h);
    
    if(sol.empty()){
      std::cout << "Abort mission\n";
    }
    else{
      for(auto el : sol){
        std::cout << el << ' ';
      }
      std::cout << '\n';
    }
  }
  
  return 0;
}