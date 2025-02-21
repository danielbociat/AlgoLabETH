///1
#include <iostream>
#include <vector>

#include <boost/graph/adjacency_list.hpp>
#include <boost/graph/cycle_canceling.hpp>
#include <boost/graph/push_relabel_max_flow.hpp>
#include <boost/graph/find_flow_cost.hpp>

// Graph Type with nested interior edge properties for Cost Flow Algorithms
typedef boost::adjacency_list_traits<boost::vecS, boost::vecS, boost::directedS> traits;
typedef boost::adjacency_list<boost::vecS, boost::vecS, boost::directedS, boost::no_property,
    boost::property<boost::edge_capacity_t, long,
        boost::property<boost::edge_residual_capacity_t, long,
            boost::property<boost::edge_reverse_t, traits::edge_descriptor>>>> graph;

typedef traits::vertex_descriptor vertex_desc;
typedef traits::edge_descriptor edge_desc;

// Custom edge adder class
class edge_adder {
  graph &G;

 public:
  explicit edge_adder(graph &G) : G(G) {}

  void add_edge(int from, int to, long capacity) {
    auto c_map = boost::get(boost::edge_capacity, G);
    auto r_map = boost::get(boost::edge_reverse, G);
    const auto e = boost::add_edge(from, to, G).first;
    const auto rev_e = boost::add_edge(to, from, G).first;
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
    int n,m;
    std::cin >> n >> m;
    
    graph G;
    edge_adder adder(G);  
    auto v_source = boost::add_vertex(G);
    auto v_sink = boost::add_vertex(G);
    
    int B = 0;
    std::vector<int>b(n);
    std::vector<vertex_desc> vd(n);
    for(int i = 0; i < n; ++i){
      std::cin >> b[i];
      vd[i] = boost::add_vertex(G);
      if(b[i] > 0){
        B+= b[i];
        adder.add_edge(v_source, vd[i], b[i]);
      }
      else{
        adder.add_edge(vd[i], v_sink, -b[i]);
      }
    }
    
    int u,v,d;
    for(int i = 0; i < m; ++i){
      std::cin >> u >> v >> d;
      adder.add_edge(vd[u], vd[v], d);
    }
    
    long flow = boost::push_relabel_max_flow(G, v_source, v_sink);
    
    if(flow < B){
      std::cout << "yes";
    }
    else{
      std::cout << "no";
    }
    std::cout << '\n';
  }
  
  return 0;
}