#include <iostream>
#include <vector>
#include <limits>

#include <boost/graph/adjacency_list.hpp>
#include <boost/graph/kruskal_min_spanning_tree.hpp>

typedef boost::adjacency_list<boost::vecS, boost::vecS, boost::undirectedS,
  boost::no_property, boost::property<boost::edge_weight_t, int> >      weighted_graph;
typedef boost::property_map<weighted_graph, boost::edge_weight_t>::type weight_map;
typedef boost::graph_traits<weighted_graph>::edge_descriptor            edge_desc;
typedef boost::graph_traits<weighted_graph>::vertex_descriptor          vertex_desc;

void dfs(int start,int curr, int max, std::vector<std::vector<int>> &max_dist, std::vector<bool> &visited, std::vector<std::vector<int>> &mst_mp,  std::vector<std::vector<int>> &costs) {
  max_dist[start][curr] = max_dist[curr][start] = max;
  visited[curr] = true;

  for (auto next : mst_mp[curr]) {
    if (!visited[next])
      dfs(start, next, std::max(max, costs[curr][next]), max_dist, visited, mst_mp, costs);
  }
}

int main(){
  std::ios_base::sync_with_stdio(false);
  
  int t;
  std::cin >> t;
  while(t--){
    int n, start;
    std::cin >> n >> start;
    
    weighted_graph G(n);
    weight_map weights = boost::get(boost::edge_weight, G);
    
    edge_desc e;
    int c;
    std::vector<std::vector<int>> costs(n+1, std::vector<int>(n+1));
    std::vector<std::vector<bool>> used(n+1, std::vector<bool>(n+1, false));
    for(int j = 0; j < n-1; ++j){
      for(int i = j+1; i < n; ++i){
        std::cin >> c;
        
        costs[i][j] = costs[j][i] = c;
        e = boost::add_edge(j, i, G).first; weights[e]=c;
      }
    }
  
    std::vector<edge_desc> mst;    // vector to store MST edges (not a property map!)
    boost::kruskal_minimum_spanning_tree(G, std::back_inserter(mst));
    
    std::vector<std::vector<int>> mst_mp(n, std::vector<int>());
    int optimal_cost = 0;
    for (auto edge : mst) {
      int i1 = boost::source(edge, G);
      int i2 = boost::target(edge, G);
      
      used[i1][i2] = used[i2][i1] = true;
      optimal_cost += costs[i1][i2];
      
      mst_mp[i1].push_back(i2);
      mst_mp[i2].push_back(i1);
    }
    
    std::vector<std::vector<int>> max_dist(n, std::vector<int>(n, -1));
    std::vector<bool> visited;
    for(int i= 0; i < n; ++i){
      visited = std::vector<bool>(n, false);
      dfs(i, i, -1, max_dist, visited, mst_mp, costs);
    }
    
    int best_diff = 99999999;
    for(int i = 0; i < n; ++i){
      for(int j = i+1; j < n; ++j){
        if(!used[i][j]){
          best_diff = std::min(best_diff, costs[i][j] - max_dist[i][j]);
        }
      }
    }
    
    std::cout << optimal_cost + best_diff << '\n';
  }
  
  return 0;
}