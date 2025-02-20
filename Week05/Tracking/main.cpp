#include <iostream>
#include <vector>

#include <boost/graph/dijkstra_shortest_paths.hpp>
#include <boost/graph/adjacency_list.hpp>

typedef boost::adjacency_list<boost::vecS, boost::vecS, boost::undirectedS,
                              boost::no_property,
                              boost::property<boost::edge_weight_t, int>
                              > weighted_graph;
typedef boost::graph_traits<weighted_graph>::edge_descriptor edge_desc;

int main(){
  std::ios_base::sync_with_stdio(false);
  
  int t; 
  std::cin >> t;
  
  while(t--){
    int n,m,k,x,y;
    std::cin >> n >> m >> k >> x >> y;
    
    weighted_graph G;
    
    for(int i = 0; i < m; ++i){
      int a,b,c,d;
      std::cin >> a >> b >> c >> d;
      
      boost::add_edge(a, b, c, G);
      
      for(int j = 1; j <=k; ++j){
        if(d == 0){
          boost::add_edge(a + j*n, b + j*n, c, G);
        }
        else{
          boost::add_edge(b + j*n, a + (j-d)*n, c, G);
          boost::add_edge(a + j*n, b + (j-d)*n, c, G);
        }
      }
    }
    
    std::vector<int> dist_map(boost::num_vertices(G));
    boost::dijkstra_shortest_paths(G, x + k*n,
      boost::distance_map(boost::make_iterator_property_map(dist_map.begin(),
      boost::get(boost::vertex_index, G))));
      
    std::cout << dist_map[y] << '\n';
  }
  
  return 0;
}