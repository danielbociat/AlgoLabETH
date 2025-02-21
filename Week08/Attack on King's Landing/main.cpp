#include <iostream>
#include <vector>

#include <boost/graph/graph_traits.hpp>
#include <boost/graph/adjacency_list.hpp>
#include <boost/graph/dijkstra_shortest_paths.hpp>
#include <boost/property_map/property_map.hpp>
#include <boost/graph/max_cardinality_matching.hpp>

#include <boost/graph/push_relabel_max_flow.hpp>

typedef boost::adjacency_list<boost::vecS, boost::vecS, boost::undirectedS,
  boost::no_property, boost::property<boost::edge_weight_t, int> >      wg_graph;
typedef boost::property_map<wg_graph, boost::edge_weight_t>::type weight_map;
typedef boost::graph_traits<wg_graph>::edge_descriptor            edge_desc;
typedef boost::graph_traits<wg_graph>::vertex_descriptor          vertex_desc;

typedef boost::adjacency_list<boost::vecS, boost::vecS, boost::undirectedS > my_graph;


int main(){
  std::ios_base::sync_with_stdio(false);
  
  int t;
  std::cin >> t;
  
  while(t--){
    int n, m, b, p, d;
    std::cin >> n >> m >> b >> p >> d;
    
    int a;
    std::vector<int> baracks(b);
    for(int i = 0; i < b; ++i){
      std::cin >> a;
      baracks[i] = a;
    }
    
    std::vector<bool> is_plaza(n, false);
    int q;
    for(int i = 0; i < p; ++i){
      std::cin >> q;
      is_plaza[q] = true;
    }
    
    wg_graph G(n);
    
    
    weight_map weights = boost::get(boost::edge_weight, G);
    edge_desc e;
    int x,y,l;
    std::vector<std::pair<int, int>> edges(m);
    for(int i = 0; i < m; ++i){
      std::cin >> x >> y >> l;
      e = boost::add_edge(x, y, G).first; weights[e]=l;
      edges[i] = {x,y};
    }
    
    std::vector<bool> is_reachable(n, false);
    for(int barack : baracks){
      std::vector<int> dist_map(n);
    
      boost::dijkstra_shortest_paths(G, barack,
        boost::distance_map(boost::make_iterator_property_map(
          dist_map.begin(), boost::get(boost::vertex_index, G))));
      
      //std::cout << "DIST\n";
      for(int i = 0; i < n; ++i){
        //std::cout << barack << ' ' << i << ' ' << dist_map[i] << '\n';
        if(dist_map[i] <= d){
          is_reachable[i] = true;
        }
      }
    }
    
    
    my_graph G2(2*n);
    for(auto edge : edges){
      int fr = edge.first;
      int sc = edge.second;
      //std::cout << fr << ' ' << sc << '\n';
      if(!(is_reachable[fr] && is_reachable[sc])) continue;
      
      if(is_plaza[fr]) std::swap(fr, sc);
      
      if(is_plaza[sc]){ 
        //std::cout << fr << ' ' << "PLZ" << sc << '\n';
        boost::add_edge(fr, sc+n, G2);
        
      }
      //std::cout << fr << ' ' << sc << '\n';
      boost::add_edge(fr, sc, G2);
    }
    
    std::vector<vertex_desc> mate_map(2*n);  // exterior property map
    boost::edmonds_maximum_cardinality_matching(G2,
    boost::make_iterator_property_map(mate_map.begin(), boost::get(boost::vertex_index, G2)));
    int matching_size = boost::matching_size(G2,
    boost::make_iterator_property_map(mate_map.begin(), boost::get(boost::vertex_index, G2)));
    
    std::cout << matching_size << '\n';
  }
  
  return 0;
}