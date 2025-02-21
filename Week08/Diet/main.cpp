#include <iostream>
#include <vector>

#include <CGAL/QP_models.h>
#include <CGAL/QP_functions.h>
#include <CGAL/Gmpz.h>

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
  
  int n,m;
  std::cin >> n >> m;
  
  while(n!=0 && m!=0){
    //std::cout << n << ' ' << m << '\n';
    std::vector<IT> mini(n);
    std::vector<IT> maxi(n);
    
    for(int i = 0 ; i < n; ++i){
      std::cin >> mini[i] >> maxi[i];
    }
    
    std::vector<std::vector<IT>> nutri_vals(m, std::vector<IT>(n));
    std::vector<IT> prices(m);
    
    for(int j = 0; j < m; ++j ){
      std::cin >> prices[j];
      
      for(int i = 0; i < n; ++i){
        std::cin >> nutri_vals[j][i];
      }
    }
    
    Program lp (CGAL::SMALLER, true, 0, false, 0); 
    
    for(int i = 0; i < n; ++i){
      for(int j = 0; j < m; ++j){
        lp.set_a(j, i, -nutri_vals[j][i]);
      }
      lp.set_b(i, -mini[i]);
    }
    
    for(int i = 0; i < n; ++i){
      for(int j = 0; j < m; ++j){
        lp.set_a(j, i+n, nutri_vals[j][i]);
      }
      lp.set_b(i+n, maxi[i]);
    }
    
    for(int j = 0; j < m; ++j){
      lp.set_c(j, prices[j]);
    }
    
    Solution s = CGAL::solve_linear_program(lp, ET());
    assert(s.solves_linear_program(lp));
    
    if(s.is_infeasible()){
      std::cout << "No such diet.\n";
    }
    else{
      std::cout << floor_to_double(s.objective_value()) << '\n';
    }
    
    std::cin >> n >> m;
  }
  
  return 0;
}