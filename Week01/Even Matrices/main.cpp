#include <iostream>
#include <vector>

int countMismatches(std::vector <int> a, std::vector <int> b){
  int n = a.size();
  
  int mismatches = 0;
  
  for(int i = 0; i < n; ++i){
    mismatches += (a[i] != b[i]);
  }
  
  return mismatches;
}

int main(){
  std::ios_base::sync_with_stdio(false);
  
  int t;
  std::cin >> t;
  
  while(t--){
    int n;
    std::cin >> n;
    
    std::vector<std::vector<int>> mat(n, std::vector<int>(n));
    
    for(int i = 0; i < n; ++i){
      for(int j = 0; j < n; ++j){
        std::cin >> mat[i][j];
      }
    }
    
    for(int i = 0; i < n; ++i){
      for(int j = 1; j < n; ++j){
          mat[i][j] += mat[i][j-1];
          mat[i][j] = mat[i][j] % 2;
      }
    }
    for(int i = 1; i < n; ++i){
      for(int j = 0; j < n; ++j){
          mat[i][j] += mat[i-1][j];
          mat[i][j] = mat[i][j] % 2;
      }
    }
    mat.push_back(std::vector<int>(n, 0));
    
    long long sol = 0;
    for(int i = 0; i < n; ++i){
      for(int j = i+1; j <= n; ++j){
        int diffParity = countMismatches(mat[i], mat[j]);
        int sameParity = n + 1 - diffParity;
        
        sol += diffParity * (diffParity-1) / 2;
        sol += sameParity * (sameParity-1) / 2;
      }
    }
    
    std::cout << sol << '\n';
    
    /**
    //Print
    for(int i = 0; i < n; ++i){
      for(int j = 0; j < n; ++j){
          std::cout << mat[i][j] << ' ';
      }
      std::cout << '\n';
    }**/
  }
  
  return 0;
}