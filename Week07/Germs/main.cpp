#include <iostream>
#include <vector>
#include <limits>
#include <algorithm>

#include <CGAL/number_utils.h>
#include <CGAL/Exact_predicates_inexact_constructions_kernel.h>
#include <CGAL/Delaunay_triangulation_2.h>
#include <CGAL/squared_distance_2.h>

typedef CGAL::Exact_predicates_inexact_constructions_kernel K;
typedef CGAL::Delaunay_triangulation_2<K>  Triangulation;
typedef Triangulation::Edge_iterator  Edge_iterator;

typedef K::Point_2 P;

double ceil_to_double(const K::FT& x)
{
  double a = std::ceil(CGAL::to_double(x));
  while (a-1 >= x) a -= 1;
  while (a < x) a += 1;
  return a;
}

int main(){
  std::ios_base::sync_with_stdio(false);
  
  int n;
  std::cin >> n;
  
  while(n){
    double l, b, r, t;
    std::cin >> l >> b >> r >> t;
    
    std::vector<double> minis;
    minis.reserve(n);
    
    std::vector<P> points(n);
    double x, y;
    for(int i = 0; i < n; ++i){
      std::cin >> x >> y;
      points[i] = P(x,y);
    }
    
    Triangulation dt;
    dt.insert(points.begin(), points.end());
    
    for(auto v = dt.finite_vertices_begin(); v != dt.finite_vertices_end(); ++v){
      double vx = v->point().x();
      double vy = v->point().y();
      
      double mini_margin = std::min({CGAL::abs(r-vx), CGAL::abs(vx-l), CGAL::abs(t-vy), CGAL::abs(vy-b)}) - 0.5;
      double mini_cells = std::numeric_limits<double>::max();
      
      auto start = dt.incident_vertices(v);
      if(start != 0){
        auto circulator = start;
        do {
          if(!dt.is_infinite(circulator)){
            mini_cells = std::min(mini_cells, CGAL::squared_distance(v->point(), circulator->point()));
          }
        } while(++circulator != start);  
      }
      
      minis.push_back(std::min(mini_margin, (std::sqrt(mini_cells)-1)/2));
    }
    
    sort(minis.begin(), minis.end());

    std::cout << ceil_to_double(std::sqrt(minis[0])) << ' ' 
    << ceil_to_double(std::sqrt(minis[n/2])) << ' '
    << ceil_to_double(std::sqrt(minis[n-1])) << '\n';
    
    std::cin >> n;
  }
  
  return 0;
}