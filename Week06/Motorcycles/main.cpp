#include <iostream>
#include <tuple>
#include <vector>

#include <CGAL/Gmpq.h>

typedef CGAL::Gmpq dbl;
typedef std::tuple<long, dbl, int> TPL;

bool compare(TPL a, TPL b){
  return std::get<0>(a) > std::get<0>(b);
}

int main(){
  std::ios_base::sync_with_stdio(false);
  
  int t;
  std::cin >> t;
  
  while(t--){
    int n;
    std::cin >> n;
    
    std::vector<TPL> v(n);
    
    // higher slope => motorbike goes even more uppwards
    // smaller slope => motorbike goes even more downwards
    for(int i = 0; i < n; ++i){
      double y0, x1, y1;
      std::cin >> y0 >> x1 >> y1;
      
      v[i] = TPL(y0, ((dbl)y1-(dbl)y0)/(dbl)x1, i);
    }
    
    std::sort(v.begin(), v.end(), compare);
    
    std::vector<bool> sol(n, true);
    
    // top down pass
    dbl smallest_slope = std::get<1>(v[0]);
    for(int i = 1; i < n; ++i){
      dbl slope = std::get<1>(v[i]);
      int ind = std::get<2>(v[i]);
      
      if(CGAL::abs(slope) <= CGAL::abs(smallest_slope)){
        smallest_slope = slope;
      } 
      else if(slope > smallest_slope){
        sol[ind] = false;
      }
    }
    
    smallest_slope = std::get<1>(v[n-1]);
    for(int i = n-2; i >=0; --i){
      int ind = std::get<2>(v[i]);
      dbl slope = std::get<1>(v[i]);
      
      if(CGAL::abs(slope) < CGAL::abs(smallest_slope) || 
      (CGAL::abs(slope) == CGAL::abs(smallest_slope) && slope > smallest_slope)){
        smallest_slope = slope;
      } 
      else if(slope < smallest_slope){
        sol[ind] = false;
      }
    }
    
    for(int i = 0; i < n; ++i)
      if(sol[i])
        std::cout << i << ' ';
    std::cout << '\n';
  }
  
  return 0;
}