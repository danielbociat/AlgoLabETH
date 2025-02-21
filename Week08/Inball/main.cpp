#include <iostream>
#include <vector>

#include <CGAL/QP_models.h>
#include <CGAL/QP_functions.h>
#include <CGAL/Gmpz.h>
#include <CGAL/number_utils.h>

typedef int IT;
typedef CGAL::Gmpz ET;

typedef CGAL::Quadratic_program<IT> Program;
typedef CGAL::Quadratic_program_solution<ET> Solution;

double floor_to_double(const CGAL::Quotient<ET> & x){
  double a = std::floor(CGAL::to_double(x));
  
  while (a > x) a -= 1;
  while (a+1 <= x) a += 1;
  
  return a;
}

int main(){
  std::ios_base::sync_with_stdio(false);
  
  int n;
  std::cin >> n;
  
  while(n){
    int d; 
    std::cin >> d;
    
    std::vector<std::vector<int>> a(n, std::vector<int>(d));
    std::vector<int> b(n);
    std::vector<int> norms(n);
    
    for(int i = 0; i < n; ++i){
      int curr = 0;
      for(int j = 0; j < d; ++j){
        std::cin >> a[i][j];
        curr += a[i][j] * a[i][j];
      }
      
      norms[i] = (int)std::sqrt(curr);
      
      std::cin >> b[i];
    }
    
    Program lp (CGAL::SMALLER, false, 0, false, 0); 
    
    int R = d;
    
    for(int i = 0; i < n; ++i){
      for(int j = 0; j < d; ++j){
        lp.set_a(j, i, a[i][j]);
        std::cout << 'x' << j << " * " << a[i][j] << " + ";
      }
      std::cout << 'r' << " * " << norms[i] << " <= ";
      lp.set_a(R, i, norms[i]);
      
      std::cout << b[i] << "\n";
      lp.set_b(i, b[i]);
    }
    
    lp.set_l(R, true, 0);
    lp.set_c(R, -1);
    
    Solution s = CGAL::solve_linear_program(lp, ET());
    assert(s.solves_linear_program(lp));
    
    if(s.is_infeasible()){
      std::cout << "none\n";
    }
    else if(s.is_unbounded()){
      std::cout << "inf\n";
    }
    else{
      std::cout << floor_to_double(-s.objective_value()) << '\n';
    }
    
    std::cin >> n;
  }
  
  
  return 0;
}