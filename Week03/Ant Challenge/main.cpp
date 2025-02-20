
#include <iostream>
#include <vector>
#include <algorithm>

#include <boost/graph/dijkstra_shortest_paths.hpp>
#include <boost/graph/adjacency_list.hpp>
#include <boost/graph/prim_minimum_spanning_tree.hpp>

typedef boost::adjacency_list<boost::vecS, boost::vecS, boost::undirectedS,
                              boost::no_property,
                              boost::property<boost::edge_weight_t, int>
                              > weighted_graph;
typedef boost::property_map<weighted_graph, boost::edge_weight_t>::type weight_map;
typedef boost::graph_traits<weighted_graph>::edge_descriptor edge_desc;

std::vector<std::vector<std::vector<int>>> weights;
std::vector<std::vector<int>> best_weights;

void update_weights(std::vector<std::vector<int>> w, int start, int n){
  weighted_graph G(n);
  weight_map wg = boost::get(boost::edge_weight, G);
  
  for(int i = 0; i < n; ++i){
    for(int j = i+1; j < n; ++j){
      if(w[i][j] < 9999999){
        edge_desc e;
        e = boost::add_edge(i, j, G).first;
        wg[e] = w[i][j];
      }
    }
  }
  
  std::vector< boost::graph_traits< weighted_graph >::vertex_descriptor > p(num_vertices(G));
  prim_minimum_spanning_tree(G, &p[0], boost::root_vertex(start));
  
  for (std::size_t i = 0; i != p.size(); ++i){
    if (p[i] != i){
      best_weights[i][p[i]] = std::min(best_weights[i][p[i]], w[i][p[i]]);
    }
  }
}

int main(){
  std::ios_base::sync_with_stdio(false);
  
  int t;
  std::cin >> t;
  while(t--){
    int n, e, s, start, end;
    std::cin >> n >> e >> s >> start >> end;
    
    weights = std::vector<std::vector<std::vector<int>>>(
      s, std::vector<std::vector<int>>(
        n, std::vector<int>(n,9999999)));
    
    best_weights = std::vector<std::vector<int>>(n,std::vector<int>(n,9999999));
    
    for(int j = 0; j < e; ++j){
      int a,b;
      std::cin >> a >> b;
      
      for(int i = 0; i < s; ++i){
        int w;
        std::cin >> w;
        
        weights[i][a][b]=w;
        weights[i][b][a]=w;
      }
    }
   
    for(int i = 0; i < s; ++i){
      int h;
      std::cin >> h;
      
      update_weights(weights[i], h, n);
    }
    
    weighted_graph G(n);
    weight_map wg = boost::get(boost::edge_weight, G);
    
    for(int i = 0; i < n; ++i){
      for(int j = 0; j < n; ++j){
        if(best_weights[i][j] < 9999999){
          edge_desc e;
          e = boost::add_edge(i, j, G).first;
          wg[e] = best_weights[i][j];
        }
      }
    }
    
    std::vector<int> dist_map(boost::num_vertices(G));
      boost::dijkstra_shortest_paths(G, start,
      boost::distance_map(boost::make_iterator_property_map(dist_map.begin(),
      boost::get(boost::vertex_index, G))));
    
    std::cout << dist_map[end] << '\n';
  }
  
  return 0;
}