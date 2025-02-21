#include <iostream>
#include <vector>
#include <queue>
#include <climits>
#include <cmath>
#include <set>

typedef std::vector<int> VI;
typedef std::vector<VI> VVI;
typedef std::vector<VVI> VVVI;
typedef std::vector<VVVI> VVVVI;

int n, k, m;

VI insert(VI &q, int val){
  return {val, q[0], q[1]};
}

VI encode(VI &q_north, VI &q_south, int diff){
  return {
    5*q_north[0] + q_north[1],
    5*q_south[0] + q_south[1],
    diff+12
  };
}

int count_distinct(VI &q){
  if(m == 2) return (int)std::set<int>({0, q[0], q[1]}).size() - 1;
  return (int)std::set<int>({0, q[0], q[1], q[2]}).size() - 1;
}

int excitement(bool use_north, VI &q_north, VI &q_south, int diff){
  if(use_north){
    return count_distinct(q_north) * 1000 - (int)(std::pow(2, std::abs(diff)));
  }
  
  return count_distinct(q_south) * 1000 - (int)(std::pow(2, std::abs(diff)));
}

int solve(VVVVI &DP, int idx, VI &fighters, VI &q_north, VI &q_south, int diff){
  
  if(std::abs(diff) >= 12) return INT_MIN;
  if(idx == n) return 0;
  
  VI key = encode(q_north, q_south, diff);
  
  if(DP[idx][key[0]][key[1]][key[2]] != -1) return DP[idx][key[0]][key[1]][key[2]];
  
  int curr_fighter = fighters[idx]; 
  
  VI new_north = insert(q_north, curr_fighter);
  VI new_south = insert(q_south, curr_fighter);
  
  int val_north = excitement(true, new_north, q_south, diff+1);
  int val_south = excitement(false, q_north, new_south, diff-1);
  
  int best = -1;
  if(val_north > 0){
    best = std::max(best, val_north + solve(DP, idx+1, fighters, new_north, q_south, diff+1)); 
  }
  if(val_south > 0){
    best = std::max(best, val_south + solve(DP, idx+1, fighters, q_north, new_south, diff-1)); 
  }
  
  DP[idx][key[0]][key[1]][key[2]] = best;
  return best;
}

int main(){
  std::ios_base::sync_with_stdio(false);
  
  int t;
  std::cin >> t;
  while(t--){
    std::cin >> n >> k >> m;
    
    VI fighters(n);

    for(int i = 0; i < n; ++i){
      std::cin >>  fighters[i];
      
      /// increase type by 1 to have type 0 = empty place
      fighters[i]++;
    }
    
    VVVVI DP(n, VVVI(25, VVI(25, VI(25, -1))));
    
    // init queue for north and south as empty
    VI q_north = {0, 0, 0};
    VI q_south = {0, 0, 0};
    
    std::cout << solve(DP, 0, fighters, q_north, q_south, 0) << '\n';
    
  }
  
  return 0;
}