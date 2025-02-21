///4
#include <iostream>
#include <vector>
#include <algorithm>
#include <tuple>

typedef std::pair<int, int> edge;
typedef std::tuple<long, long, long> res;

bool cmp(res a, res b){
  return std::get<2>(a) * std::get<1>(b) < std::get<2>(b) * std::get<1>(a);
}

res solve(int node, std::vector<long> &g, std::vector <std::vector<edge>> &edges){
  
  if(edges[node].size() == 0){
    return {g[node], 1, 0};
  }
  
  std::vector<res> tmp_ress;
  
  for(auto e : edges[node]){
    auto tmp_res = solve(e.first, g, edges);
    
    long gold = std::get<0>(tmp_res);
    long nodes = std::get<1>(tmp_res);
    long times = std::get<2>(tmp_res);
    
    tmp_ress.push_back({gold - nodes*e.second, nodes, times + 2*e.second});
  }
  
  sort(tmp_ress.begin(), tmp_ress.end(), cmp);
  
  long total = g[node];
  long time_spent = 0;
  long num_nodes = 0;
  
  for(auto subtree : tmp_ress){
    total += std::get<0>(subtree) - time_spent * std::get<1>(subtree);
    num_nodes += std::get<1>(subtree);
    time_spent += std::get<2>(subtree);
  }
  
  return {total, num_nodes+1, time_spent};
}

int main(){
  std::ios_base::sync_with_stdio(false);
  
  int t;
  std::cin >> t;
  
  while(t--){
    int n;
    std::cin >> n;
    
    std::vector <long> g(n+1,0);
    for(int i = 0; i < n; ++i){
      std::cin >> g[i+1];
    }
    
    std::vector <std::vector<edge>> edges(n+1, std::vector<edge>());
    int u,v,l;
    for(int i = 0; i < n; ++i){
      std::cin >> u >> v >> l;
      edges[u].push_back({v,l});
    }
    
    auto sol = solve(0, g, edges);
    std::cout << std::get<0>(sol) << '\n';
  }
  
  return 0;
}