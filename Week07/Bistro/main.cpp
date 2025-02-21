#include <iostream>

#include <CGAL/Exact_predicates_inexact_constructions_kernel.h>
#include <CGAL/Delaunay_triangulation_2.h>
#include <CGAL/squared_distance_2.h>

typedef CGAL::Exact_predicates_inexact_constructions_kernel K;
typedef CGAL::Delaunay_triangulation_2<K>  Triangulation;
typedef Triangulation::Edge_iterator  Edge_iterator;
typedef K::Point_2 P;

int main(){
  std::ios_base::sync_with_stdio(false);
  
  int n;
  std::cin >> n;
  
  while(n){
    int x,y;
    std::vector<P> pts;
    pts.reserve(n);
    for(int i = 0; i < n; ++i){
      std::cin >> x >> y;
      pts.push_back(P(x, y));
    }
    
    Triangulation t;
    t.insert(pts.begin(), pts.end());
    
    int m;
    std::cin >> m;
    std::vector<std::pair<int, int>> locs(m);
    
    for(int i = 0; i < m; ++i){
      std::cin >> x >> y;
      P src = P(x,y);
      P tg = t.nearest_vertex(src)->point();
      std::cout << (long) CGAL::squared_distance(src, tg) << '\n'; 
    }

    std::cin >> n;
  }
  
  return 0;
}