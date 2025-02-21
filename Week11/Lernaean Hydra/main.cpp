///2
#include <iostream>
#include <vector>

#define MAX 9999999

int n, m;
int k,d;

int main(){
  std::ios_base::sync_with_stdio(false);
  
  int t;
  std::cin >> t;
  while(t--){
    std::cin >> n >> m;
    std::cin >> k >> d;
    
    std::vector<std::vector<std::vector<int>>> mp(n);
    std::vector<int> seq(k);
    for(int i = 0; i < m; ++i){
      for(int j = 0; j < k; ++j){
        std::cin >> seq[j];
      }
      
      int idx = seq[k-1];
      int tg = idx;
      bool flag = true;
      
      for(int j = k-1; j >= 0; --j){ // eliminate unsable move sequences
        if(seq[j] < tg - 1){
          flag = false; 
          break;
        }
        tg = std::min(tg, seq[j]);
      }
      if(flag){
        mp[idx].push_back(seq);
      }
    }
    
    std::vector<std::vector<int>> DP(n, std::vector<int>(d,MAX));
    for(int i = 0; i < (int)mp[0].size(); ++i){ // prepopulate cutting 0th head using i th move
      DP[0][i] = k;
    }
    
    for(int i = 1; i < n; ++i){ //cut head i
      for(int j = 0; j < (int)mp[i].size(); ++j){ // using pattern mp[i][j] / move set j
        for(int kj = 0; kj < (int)mp[i-1].size(); ++kj){ // used pattern mp[i-1][kj] to cut head i-1
          
          bool flag = false, is_valid = true;
          int prev_pattern_idx = k-1, to_add = 0;
          
          for(int kkj = k-1; kkj >= 0; kkj--){ /// go through the moves of pattern  mp[i][j]
            
            if(mp[i][j][kkj] == i-1){ // if we saw the end of the previous pattern we may reuse it
              flag = true;
            }
            
            if(flag){
              if(mp[i][j][kkj] != mp[i-1][kj][prev_pattern_idx]){ // cannot use previous pattern
                is_valid = false;
                break;
              }
              prev_pattern_idx--; // check deeper
            }
            else{
              to_add++; // need to use a move
            }
          }
          
          if(is_valid){
            if(!flag) DP[i][j] = std::min(DP[i][j], DP[i-1][kj] + k); // cannot shortcut the prev pattern, add k 
            else DP[i][j] = std::min(DP[i][j], DP[i-1][kj] + to_add); // can shortcut thre prev pattern, add only whats left
          }
        }
      }
    }
    
    int mini = MAX; 
    
    for(int i = 0; i < (int)mp[n-1].size(); ++i){
      mini = std::min(mini, DP[n-1][i]);
    }
    
    if(mini >= MAX){
      std::cout << "Impossible!\n";
    }
    else{
      std::cout << mini << '\n';
    }
    
  }
  
  return 0;
}