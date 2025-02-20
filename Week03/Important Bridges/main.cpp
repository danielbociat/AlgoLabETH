#include <iostream>
#include <vector>
#include <algorithm>

#include <boost/graph/biconnected_components.hpp>
#include <boost/graph/adjacency_list.hpp>

struct edge_component_t
{
    typedef boost::edge_property_tag kind;
} edge_component;

typedef boost::adjacency_list< boost::vecS, boost::vecS, boost::undirectedS, boost::no_property,
        boost::property< edge_component_t, std::size_t > >
        graph_t;
typedef boost::graph_traits< graph_t >::vertex_descriptor vertex_t;


int main(){
  std::ios_base::sync_with_stdio(false);
  
  int c;
  std::cin >> c;
  
  while(c--){
    int n, m;
    std::cin >> n >> m;
    
    graph_t G(n);
    
    for(int i = 0; i < m; ++i){
      int a,b;
      std::cin >> a >> b;
      
      boost::add_edge(a,b,G);
    }
    
    boost::property_map< graph_t, edge_component_t >::type component
        = get(edge_component, G);

    std::size_t num_comps = boost::biconnected_components(G, component);
    
    std::vector<int> edges_per_components(num_comps,0);
    
    boost::graph_traits< graph_t >::edge_iterator ei, ei_end;
    
    for (boost::tie(ei, ei_end) = boost::edges(G); ei != ei_end; ++ei){
      edges_per_components[component[*ei]]++;
    }
    
    int sol = 0;
 
    std::vector <std::pair<int, int>> sol_pairs;
    
    for (boost::tie(ei, ei_end) = boost::edges(G); ei != ei_end; ++ei){
      if(edges_per_components[component[*ei]] == 1){
        sol++;
        sol_pairs.push_back({std::min(source(*ei, G),target(*ei, G)),std::max(source(*ei, G),target(*ei, G))});
      }
    }
    
    std::cout << sol << '\n';
    sort(sol_pairs.begin(), sol_pairs.end());
    
    for(auto el : sol_pairs){
      std::cout << el.first << ' ' << el.second << '\n';
    }
  }
  
  return 0;
}