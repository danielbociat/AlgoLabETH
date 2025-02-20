#include <iostream>
#include <vector>
#include <deque>
#include <set>

int main()
{
  std::ios_base::sync_with_stdio(false);
  
  int t;
  std::cin >> t;
  while(t--){
    int n;
    std::cin >> n;
    
    int x;
    std::vector <bool> beach(2001001, false);
    
    std::set<int> sol;
    std::set<int> pos;
    int best_num = -1;
    int best_dist = -1;
    for(int i = 0; i < n; ++i){
      std::cin >> x;
      beach[x + 1000000] = true;
    }
    
    for(int j = 0; j <= 100; ++j){
      if(beach[j]){
        best_dist = j;
        pos.insert(j);
      }
    }
    sol.insert(0);
    best_num = (int)pos.size();
    
    for(int i = 1; i <= 2000001; ++i){
      if(beach[i + 100]){
        pos.insert(i + 100);
      }
      if((i - 101 >= 0) && beach[i - 101]){
        pos.erase(pos.begin());
      }
      
      if(!pos.empty()){
        if((int)pos.size() > best_num){
          best_num = pos.size();
          best_dist = std::max(std::abs(*pos.begin() - i), std::abs(*pos.rbegin() - i));
          sol.clear();
          sol.insert(i);
        }
        else{
          if((int)pos.size() == best_num){
            int dist = std::max(std::abs(*pos.begin() - i), std::abs(*pos.rbegin() - i));
            
            if(dist < best_dist){
              best_dist = dist;
              sol.clear();
              sol.insert(i);
            }
            else if(dist == best_dist){
              sol.insert(i);
            }
          }
        }
        
      }
    }
    
    std::cout << best_num  << ' ' << best_dist << '\n';
    for(auto el : sol){
      std::cout << el-1000000 << ' ';
    }
    std::cout << '\n';
    
   
  }
  
  return 0;
}