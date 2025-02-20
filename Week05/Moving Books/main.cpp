#include <iostream>
#include <vector>
#include <algorithm>

void print_vector(std::vector<int> &a){
  for(int el : a){
    std::cout << el << ' ';
  }
  std::cout << '\n';
}

bool canBeSolvedInR(std::vector<int>&can_lift, int r, int to_be_lifted){
  int curr_lifted = 0;
  for(auto el: can_lift){
    curr_lifted += std::min(r, el - curr_lifted);
  }
  
  return curr_lifted == to_be_lifted;
}

int main(){
  std::ios_base::sync_with_stdio(false);
  
  int t;
  std::cin >> t;
  
  while(t--){
    int n, m;
    std::cin >> n >> m;
    
    std::vector <int> s(n);
    for(int i = 0; i < n; ++i){
      std::cin >> s[i];
    }
    
    sort(s.begin(), s.end());
    
    std::vector <int> w(m);
    for(int i = 0; i < m; ++i){
      std::cin >> w[i];
    }
    
    sort(w.begin(), w.end());
    
    if(w[m-1] > s[n-1]){
      std::cout << "impossible\n";
      continue;
    }
    
    int i = 0, j = 0;
    std::vector<int> can_lift(n, 0);
    while(j < m){
      if(w[j] <= s[i]){
        can_lift[i]++;
        j++;
      }
      else{
        i++;
        can_lift[i] = can_lift[i-1];
      }
    }
    i++;
    while(i < n){
      can_lift[i] = can_lift[i-1];
      i++;
    }
    
    int best_sol = m;
    int l=1, r = m;
    int mid = l + (r-l)/2;
    while(l <= r){
      if(canBeSolvedInR(can_lift, mid, m)){
        best_sol = std::min(best_sol, mid);
        
        r = mid-1;
        mid = l + (r-l)/2;
      }
      else{
        l = mid+1;
        mid = l + (r-l)/2;
      }
    }
    
    std::cout << 3*best_sol - 1 << '\n';
  }
  
  return 0;
}