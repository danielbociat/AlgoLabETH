///1
#include <iostream>
#include <vector>
#include <climits>

// example: how to solve a simple explicit LP
#include <CGAL/QP_models.h>
#include <CGAL/QP_functions.h>
#include <CGAL/Gmpz.h>

// choose exact type for solver (CGAL::Gmpz or CGAL::Gmpq)
typedef CGAL::Gmpq ET;

// program and solution types
typedef CGAL::Quadratic_program<ET> Program;
typedef CGAL::Quadratic_program_solution<ET> Solution;

typedef std::pair<double, double> coord;

int main(){
  std::ios_base::sync_with_stdio(false);
  
  int t;
  std::cin >> t;
  std::cout << std::setprecision(0) << std::fixed;
  
  while(t--){
    int n, m, h, w;
    std::cin >> n >> m >> h >> w;
    
    Program lp(CGAL::SMALLER, true, 1, false, 0);
    
    std::vector<coord> free_nails(n);
    std::vector<coord> used_nails(m);
    
    int x,y;
    for(int i = 0; i < n; ++i){
      std::cin >> x >> y;
      
      free_nails[i] = {x,y};
      lp.set_c(i, -2 * (h+w));
    }
    
    for(int i = 0; i < m; ++i){
      std::cin >> x >> y;
      used_nails[i] = {x,y};
    }
    
    int cc = 0;
    for(int j = 0; j < n; ++j){
      ET min_lim = DBL_MAX;
      for(int i = 0; i < m; ++i){
        ET dx = std::abs(used_nails[i].first - free_nails[j].first);
        ET dy = std::abs(used_nails[i].second - free_nails[j].second);
        
        min_lim = std::min(min_lim, std::max(dx/w, dy/h));
      }
      lp.set_a(j, cc, 1);
      lp.set_b(cc++, 2 * min_lim - 1);
    }
    
    /// free nails
    for(int i = 0; i < n;++i){
      for(int j = i+1; j < n; ++j){
        ET dx = std::abs(free_nails[i].first - free_nails[j].first);
        ET dy = std::abs(free_nails[i].second - free_nails[j].second);
        
        lp.set_a(i, cc, 1);
        lp.set_a(j, cc, 1);
        lp.set_b(cc++, 2*std::max(dx/w, dy/h));
      }
    }
    
    Solution s = CGAL::solve_linear_program(lp, ET());
    std::cout << (long)std::ceil(CGAL::to_double(-s.objective_value())) << "\n";
  }
  
  
  return 0;
}