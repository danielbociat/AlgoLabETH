#include <iostream>
#include <vector>
#include <algorithm>

#include <boost/pending/disjoint_sets.hpp>
#include <boost/graph/connected_components.hpp>
#include <boost/graph/adjacency_list.hpp>

#include <CGAL/Exact_predicates_inexact_constructions_kernel.h>
#include <CGAL/Delaunay_triangulation_2.h>
#include <CGAL/squared_distance_2.h>
#include <CGAL/Triangulation_face_base_with_info_2.h>
#include <CGAL/Triangulation_vertex_base_with_info_2.h>

typedef boost::adjacency_list< boost::vecS, boost::vecS, boost::undirectedS > Graph;

typedef CGAL::Exact_predicates_inexact_constructions_kernel K;

typedef CGAL::Triangulation_vertex_base_with_info_2<int, K> Vb;
typedef CGAL::Triangulation_face_base_with_info_2<int, K> Fb;
typedef CGAL::Triangulation_data_structure_2<Vb,Fb> Tds;
typedef CGAL::Delaunay_triangulation_2<K, Tds>  Triangulation;

typedef K::Point_2 P;
typedef std::pair<P, int> P_info;

typedef std::tuple<int, int, double> Edge;

bool cmp_edge(const Edge e1, const Edge e2){
  return std::get<2>(e1) < std::get<2>(e2);
}

int main(){
  std::ios_base::sync_with_stdio(false);
  
  int t;
  std::cin >> t;
  
  while(t--){
    int n, m, k;
    double s;
    std::cin >> n >> m >> s >> k;
    
    int x, y;
    std::vector<P_info> trees(n);
    for(int i = 0 ; i < n ; ++i){
      std::cin >> x >> y;
      trees[i] = {P(x,y), i};
    }
    
    Triangulation dt;
    dt.insert(trees.begin(), trees.end());
    
    std::vector<P> bones(m);
    for(int i = 0 ; i < m ; ++i){
      std::cin >> x >> y;
      bones[i] = P(x,y);
    }
    
    Graph G(n);
    std::vector<Edge> edges;
    for (auto e = dt.finite_edges_begin(); e != dt.finite_edges_end(); ++e) {
      int i1 = e->first->vertex((e->second+1)%3)->info();
      int i2 = e->first->vertex((e->second+2)%3)->info();
      
      edges.push_back({i1, i2, dt.segment(e).squared_length()});
      
      if(dt.segment(e).squared_length() <= s ){
        add_edge(i1, i2, G);
      }
    }
    
    std::vector< int > component(num_vertices(G));
    connected_components(G, &component[0]);
    
    int no_edges = edges.size();
    std::vector<Edge> edges_with_bones(no_edges + m + 1);
    for(int i = 0 ; i < no_edges; ++i){
      edges_with_bones[i] = edges[i];
    }
    
    std::vector<int> bones_per_area(n, 0);
    std::vector<std::pair<int, double>> bone_info(m);
    for(int i = 0; i < m; ++i){
      auto v = dt.nearest_vertex(bones[i]);
      double dist = CGAL::squared_distance(bones[i], v->point());
      
      bone_info[i] = {v->info(), dist};
      
      if(4 * dist <= s) {
        bones_per_area[component[v->info()]] += 1;
      }
      edges_with_bones[i+no_edges] = {v->info(), i+n, 4 * dist};
    }
    
    int sol1 = *std::max_element(bones_per_area.begin(), bones_per_area.end());
    std::cout << sol1 << ' ';
    
    std::sort(edges_with_bones.begin(), edges_with_bones.end(), cmp_edge);
    
    double sol = 0;
    
    int left = 0, right = edges_with_bones.size()-1;
    
    if(sol1 >= k){
      while(std::get<2>(edges_with_bones[right]) > s) right--;
    }
    else{
      while(std::get<2>(edges_with_bones[left]) <= s) left++;
    }
    
    int mid = left + (right-left)/2;
    
    while(left <= right){
      Graph GG(n);
      std::vector<int> bones_per_area2(n+m, 0);
      double tg = std::get<2>(edges_with_bones[mid]);
      for(auto edge : edges){
        if(std::get<2>(edge) <= tg){
          add_edge(std::get<0>(edge), std::get<1>(edge), GG);
        }
      }
      
      std::vector< int > component2(num_vertices(GG));
      connected_components(GG, &component2[0]);
      
      int maxi = -1;
      
      for(int i = 0; i < m; ++i){
        if(4 * bone_info[i].second <= tg) {
          bones_per_area2[component2[bone_info[i].first]] += 1;
          maxi = std::max(maxi, bones_per_area2[component2[bone_info[i].first]]);
        }
      }
      
      if(maxi >= k){
        right = mid-1;
        sol = tg;
      }
      else{
        left = mid + 1;
      }
      mid = left + (right-left)/2;
    }
    
    std::cout << (long)sol << '\n';
  }
  
  return 0;  
}