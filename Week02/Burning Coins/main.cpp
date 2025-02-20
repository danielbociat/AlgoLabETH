#include <iostream>
#include <vector>

std::vector <std::vector<int>> DP;

int game(std::vector<int> &v, int i, int j, bool my_turn){
  if(DP[i][j] != -1) return DP[i][j];
  
  if(my_turn){
    if(i == j) DP[i][i] = v[i];
    else
      DP[i][j] = std::max(v[i] + game(v, i+1,j,!my_turn), v[j] + game(v, i,j-1,!my_turn));

    return DP[i][j];
  }
  
  if(i == j) DP[i][i] = 0;
  else
    DP[i][j] = std::min(game(v, i+1,j,!my_turn), game(v, i,j-1,!my_turn));
  
  return DP[i][j];
}

int main(){
  std::ios_base::sync_with_stdio(false);
  
  int t;
  std::cin >> t;
  
  while(t--){
    int n;
    std::cin >> n;
    
    std::vector<int>v(n,0);
    
    for(int i = 0; i < n; ++i){
      std::cin >> v[i];
    }
    
    DP = std::vector<std::vector<int>> (n, std::vector<int>(n,-1));

    std::cout << game(v, 0, n-1, true) << '\n';
  }
  
  return 0;
}