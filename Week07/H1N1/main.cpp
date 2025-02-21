#include <iostream>
#include <queue>
#include <limits>

#include <CGAL/Exact_predicates_inexact_constructions_kernel.h>
#include <CGAL/Triangulation_face_base_with_info_2.h>
#include <CGAL/Delaunay_triangulation_2.h>
#include <CGAL/squared_distance_2.h>
#include <CGAL/Gmpq.h>
#include <CGAL/utils.h>

typedef CGAL::Exact_predicates_inexact_constructions_kernel K;

typedef CGAL::Gmpq dbl;

typedef CGAL::Triangulation_vertex_base_2<K> Vb;
typedef CGAL::Triangulation_face_base_with_info_2<dbl,K> Fb;
typedef CGAL::Triangulation_data_structure_2<Vb,Fb> Tds;
typedef CGAL::Delaunay_triangulation_2<K,Tds> Triangulation;

typedef K::Point_2 P;

typedef std::pair<dbl, Triangulation::Face_handle> pr;

void precompute(Triangulation &t){
  std::priority_queue <pr> q;
  
  for (auto f = t.all_faces_begin(); f != t.all_faces_end(); ++f) {
    if(t.is_infinite(f)){
      q.push({std::numeric_limits<long>::max(), f});
    }
    f->info() = 0;
  }
  
  while(!q.empty()){
    auto top = q.top();
    q.pop();
    
    auto largest_poss_escapee = top.first;
    auto curr_face = top.second;
    
    if(largest_poss_escapee <= curr_face->info()){
      continue;
    }
    curr_face->info() = largest_poss_escapee;
    
    for(int i = 0; i < 3; ++i){
      auto neighbor = curr_face->neighbor(i);
      
      if(t.is_infinite(neighbor)){
        continue;
      }
      
      auto v1 = curr_face->vertex((i+1)%3)->point();
      auto v2 = curr_face->vertex((i+2)%3)->point();
      dbl sq_edge_dist = CGAL::squared_distance(v1, v2);
      
      if(neighbor->info() >= sq_edge_dist) {
        continue;
      }
      
      q.push({std::min(sq_edge_dist, largest_poss_escapee), neighbor});
    }
  }
}

int main(){
  std::ios_base::sync_with_stdio(false);
  
  int n;
  std::cin >> n;
  
  while(n){
    std::vector<P> pts;
    pts.reserve(n);
    
    int x,y;
    for(int i = 0; i < n; ++i){
      std::cin >> x >> y;
      pts.push_back(P(x,y));
    }
    
    Triangulation t;
    t.insert(pts.begin(), pts.end());
    
    precompute(t);
    
    int m;
    std::cin >> m;
    
    dbl d;
    for(int i = 0; i < m; ++i){
      std::cin >> x >> y >> d;
      
      P person = P(x,y);
      P nearest_vertex = t.nearest_vertex(person)->point();
      
      if(d > CGAL::squared_distance(person, nearest_vertex)){
        std::cout << 'n';
      }
      else{
        auto f = t.locate(person);
        
        if(4 * d <= f->info()){
          std::cout << 'y';
        }
        else{
          std::cout << 'n';
        }
      }
    }
    
    std::cout << '\n';
    
    std::cin >> n;
  }
  
  return 0;
}