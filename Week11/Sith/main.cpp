///1
#include <iostream>
#include <vector>
#include <queue>

#include <CGAL/Exact_predicates_inexact_constructions_kernel.h>
#include <CGAL/Delaunay_triangulation_2.h>
#include <CGAL/Triangulation_vertex_base_with_info_2.h>

typedef CGAL::Exact_predicates_inexact_constructions_kernel K;
typedef K::Point_2 P;

typedef CGAL::Triangulation_vertex_base_with_info_2<bool, K> Vb;
typedef CGAL::Triangulation_face_base_2<K> Fb;
typedef CGAL::Triangulation_data_structure_2<Vb, Fb> Tds;
typedef CGAL::Delaunay_triangulation_2<K, Tds> Triangulation;
typedef Triangulation::Vertex_handle vh;

int solve(std::vector<P> &p, int ignored, double max_r){
  Triangulation dt;
  dt.insert(p.begin() + ignored, p.end());
    
  for(auto v = dt.finite_vertices_begin(); v != dt.finite_vertices_end(); ++v){
    v->info() = false;
  }
  
  int maxi = 0;
  for(auto v = dt.finite_vertices_begin(); v != dt.finite_vertices_end(); ++v){
    if(v->info()) continue;
    
    int curr_size = 0;
    std::queue<vh> q;
    q.push(v);
    
    v->info() = true;
    
    while(!q.empty()){
      auto curr_v = q.front();
      q.pop();
      curr_size++;
      
      Triangulation::Vertex_circulator c = dt.incident_vertices(curr_v);
      do {
        if (!dt.is_infinite(c) && !c->info() && CGAL::squared_distance(curr_v->point(), c->point()) <= max_r){
          c->info() = true;
          q.push(c);
        }
      } while (++c != dt.incident_vertices(curr_v));
    }
    
    maxi = std::max(curr_size, maxi);
  }
  
  return maxi;
}

int main(){
  std::ios_base::sync_with_stdio(false);
  
  int t;
  std::cin >> t;
  while(t--){
    int n;
    double r;
    std::cin >> n >> r;
    
    int x,y;
    std::vector<P> p(n);
    for(int i = 0; i < n; ++i){
      std::cin >> x >> y;
      p[i] = P(x,y);
    }
  
    int best = 0;
    int left = 1, right = n/2;
    int mid = (right-left)/2 + left;
    
    double r_sq = r*r;
    
    while(left <= right){
      int max_res = solve(p, mid, r_sq);
      
      if(mid < max_res){
        best = std::max(best,mid);
        left = mid+1;
      }
      else{
        best = std::max(best,max_res);
        right = mid-1;
      }
      
      mid = (right-left)/2 + left;
    }
     
    std::cout << best << '\n';
  }
  
  return 0;
}