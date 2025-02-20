#include <iostream>
#include <vector>

int main(){
  std::ios_base::sync_with_stdio(false);
  
  int t;
  std::cin >> t;
  
  while(t--){
    int n,m,k;
    std::cin >> n >> m >> k;
    
    std::vector <int> v(n, 0);
    for(int i = 0; i < n; ++i){
      std::cin >> v[i];
    }
    
    int left = 0, right = 0;
    int curr_sum = v[0];

    std::vector<int> pairs (n,-1);
    
    while(right < n && left < n){
      if(curr_sum > k){
        curr_sum -= v[left];
        left++;
        continue;
      }
      if(curr_sum < k){
        right++;
        curr_sum += v[right];
        continue;
      }
      if(curr_sum == k){
        pairs[right] = left;
        curr_sum -= v[left];
        left++;
        right++;
        curr_sum += v[right];
      }
    }
    
    std::vector<std::vector<int>> MAX (m+1, std::vector<int>(n+1, -1));
    std::vector<std::vector<int>> DP (m+1, std::vector<int>(n+1, -1));
    
    for (int i = 0; i < n; ++i){
      if(pairs[i] != -1){
        DP[1][i] = i - pairs[i] + 1;
      }
      MAX[1][i] = std::max(MAX[1][i-1], DP[1][i]);
    }
    
    for(int att = 2; att <= m; ++att){
      for (int i = 0; i < n; ++i){
        if(pairs[i] != -1 && MAX[att-1][pairs[i]-1] > 0){
          DP[att][i] = (i - pairs[i] + 1) + MAX[att-1][pairs[i]-1];
        } 
        else{
          DP[att][i] = -1;
        }
        MAX[att][i] = std::max(DP[att][i], MAX[att][i-1]);
      }  
    }

    if(MAX[m][n-1] == 0){
      std::cout << "fail";
    }
    else{
      std::cout << MAX[m][n-1];
    } 
    std::cout << '\n';
  }
  
  return 0;
}