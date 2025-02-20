#include <iostream>
#include <vector>
#include <algorithm>

#include <boost/graph/kruskal_min_spanning_tree.hpp>
#include <boost/graph/dijkstra_shortest_paths.hpp>
#include <boost/graph/adjacency_list.hpp>

typedef boost::adjacency_list<boost::vecS, boost::vecS, boost::undirectedS,
                              boost::no_property,
                              boost::property<boost::edge_weight_t, int>
                              > weighted_graph;
typedef boost::property_map<weighted_graph, boost::edge_weight_t>::type weight_map;
typedef boost::graph_traits<weighted_graph>::edge_descriptor edge_desc;

void sol(const weighted_graph &G){
  std::vector<edge_desc> mst;
  
  boost::kruskal_minimum_spanning_tree(G, std::back_inserter(mst));
  
  int total_weight = 0;
  for (std::vector<edge_desc>::iterator it = mst.begin(); it != mst.end(); ++it) {
    total_weight += boost::get(boost::edge_weight, G, *it);
  }
  
  std::vector<int> dist_map(boost::num_vertices(G));
  boost::dijkstra_shortest_paths(G, 0,
    boost::distance_map(boost::make_iterator_property_map(dist_map.begin(),
    boost::get(boost::vertex_index, G))));
  
  std::cout << total_weight << ' ' << *std::max_element(dist_map.begin(), dist_map.end()) << '\n'; 
}

int main(){
  std::ios_base::sync_with_stdio(false);
  
  int t;
  std::cin >> t;
  
  while(t--){
    int n, m;
    std::cin >> n >> m;
    
    weighted_graph G(n);
    weight_map weights = boost::get(boost::edge_weight, G);
    
    for(int i = 0; i < m; i++){
      int a,b,w;
      std::cin >> a >> b >> w;
      
      edge_desc e;
      e = boost::add_edge(a, b, G).first;
      weights[e] = w;
    }
    
    sol(G);
  }
  
  return 0;
}