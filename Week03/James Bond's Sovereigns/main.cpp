#include <iostream>
#include <vector>

std::vector <std::vector<int>> DP;

int sol(std::vector<int> &x, int i, int j, int curr_pers, int k, int m){
  
  if(DP[i][j] >= 0) return DP[i][j];
  
  if(i == j){
    if(curr_pers == k) DP[i][j] = x[i];
    else DP[i][j] = 0;
    
    return DP[i][j];
  }
  
  if(curr_pers == k){
    DP[i][j] = std::max(x[i] + sol(x,i+1,j,(curr_pers+1)%m,k,m), 
                        x[j] + sol(x,i,j-1,(curr_pers+1)%m,k,m));
    return DP[i][j];
  }
  
  DP[i][j] = std::min(sol(x,i+1,j,(curr_pers+1)%m,k,m), sol(x,i,j-1,(curr_pers+1)%m,k,m));
  return DP[i][j];
}

int main(){
  std::ios_base::sync_with_stdio(false);
  
  int t;
  std::cin >> t;

  while(t--){
    int n, m, k;
    std::cin >> n >> m >> k;
    
    std::vector <int> x(n, 0);
    for(int i = 0; i < n; ++i){
      std::cin >> x[i];
    }
    
    DP = std::vector<std::vector<int>>(n, std::vector<int>(n, -1));
   
    std::cout << sol(x, 0, n-1, 0, k, m) << '\n';
  }
  
  return 0;
}