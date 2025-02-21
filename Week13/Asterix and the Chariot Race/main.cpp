#include <iostream>
#include <vector>

std::vector <std::vector<int>> DP;

int solve(int node, int parentSelected, std::vector<std::vector<int>> &stages, std::vector <int> &c){
  if(DP[node][parentSelected] != -1){
    return DP[node][parentSelected];
  } 
  
  int sumRoot = c[node];
  for(auto el : stages[node]){
    sumRoot += solve(el, 1, stages, c);
  }
  
  int sumNotRoot = 0;
  for(auto el : stages[node]){
    sumNotRoot += solve(el, 0, stages, c);
  }
  
  int sol;
  if(parentSelected){
    sol = std::min(sumRoot, sumNotRoot);
  }
  else{
    int best = 999999999;
    
    for(auto el : stages[node]){
      int cs = c[el];
      
      for(auto eel : stages[el]){
        cs += solve(eel, 1, stages, c);
      }
      
      cs += sumNotRoot - solve(el, 0, stages, c);
      best = std::min(best, cs);
    }
    
    sol = std::min(sumRoot, best);
  }
  
  DP[node][parentSelected] = sol;
  
  return DP[node][parentSelected];
}

int main(){
  std::ios_base::sync_with_stdio(false);
  
  int t;
  std::cin >> t;
  
  while(t--){
    int n;
    std::cin >> n;
    
    std::vector<std::vector<int>> stages(n, std::vector<int>());
    int i,j;
    for(int k = 0; k < n-1; ++k){
      std::cin >> i >> j;
    
      stages[i].push_back(j);
    }
    
    DP = std::vector<std::vector<int>>(n, std::vector<int>(2, -1));
    
    std::vector <int> c(n);
    for(int i = 0; i < n; ++i){
      std::cin >> c[i];
    }
    
    solve(0, 0, stages, c);
    
    std::cout << DP[0][0] << '\n';
  }
  
  return 0;
}