#include <iostream>
#include <vector>
#include <tuple>

#include <CGAL/QP_models.h>
#include <CGAL/QP_functions.h>
#include <CGAL/Gmpz.h>

#include <CGAL/Exact_predicates_inexact_constructions_kernel.h>
#include <CGAL/Delaunay_triangulation_2.h>
#include <CGAL/squared_distance_2.h>
#include <CGAL/Triangulation_face_base_with_info_2.h>
#include <CGAL/Triangulation_vertex_base_with_info_2.h>

typedef int IT;
typedef CGAL::Gmpz ET;
typedef CGAL::Quadratic_program<IT> Program;
typedef CGAL::Quadratic_program_solution<ET> Solution;
typedef CGAL::Exact_predicates_inexact_constructions_kernel K;
typedef CGAL::Triangulation_vertex_base_with_info_2<int, K> Vb;
typedef CGAL::Triangulation_face_base_with_info_2<int, K> Fb;
typedef CGAL::Triangulation_data_structure_2<Vb,Fb> Tds;
typedef CGAL::Delaunay_triangulation_2<K, Tds>  Triangulation;
typedef Triangulation::Edge_iterator  Edge_iterator;
typedef Tds::Vertex_handle VH;
typedef K::Point_2 P;
typedef std::pair<P, double> c_line;

long to_floor(const CGAL::Quotient<ET> &input_val)
{
  double num = std::floor(CGAL::to_double(input_val));
  while (num > input_val) num -= 1;
  while (num+1 <= input_val) num += 1;
  return (long)num;
}

int compute_idx(int curr_stadium, int curr_warehouse, int no_warehouses){
  return curr_stadium*no_warehouses + curr_warehouse;
}

int main(){
  std::ios_base::sync_with_stdio(false);
  
  int t;
  std::cin >> t;
  while(t--){
    int n, m, c;
    std::cin >> n >> m >> c;
    
    Program lp (CGAL::SMALLER, true, 0, false, 0); 
    
    int x, y, s, a;
    std::vector<P> poss(n+m);
    std::vector<int> alc_warehouse(n);
    int cc = 0;
    for(int i = 0; i < n; ++i){
      std::cin >> x >> y >> s >> a;  
      poss[i] = P(x,y);
      alc_warehouse[i] = a;
      
      for(int j = 0; j < m; ++j){
        lp.set_a(compute_idx(j,i,n), cc, 1);
      }
      lp.set_b(cc, s);
      cc++;
    }
    
    int d, u;
    for(int j = 0; j < m; ++j){
      std::cin >> x >> y >> d >> u;
      poss[n+j] = P(x,y);
      
      for(int i = 0; i < n; ++i){
        lp.set_a(compute_idx(j,i,n), cc, 1);
        lp.set_a(compute_idx(j,i,n), cc+1, -1);
        lp.set_a(compute_idx(j,i,n), cc+2, alc_warehouse[i]);
      }
      
      lp.set_b(cc, d);
      lp.set_b(cc+1, -d);
      lp.set_b(cc+2, 100*u);
      
      cc += 3;
    }
    
    std::vector<std::vector<int>> revs(n, std::vector<int>(m));
    for(int i = 0; i < n; ++i){
      for(int j = 0; j < m; ++j){
        std::cin >> revs[i][j];
        revs[i][j] *= 100;
      }
    }
    
    Triangulation dt;
    dt.insert(poss.begin(), poss.end());
    
    int r;
    std::vector<c_line> used_lines;
    for(int i = 0; i < c; ++i){
      std::cin >> x >> y >> r;
    
      P lnc = P(x,y);
      auto closest = dt.nearest_vertex(lnc);
      if(CGAL::squared_distance(closest->point(), lnc) <= r*r){
        used_lines.push_back({lnc, r*r});
      }
    }
    
    int no_used_lines = used_lines.size();
    std::vector<bool> used_by_stadium(no_used_lines);
    for(int j = 0; j < m; ++j){
      for(int k = 0; k < no_used_lines; ++k){
        used_by_stadium[k] = CGAL::squared_distance(used_lines[k].first, poss[n+j]) <= used_lines[k].second;
      }
      
      for(int i = 0; i < n; ++i){
        for(int k = 0; k < no_used_lines; ++k){
          bool used_by_warehouse = CGAL::squared_distance(used_lines[k].first, poss[i]) <= used_lines[k].second;
          
          if(used_by_warehouse != used_by_stadium[k]){
            revs[i][j]--;
          }
        }
      }
    }
    
    for(int i = 0; i < n; ++i){
      for(int j = 0; j < m; ++j){
        lp.set_c(compute_idx(j,i,n), -revs[i][j]);
      }
    }
    
    Solution sol = CGAL::solve_linear_program(lp, ET());
    
    if(sol.is_infeasible()){
      std::cout << "no\n";
    }
    else{
      std::cout << to_floor(-sol.objective_value() / 100) << '\n';
    }
  }
  
  return 0;
}