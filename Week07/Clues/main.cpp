#include <iostream>
#include <vector>
#include <queue>
#include <unordered_map>

#include <boost/graph/adjacency_list.hpp>
#include <boost/graph/bipartite.hpp>
#include <boost/graph/connected_components.hpp>

#include <CGAL/Exact_predicates_inexact_constructions_kernel.h>
#include <CGAL/Delaunay_triangulation_2.h>
#include <CGAL/squared_distance_2.h>
#include <CGAL/Triangulation_face_base_with_info_2.h>
#include <CGAL/Triangulation_vertex_base_with_info_2.h>

typedef boost::adjacency_list<boost::vecS, boost::vecS, boost::undirectedS> graph;
typedef boost::graph_traits<graph>::vertex_descriptor vertex_desc;
typedef boost::graph_traits<graph>::edge_iterator edge_it; 

typedef CGAL::Exact_predicates_inexact_constructions_kernel K;

typedef CGAL::Triangulation_vertex_base_with_info_2<int, K> Vb;
typedef CGAL::Triangulation_face_base_with_info_2<int, K> Fb;
typedef CGAL::Triangulation_data_structure_2<Vb,Fb> Tds;
typedef CGAL::Delaunay_triangulation_2<K, Tds>  Triangulation;

typedef Triangulation::Edge_iterator  Edge_iterator;
typedef Tds::Vertex_handle VH;

typedef K::Point_2 P;
typedef std::pair<P, int> P_info;

void build_graph(VH v, graph &G, Triangulation &dt, double sq_radius, std::vector<int> &visited){
  std::queue<VH> q;
  q.push(v);
  visited[v->info()] = v->info();  
  
  while(!q.empty()) {
    auto curr = q.front();
    q.pop();
    
    auto start = dt.incident_vertices(curr);
    if(start != 0){
      auto neigh = start;
      do{
        if(dt.is_infinite(neigh) || visited[neigh->info()] == v->info()) continue;
        
        if(CGAL::squared_distance(v->point(), neigh->point()) <= sq_radius){
          q.push(neigh);
          visited[neigh->info()] = v->info();
          
          if(neigh->info() < v->info())
            boost::add_edge(neigh->info(), v->info(), G);
        } 
      } while(++neigh != start);
    }
  }
}

int main(){
  std::ios_base::sync_with_stdio(false);
  
  int t;
  std::cin >> t; 
  
  while(t--){
    int n, m;
    double r;
    
    std::cin >> n >> m >> r;
    
    std::vector<P_info> stations(n);
    int x, y;
    for(int i = 0; i < n; ++i){
      std::cin >> x >> y;
      stations[i] = {P(x,y), i};
    }
    
    Triangulation dt;
    dt.insert(stations.begin(), stations.end());
    
    graph G(n); 
    std::vector<int> visited (n, -1);
    for(auto vertex = dt.finite_vertices_begin(); vertex != dt.finite_vertices_end(); ++vertex){
      build_graph(vertex, G, dt, r*r, visited);
    }
    
    bool bipartite = is_bipartite(G);
    
    std::vector<int> component(n);
    if(bipartite){
      connected_components(G, &component[0]);
    }
    
    int a, b;
    for(int i = 0; i < m; ++i){
      std::cin >> x >> y >> a >> b;
      
      if(!bipartite){
        std::cout << 'n';
        continue;
      }
      
      P Holmes = P(x, y);
      P Watson = P(a, b);
      
      if(CGAL::squared_distance(Holmes, Watson) <= r*r){
        std::cout << 'y';
        continue;
      }
      
      auto closest_to_Holmes = dt.nearest_vertex(Holmes);
      auto closest_to_Watson = dt.nearest_vertex(Watson);
      
      if (CGAL::squared_distance(Holmes, closest_to_Holmes->point()) > r*r 
      || CGAL::squared_distance(Watson, closest_to_Watson->point()) > r*r ) {
          std::cout << "n";
          continue;
      }
      
      if(component[closest_to_Holmes->info()] == component[closest_to_Watson->info()]){
        std::cout << 'y';
      }
      else{
        std::cout << 'n';
      }
    }
    
    std::cout << '\n';
  }
  
  return 0;
}