#include <iostream>
#include <vector>
#include <algorithm>

typedef std::pair<int, int> boat;

bool cmp(boat a, boat b){
  return a.second < b.second;
}

int main(){
  std::ios_base::sync_with_stdio(false);
  
  int t;
  std::cin >> t;
  while(t--){
    int n;
    std::cin >> n;
    
    std::vector<boat> boats(n);
    for(int i = 0; i < n; ++i){
      int l, p;
      std::cin >> l >> p;
      boats[i] = {l,p};
    }
    
    sort(boats.begin(), boats.end(), cmp);
    
    if(n == 1) {
      std::cout << 1 << '\n';
      continue;
    }
    
    int sol = 2;
    boat prev_boat = boats[0];
    boat curr_boat = boats[1];
    int prev_block = prev_boat.second;
    int curr_block = std::max(prev_block + curr_boat.first, curr_boat.second);
    
    for(int i = 2; i < n; ++i){
      curr_boat = boats[i];
      
      int l = curr_boat.first;
      int p = curr_boat.second;
      
      if(p - l >= curr_block){
        prev_block = curr_block;
        curr_block = p;
        sol++;
      }
      else if(curr_block <= p){
        prev_block = curr_block;
        curr_block = curr_block + l;
        sol++;
      } else {
        curr_block = std::min(curr_block, std::max(prev_block + l, p));
      }
      
    }
    
    std::cout << sol << '\n';
  }
  
  return 0;
}