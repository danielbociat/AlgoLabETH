#include <iostream>
#include <vector>
#include <algorithm>
#include <tuple>
#include <set>
#include <limits>

#include <CGAL/Exact_predicates_exact_constructions_kernel.h>
#include <CGAL/Delaunay_triangulation_2.h>
#include <CGAL/Triangulation_vertex_base_with_info_2.h>
#include <CGAL/Triangulation_face_base_2.h>
#include <CGAL/Object.h>
#include <CGAL/squared_distance_2.h>

#include <boost/pending/disjoint_sets.hpp>

typedef CGAL::Exact_predicates_exact_constructions_kernel K;
typedef CGAL::Triangulation_vertex_base_with_info_2<int,K>   Vb;
typedef CGAL::Triangulation_face_base_2<K>                     Fb;
typedef CGAL::Triangulation_data_structure_2<Vb,Fb>            Tds;
typedef CGAL::Delaunay_triangulation_2<K,Tds>                  Triangulation;
typedef K::Point_2 P;
typedef std::pair<P, int> P_info;
typedef K::FT dbl;
typedef std::tuple <int, int, dbl> edge;

bool comp(edge a, edge b){
  return std::get<2>(a) < std::get<2>(b);
}

int main(){
  std::ios_base::sync_with_stdio(false);
  
  int t;
  std::cin >> t;
  
  while(t--){
    int n, h;
    dbl s;
    std::cin >> n >> s >> h;
    
    int x,y;
    std::vector <P_info> p(n);
    for(int i = 0; i < n; ++i){
      std::cin >> x >> y;
      p[i] = {P(x,y),i};
    }
    
    Triangulation dt;
    dt.insert(p.begin(), p.end());
    
    std::vector <edge> edges;
    for(auto curr_edge = dt.finite_edges_begin(); curr_edge != dt.finite_edges_end(); ++curr_edge){
      int i1 = curr_edge->first->vertex((curr_edge->second+1)%3)->info();
      int i2 = curr_edge->first->vertex((curr_edge->second+2)%3)->info();
      
      if(i1 > i2) std::swap(i1, i2);
      
      edges.push_back({i1, i2, dt.segment(curr_edge).squared_length()});
    }
    
    std::sort(edges.begin(), edges.end(), comp);
    
    int a2 = 0, a3 = 0, as = 0, amax = 0;
    
    dbl d2 = std::get<2>(edges[0]);
    for(auto curr_edge : edges){
      if(std::get<2>(curr_edge) == d2) a2++;
      else break;
    }
    
    dbl d3 = std::numeric_limits<long>::max();
    for(auto curr_face = dt.finite_faces_begin(); curr_face != dt.finite_faces_end(); ++curr_face){
      P p = dt.dual(curr_face);
      dbl dist_to_closest = squared_distance(p, dt.nearest_vertex(p)->point());
      
      if(dist_to_closest == d3){
        a3++;
      }
      else if(dist_to_closest < d3){
        d3 = dist_to_closest;
        a3 = 1;
      }
    }
     
    std::vector<std::pair<dbl,int>> bounds;
    for (auto curr_edge = dt.finite_edges_begin(); curr_edge != dt.finite_edges_end(); ++curr_edge){
      auto edg = dt.segment(curr_edge); 
      
      dbl len = edg.squared_length();
      auto dual = dt.dual(curr_edge);
      auto p1 = curr_edge->first->vertex((curr_edge->second+1)%3)->point();
      auto p2 = curr_edge->first->vertex((curr_edge->second+2)%3)->point();
      
      if (const K::Segment_2* segment = CGAL::object_cast<K::Segment_2>(&dual)){
        auto src = segment->source();
        auto tg = segment->target();
        
        dbl len1 = squared_distance(p1, src);
        dbl len2 = squared_distance(p1, tg);
        
        dbl mini = std::min(len1, len2);
        dbl maxi = std::max(len1, len2);
        
        if(CGAL::do_intersect(edg, *segment)){
          bounds.push_back({len/4, 0});
          bounds.push_back({maxi, 1});
          
          if(maxi >= s && len <= 4*s){
            as++;
          }
        }
        else{
          bounds.push_back({mini, 0});
          bounds.push_back({maxi, 1});
          
          if(maxi >= s && len <= 4*s && mini <= s){
            as++;
          }
        }
        
      }else if(const K::Ray_2* ray = CGAL::object_cast<K::Ray_2>(&dual)){
        dbl len_r = squared_distance(p2, ray->source());
        
        if(CGAL::do_intersect(edg, *ray)){
          bounds.push_back({len/4, 0});
          
          if(len <= 4*s){
            as++;
          }
        }
        else{
          bounds.push_back({len_r, 0});
          
          if(len <= 4*s && len_r <= s){
            as++;
          }
        }
      }
    }
    
    std::sort(bounds.begin(), bounds.end());
    int counter = 0;
    for (auto bound : bounds){
      if (bound.second==0){
        counter++;
        amax = std::max(counter, amax);
      }else{
        counter--;
      }
    }
     
    std::cout << a2 << ' ';
    std::cout << a3 << ' ';
    std::cout << as << ' ';
    std::cout << amax << '\n';
  }
  
  return 0;
}