#include <iostream>
#include <vector>
#include <set>

typedef std::pair<long, long> move;

std::set<int> ends;

long solve(int node, int moves_left, std::vector<std::vector<long>> &DP,std::vector<std::vector<move>> &moves){
  if(DP[node][moves_left] != -1) return DP[node][moves_left];
  if(moves_left == 0) return 0;
  
  if(ends.find(node) == ends.end()){
    long best = -1;
    for (auto mv : moves[node]){
      best = std::max(best, solve(mv.first, moves_left-1, DP, moves) + mv.second);
    }
    
    DP[node][moves_left] = best;
    return DP[node][moves_left];
  }
  
  DP[node][moves_left] = solve(0, moves_left, DP, moves);
  return DP[node][moves_left];
}

int main(){
  std::ios_base::sync_with_stdio(false);
  int t; 
  std::cin >> t;
  
  while(t--){
    long n,m,x,k;
    std::cin >> n >> m >> x >> k;
    
    std::vector<std::vector<move>> moves(n, std::vector<move>());
    
    long u, v, p;
    std::vector<bool> is_end(n, true);
    for(int i = 0; i < m; ++i){
      std::cin >> u >> v >> p;
      
      moves[u].push_back({v,p});
      is_end[u] = false;
    }
    
    std::vector<std::vector<long>> DP(n, std::vector<long>(k+1, -1));
    ends.clear();
    for(int i = 0; i < n; ++i){
      if(is_end[i]) ends.insert(i);
    }
    
    if(solve(0, k, DP, moves) < x) std::cout<< "Impossible\n";
    else {
      int best_sol = 9999999;
      
      int left = 1, right = k ;
      int mid = left + (right-left)/2;
      while(left <= right){
        if(solve(0, mid, DP, moves) >= x){
          best_sol = std::min(best_sol, mid);
          right = mid-1;
        }
        else{
          left = mid+1;
        }
        
        mid = left + (right-left)/2;
      }
      
      std::cout << best_sol << '\n';
    }
  }
  
  return 0;
}