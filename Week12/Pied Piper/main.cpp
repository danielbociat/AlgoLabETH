#include <iostream>
#include <vector>

int main(){
  std::ios_base::sync_with_stdio(false);
  
  int t;
  std::cin >> t;
  
  while(t--){
    int n, m;
    std::cin >> n >> m;
    
    std::vector<std::vector<std::pair<int, long>>> forward(n, std::vector<std::pair<int, long>>());
    std::vector<std::vector<std::pair<int, long>>> backward(n, std::vector<std::pair<int, long>>());
    
    int u, v, f;
    for(int i = 0; i < m; ++i){
      std::cin >> u >> v >> f;
      
      if(u < v){
        forward[u].push_back({v, f});
      }
      else{
        backward[v].push_back({u, f});
      }
    }
    
    std::vector<std::vector<long>> dp(n, std::vector<long>(n, -1));
    dp[0][0] = 0;
    
    for(int i = 0; i < n; ++i){
      for(int j = 0; j < n; ++j){
        if(i == j && i != 0) continue;
        if(dp[i][j] == -1) continue;
        
        for(auto pr : forward[i]){
          int k = pr.first;
          long v = pr.second;
          // DP[i][j], 0 -> i, j -> 0
          if(k >= j)
            dp[k][j] = std::max(dp[k][j], dp[i][j] + v);
        }
        
        for(auto pr : backward[j]){
          int k = pr.first;
          long v = pr.second;
          
          if(k >= i)
            dp[i][k] = std::max(dp[i][k], dp[i][j] + v);
        }
      }
    }
    
    std::cout << dp[n-1][n-1] << '\n';
  }
  
  return 0;
}