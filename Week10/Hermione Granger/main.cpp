///1
#include <iostream>
#include <vector>
#include <limits>

#include <CGAL/QP_models.h>
#include <CGAL/QP_functions.h>
#include <CGAL/Gmpz.h>
#include <CGAL/number_utils.h>

#include <CGAL/Exact_predicates_inexact_constructions_kernel.h>
#include <CGAL/Delaunay_triangulation_2.h>
#include <CGAL/squared_distance_2.h>
#include <CGAL/Triangulation_face_base_with_info_2.h>
#include <CGAL/Triangulation_vertex_base_with_info_2.h>

typedef CGAL::Exact_predicates_inexact_constructions_kernel K;

typedef CGAL::Triangulation_vertex_base_with_info_2<int, K> Vb;
typedef CGAL::Triangulation_face_base_with_info_2<int, K> Fb;
typedef CGAL::Triangulation_data_structure_2<Vb,Fb> Tds;
typedef CGAL::Delaunay_triangulation_2<K, Tds>  Triangulation;

typedef Triangulation::Edge_iterator  Edge_iterator;
typedef Tds::Vertex_handle VH;

typedef K::Point_2 P;
typedef std::tuple<int, int, int> info; 
typedef std::pair<P, int> P_info;

typedef long IT;
typedef CGAL::Gmpz ET;

typedef CGAL::Quadratic_program<IT> Program;
typedef CGAL::Quadratic_program_solution<ET> Solution;

int main(){
  std::ios_base::sync_with_stdio(false);
  
  int t; std::cin >> t;
  
  while(t--){
    int max_f,target_m,target_c,target_g;
    std::cin >> max_f >> target_m >> target_c >> target_g;
    
    int d,s;
    std::cin >> d >> s;
    
    std::vector<P_info> slytherins(s);
    std::vector<info> slytherins_info(s);
    int x,y,m,c,g, f;
    for(int i = 0; i < s; ++i){
      std::cin >> x >> y >> m >> c >> g;
      
      slytherins[i] = {P(x,y), i};
      slytherins_info[i] = {m, c, g};
    }
    
    Triangulation dt;
    dt.insert(slytherins.begin(), slytherins.end());
    
    std::vector<int> closest_slytherin(d);
    std::vector<int> visited_slytherin(s, -1);
    
    std::vector<int> DA_rates(d);
    std::vector<bool> usable_DA(d, true);
    for(int i = 0; i < d; ++i){
      std::cin >> x >> y >> f;
      
      DA_rates[i] = f;
      VH closest = dt.nearest_vertex(P(x,y));
      int inf = closest->info();
      
      if(visited_slytherin[inf] == -1){
        visited_slytherin[inf] = i;
      }
      else{
        int already_assigned = visited_slytherin[inf];
        if(f < DA_rates[already_assigned]){
          visited_slytherin[inf] = i;
          usable_DA[already_assigned] = false;
        }
        else{
          usable_DA[i] = false;
        }
      }
      closest_slytherin[i] = inf;
    }
    
    Program lp (CGAL::SMALLER, true, 0, true, 24); 
    
    for(int i = 0; i < d; ++i){
      if(usable_DA[i]){
        int val = std::get<0>(slytherins_info[closest_slytherin[i]]);
        lp.set_a(i, 0, -val);
      }
    }
    lp.set_b(0, -target_m);
    
    for(int i = 0; i < d; ++i){
      if(usable_DA[i]){
        int val = std::get<1>(slytherins_info[closest_slytherin[i]]);
        lp.set_a(i, 1, -val);
      }
    }
    lp.set_b(1, -target_c);
    
    for(int i = 0; i < d; ++i){
      if(usable_DA[i]){
        int val = std::get<2>(slytherins_info[closest_slytherin[i]]);
        lp.set_a(i, 2, -val);
      }
    }
    lp.set_b(2, -target_g);
    
    for(int i = 0; i < d; ++i){
      if(usable_DA[i]){
        lp.set_a(i, 3, DA_rates[i]);
      }
    }
    lp.set_b(3, max_f);
    
    Solution sol = CGAL::solve_linear_program(lp, ET());
    //std::cout << sol << '\n';
    if(sol.is_infeasible()) {
      std::cout << "H\n";
      continue;
    }
    std::cout << "L\n";
  }
  
  return 0;
}