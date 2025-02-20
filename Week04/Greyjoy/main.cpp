///4
#include <iostream>
#include <vector>
#include <tuple>
#include <algorithm>
#include <unordered_map>

#define pairii std::pair<int, int>

int main(){
  std::ios_base::sync_with_stdio(false);
  
  int t;
  std::cin >> t;
  
  while(t--){
     int n, k, w;
     std::cin >> n >> k >> w;
     
     std::vector <int> c(n);
     
     for(int i = 0; i < n; ++i){
       std::cin >> c[i];
     }
     
     std::vector<std::vector<int>> waterways(w);
     std::vector<std::vector<int>> presum(w);
     
     for(int i = 0; i < w; ++i){
       int l;
       std::cin >> l;
       waterways[i] = std::vector<int>(l);
       presum[i] = std::vector<int>(l);
       
       int left;
       std::cin >> left; // should always be 0

       waterways[i][0] = c[0];
       presum[i][0] = c[0];
       
       for(int j = 1; j < l; ++j){
          int r;
          std::cin >> r;
          
          waterways[i][j] = c[r];
          presum[i][j] = c[r] + presum[i][j-1];
       }
     }
     
     int best_sol = 0;
     
     //only one waterway
     for(int i = 0; i < w; ++i){
       int left = 0, right = 0;
       int curr_val = waterways[i][0];
       int len = (int)waterways[i].size();
       
       while(left < len && right < len){
         if(curr_val < k){
           right++;
           
           if(right == len) break;
           
           curr_val += waterways[i][right];
           continue;
         }
         if(curr_val > k){
           curr_val -= waterways[i][left];
           left++;
           continue;
         }
         if(curr_val == k){
           best_sol = std::max(best_sol, right-left+1);
           
           right++;
           
           if(right == len) break;
           
           curr_val += waterways[i][right];
           
         }
       }
     }
     
     //2 waterways
     std::unordered_map<int, std::vector<pairii>> mp;
     
     for(int i = 0; i < w; ++i){
       for(int j = 1; j < (int)waterways[i].size(); ++j){
         mp[presum[i][j]].push_back(std::make_pair(j,i));
       }
     }
     
     for(auto nodes : mp){
        int target = k - nodes.first + c[0];
        
        if(mp.find(target) == mp.end()){
          continue;
        }
        
        for(auto node : nodes.second){
          for(auto el : mp[target]){
            if(el.second != node.second){
              best_sol = std::max(best_sol, el.first + node.first + 1);
            }
          }
        }
     }
     
     std::cout << best_sol << '\n';
  }
  
  return 0;
}