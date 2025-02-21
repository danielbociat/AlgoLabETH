#include <iostream>
#include <vector>
#include <limits>

#include <CGAL/QP_models.h>
#include <CGAL/QP_functions.h>
#include <CGAL/Gmpz.h>
#include <CGAL/number_utils.h>

typedef std::pair<long, long> pos;

typedef long IT;
typedef CGAL::Gmpz ET;

typedef CGAL::Quadratic_program<IT> Program;
typedef CGAL::Quadratic_program_solution<ET> Solution;

double ceil_to_double(const CGAL::Quotient<ET> & x)
{
  double a = std::ceil(CGAL::to_double(x));
  while (a-1 >= x) a -= 1;
  while (a < x) a += 1;
  return a;
}

int main(){
  std::ios_base::sync_with_stdio(false);
  
  int t; 
  std::cin >> t;
  
  while(t--){
    long n, m, s; 
    std::cin >> n >> m >> s;
    
    int cc = 0;
    
    Program lp (CGAL::SMALLER, false, 0, false, 0); 
    const int A = 0;
    const int B = 1;
    const int C = 2;
    
    long x,y;
    long sum_x_nobles = 0;
    long sum_y_nobles = 0;
    
    std::vector<pos> noble_houses(n);
    for(int i = 0; i < n; ++i){
      std::cin >> x >> y;
      noble_houses[i] = {x,y};
      
      sum_x_nobles += x;
      sum_y_nobles += y;
      lp.set_a(A, cc, x); lp.set_a(B, cc, y); lp.set_a(C, cc, 1); lp.set_b(cc++, 0);
    }
    
    long sum_x_commons = 0;
    long sum_y_commons = 0;
    std::vector<pos> common_houses(m);
    for(int i = 0; i < m; ++i){
      std::cin >> x >> y;
      common_houses[i] = {x,y};
    
      sum_x_commons += x;
      sum_y_commons += y;
      lp.set_a(A, cc, -x); lp.set_a(B, cc, -y); lp.set_a(C, cc, -1); lp.set_b(cc++, 0);
    }
  
    lp.set_l(A, true, 1); 
    lp.set_u(A, true, 1);
    
    Solution sol = CGAL::solve_linear_program(lp, ET());
    
    if(sol.is_infeasible()) {
      std::cout << "Y\n";
      continue;
    }
    
    if(s != -1) {
      lp.set_a(B, cc, sum_y_commons - sum_y_nobles);
      lp.set_a(C, cc, m-n);
      lp.set_b(cc++, s + sum_x_nobles - sum_x_commons);
      
      Solution sol = CGAL::solve_linear_program(lp, ET());
    
      if(sol.is_infeasible()) {
        std::cout << "B\n";
        continue;
      }
    }
    
    const int A2 = 3;
    const int B2 = 4;
    const int C2 = 5;
    const int D2 = 6;
    
    lp.set_l(B2, true, 1); 
    lp.set_u(B2, true, 1);
    
    lp.set_l(D2, true, 0);
    
    for(int i = 0; i < n; ++i){
      lp.set_a(A2, cc, noble_houses[i].first); 
      lp.set_a(C2, cc, 1); 
      lp.set_a(D2, cc, -1);
      lp.set_b(cc++, -noble_houses[i].second);
      
      lp.set_a(A2, cc, -noble_houses[i].first); 
      lp.set_a(C2, cc, -1); 
      lp.set_a(D2, cc, -1);
      lp.set_b(cc++, noble_houses[i].second);
    }
    
    for(int i = 0; i < m; ++i){
      lp.set_a(A2, cc, common_houses[i].first); 
      lp.set_a(C2, cc, 1); 
      lp.set_a(D2, cc, -1);
      lp.set_b(cc++, -common_houses[i].second);
      
      lp.set_a(A2, cc, -common_houses[i].first); 
      lp.set_a(C2, cc, -1); 
      lp.set_a(D2, cc, -1);
      lp.set_b(cc++, common_houses[i].second);
    }
    
    // lines are perpendicular => B1 + A2 = 0
    lp.set_a(B, cc, 1); lp.set_a(A2, cc, 1); lp.set_b(cc++, 0);
    lp.set_a(B, cc, -1); lp.set_a(A2, cc, -1); lp.set_b(cc++, 0);     
    
    lp.set_c(D2, 1);
    
    sol = CGAL::solve_linear_program(lp, ET());
    
    std::cout << std::fixed;
    std::cout << (long)ceil_to_double(sol.objective_value()) << '\n';
  }
  
  return 0;
}