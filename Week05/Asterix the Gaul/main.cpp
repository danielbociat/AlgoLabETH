#include <iostream>
#include <vector>
#include <algorithm>

typedef std::pair<long, long> move;

bool cmpMove(move a, move b){
  return a.second < b.second;
}

bool isSolvable(long pot_value, std::vector<move> &moves, long n, long D, long T){
  int hf_card = n/2;
  
  std::vector<move> set1;
  for(int s = 0; s < 1<<hf_card; ++s){
    long curr_dist = 0, curr_time = 0;
    for(int i = 0; i < hf_card; ++i){
      if(s & 1<<i){
        curr_dist += moves[i].first + pot_value;
        curr_time += moves[i].second;
      }
    }
    
    set1.push_back({curr_dist, curr_time});
  }
  sort(set1.begin(), set1.end(), cmpMove);
  
  std::vector<move> set2;
  for(int s = 0; s < (1<<(n-hf_card)); ++s){
    long curr_dist = 0, curr_time = 0;
    for(int i = 0; i < n-hf_card; ++i){
      if(s & (1<<i)){
        curr_dist += moves[i+hf_card].first + pot_value;
        curr_time += moves[i+hf_card].second;
      }
    }
    
    set2.push_back({curr_dist, curr_time});
  }
  sort(set2.begin(), set2.end(), cmpMove);
  
  long best_dist = set2[0].first;
  for(int i = 1; i < (int)set2.size(); ++i){
    if(set2[i].first > best_dist){
      best_dist = set2[i].first;
    }
    set2[i].first = best_dist;
  }
  
  for(auto mv : set1){
    long target_dist = D - mv.first;
    long max_time = T - mv.second;
    
    int l = 0, r = set2.size()-1, m; 
    while(l <= r){
      m = l + (r-l)/2;
      
      if(set2[m].second < max_time){
        if(set2[m].first >= target_dist) return true;
        l = m+1;
      }
      else{
        r = m-1;
      }
    }
  }
  
  return false;
}

int main(){
  std::ios_base::sync_with_stdio(false);
  
  int c;
  std::cin >> c;
  while(c--){
    long n, m, D, T;
    std::cin >> n >> m >> D >> T;
    
    std::vector<move> moves(n);
    for(int i = 0; i < n; ++i){
      long d,t;
      std::cin >> d >> t;
      
      moves[i] = {d, t};
    }
    std::vector<long> potions(m+1);
    
    potions[0] = 0;
    for(int i = 1; i <= m; ++i){
      std::cin >> potions[i];
    }
    
    long left = 0, right = m, mid;
    long best_sol = m+1;
    while(left <= right){
      mid = left + (right-left)/2;
      
      if(isSolvable(potions[mid], moves, n, D, T)){
        right = mid-1;
        best_sol = std::min(best_sol, mid);
      } 
      else{
        left = mid+1; 
      }
    }
    
    if(best_sol == m+1){
      std::cout << "Panoramix captured";
    }
    else{
      std::cout << best_sol;
    }
    std::cout << '\n';
  }
  
  return 0;
}