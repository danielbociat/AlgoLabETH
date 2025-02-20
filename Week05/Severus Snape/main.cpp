#include <iostream>
#include <vector>
#include <algorithm>
#include <limits>

typedef std::pair<long, long> potionA;

int n, m;
long a, b;
long P, H, W;

bool cmpB(long a, long b){
  return a > b;
}

int main(){
  std::ios_base::sync_with_stdio(false);
  
  int t;
  std::cin >> t;
  
  while(t--){
    std::cin >> n >> m;
    std::cin >> a >> b;
    std::cin >> P >> H >> W;
    
    std::vector<potionA> potionsA(n);
    for(int i = 0; i < n; ++i){
      long p, h; 
      std::cin >> p >> h;
      potionsA[i] = {p, h};
    }
    
    std::vector<long> potionsB(m);
    for(int i = 0; i < m; ++i){
      std::cin >> potionsB[i];
    }
    sort(potionsB.begin(), potionsB.end(), cmpB);
    
    long neg_inf = std::numeric_limits<long>::min();
    std::vector<std::vector<std::vector<long>>> DP(n+1, 
    std::vector<std::vector<long>>(n+1, std::vector<long>(H+1, neg_inf)));
    
    // Max power obtainable, while having at least h happiness using only 1 potion out of the first i
    for(int i = 1; i <= n; ++i){
      for(int h = 0; h <= H; h++){
        if(potionsA[i-1].second >= h){
          DP[1][i][h] = std::max(potionsA[i-1].first, DP[1][i-1][h]);
        }
        else{
          DP[1][i][h] = DP[1][i-1][h];
        }
      }
    }
    
    // Max power obtainable, while having at least h happiness using only j potions out of the first i
    for(int j = 2; j <= n; ++j){
      for(int i = j; i <= n; ++i){
        for(int h = 0; h <= H; ++h){
          long h_without_pot_i = std::max(0L, h - potionsA[i-1].second);
          
          DP[j][i][h] = std::max(DP[j][i-1][h],
                          DP[j-1][i-1][h_without_pot_i] + potionsA[i-1].first);
        }
      }
    }
    
    long curr_wit = 0;
    int sol = -1;
    
    for(int i = 1; i <= m && sol==-1; ++i){
      curr_wit += potionsB[i-1];
      
      if(curr_wit >= W){
        for(int j = 1; j <= n ; ++j){
          if(curr_wit < W + a*j) break;
          
          if(DP[j][n][H] >= P + b*i){
            sol = j + i;
            break;
          }
        }
      }
    }
    
    std::cout<< sol << '\n';
  }
  
  return 0;
}