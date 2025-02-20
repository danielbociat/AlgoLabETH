#include <iostream>
#include <vector>
#include <limits.h>

int main(){
  std::ios_base::sync_with_stdio(false);
  
  int t;
  std::cin >> t;
  
  while(t--){
    int n, k;
    std::cin >> n >> k;
    
    std::vector<int> v(n, 0);
    
    for(int i = 0; i<n; ++i){
      std::cin >> v[i];
    }
    
    int left = 0, right = 0, bleft = 0, bright = 0;
    int curr_sum = v[0], bdiff = INT_MAX;
    
    while(left < n && right < n){
      if(abs(curr_sum - k) < bdiff){
        bdiff = abs(curr_sum - k);
        bleft = left;
        bright = right;
      }
      
      if(curr_sum > k){
        curr_sum -= v[left];
        left++;
      }
      if(curr_sum < k){
        right++;
        curr_sum += v[right];
      }
      if(curr_sum == k){
        bleft = left;
        bright = right;
        break;
      }
    }
    
    std::cout << bleft  << ' ' << bright << '\n';
    
  }
  
  return 0;
}