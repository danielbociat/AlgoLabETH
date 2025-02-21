///1
#include <iostream>
#include <queue>
#include <vector>

#include <boost/graph/adjacency_list.hpp>
#include <boost/graph/push_relabel_max_flow.hpp>
#include <boost/tuple/tuple.hpp>

typedef  boost::adjacency_list_traits<boost::vecS, boost::vecS, boost::directedS> traits;
typedef boost::adjacency_list<boost::vecS, boost::vecS, boost::directedS, boost::no_property,
  boost::property<boost::edge_capacity_t, long,
    boost::property<boost::edge_residual_capacity_t, long,
      boost::property<boost::edge_reverse_t, traits::edge_descriptor> > > >  graph;

typedef  boost::graph_traits<graph>::edge_descriptor      edge_desc;
typedef  boost::graph_traits<graph>::out_edge_iterator      out_edge_it;

class edge_adder {
 graph &G;

 public:
  explicit edge_adder(graph &G) : G(G) {}

  void add_edge(int from, int to, long capacity) {
    auto c_map = boost::get(boost::edge_capacity, G);
    auto r_map = boost::get(boost::edge_reverse, G);
    const edge_desc e = boost::add_edge(from, to, G).first;
    const edge_desc rev_e = boost::add_edge(to, from, G).first;
    c_map[e] = capacity;
    c_map[rev_e] = 0; // reverse edge has no capacity!
    r_map[e] = rev_e;
    r_map[rev_e] = e;
  }
};

int main(){
  std::ios_base::sync_with_stdio(false);
  
  int t;
  std::cin >> t;
  while(t--){
    int n, m;
    std::cin >> n >> m;
    
    graph G(n);
    edge_adder adder(G);
    auto rc_map = boost::get(boost::edge_residual_capacity, G);
    auto source = boost::add_vertex(G);
    auto sink = boost::add_vertex(G);
    
    std::vector<int> s(n);
    for(int i = 0; i < n; ++i){
      std::cin >> s[i];
      
      if(s[i] > 0){
        adder.add_edge(source, i, s[i]);
      }
      else{
        adder.add_edge(i, sink, -s[i]);
      }
    }
    
    int inf = 99999999;
    int u,v;
    for(int i = 0; i < m; ++i){
      std::cin >> u >> v;
      adder.add_edge(u, v, inf);
    }
    
    boost::push_relabel_max_flow(G, source, sink);
    std::vector<int> vis(n+2, false);
    std::queue<int> Q;
    
    vis[source] = true;
    Q.push(source);
    while (!Q.empty()) {
      const int u = Q.front();
      Q.pop();
      out_edge_it ebeg, eend;
      for (boost::tie(ebeg, eend) = boost::out_edges(u, G); ebeg != eend; ++ebeg) {
        const int v = boost::target(*ebeg, G);
        if (rc_map[*ebeg] == 0 || vis[v]) continue;
        vis[v] = true;
        Q.push(v);
      }
    }
  
    bool flag = false;
    int sum = 0;
    for (int i = 0; i < n; ++i) {
      if (vis[i]) {
        sum += s[i];
        flag = true;
      }
    }
    
    if(flag && sum > 0){
      std::cout << sum;
    }
    else{
      std::cout << "impossible";
    }
    
    std::cout << '\n';
  }
  
  return 0;
}