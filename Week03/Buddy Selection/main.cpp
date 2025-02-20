#include <iostream>
#include <vector>
#include <string>
#include <algorithm>

#include <boost/graph/adjacency_list.hpp>
#include <boost/graph/max_cardinality_matching.hpp>

typedef boost::adjacency_list<boost::vecS, boost::vecS, boost::undirectedS > my_graph;

int main(){
  std::ios_base::sync_with_stdio(false);
  
  int t;
  std::cin >> t;
  
  while(t--){
    int n, c, f;
    std::cin >> n >> c >> f;
    
    std::vector<std::vector<std::string>> traits(n, std::vector<std::string>());
    
    for(int i = 0; i < n; ++i){
      for(int j = 0; j < c; ++j){
        std::string trait;
        
        std::cin >> trait;
        traits[i].push_back(trait);
      }
      sort(traits[i].begin(), traits[i].end());
    }
    
    my_graph G(n);
    
    for(int i = 0; i < n; ++i){
      for(int j = i+1; j < n; ++j){
        std::vector<std::string> common;
        std::set_intersection(traits[i].begin(),traits[i].end(),
                          traits[j].begin(),traits[j].end(),
                          std::back_inserter(common));
        
        if((int)common.size() > f){
          boost::add_edge(i, j, G);
        }                  
      }
    }
    
    std::vector<boost::graph_traits< my_graph >::vertex_descriptor > mate(n);
    checked_edmonds_maximum_cardinality_matching(G, &mate[0]);
    
    if((int)boost::matching_size(G, &mate[0]) * 2 == n){
      std::cout << "not optimal";
    }
    else{
      std::cout << "optimal";
    }
    
    std::cout << '\n';
  }
  
  return 0;
}