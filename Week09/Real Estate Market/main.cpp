#include <iostream>
#include <vector>

#include <boost/graph/adjacency_list.hpp>
#include <boost/graph/cycle_canceling.hpp>
#include <boost/graph/push_relabel_max_flow.hpp>
#include <boost/graph/successive_shortest_path_nonnegative_weights.hpp>
#include <boost/graph/find_flow_cost.hpp>

typedef boost::adjacency_list_traits<boost::vecS, boost::vecS, boost::directedS> traits;
typedef boost::adjacency_list<boost::vecS, boost::vecS, boost::directedS, boost::no_property,
    boost::property<boost::edge_capacity_t, int,
        boost::property<boost::edge_residual_capacity_t, int,
            boost::property<boost::edge_reverse_t, traits::edge_descriptor,
                boost::property <boost::edge_weight_t, int> > > > > graph; // new! weightmap corresponds to costs

typedef traits::vertex_descriptor vertex_desc;
typedef boost::graph_traits<graph>::edge_descriptor             edge_desc;
typedef boost::graph_traits<graph>::out_edge_iterator           out_edge_it; // Iterator

class edge_adder {
 graph &G;

 public:
  explicit edge_adder(graph &G) : G(G) {}
  void add_edge(int from, int to, int capacity, int cost) {
    auto c_map = boost::get(boost::edge_capacity, G);
    auto r_map = boost::get(boost::edge_reverse, G);
    auto w_map = boost::get(boost::edge_weight, G); // new!
    const edge_desc e = boost::add_edge(from, to, G).first;
    const edge_desc rev_e = boost::add_edge(to, from, G).first;
    c_map[e] = capacity;
    c_map[rev_e] = 0; // reverse edge has no capacity!
    r_map[e] = rev_e;
    r_map[rev_e] = e;
    w_map[e] = cost;   // new assign cost
    w_map[rev_e] = -cost;   // new negative cost
  }
};

int main(){
  std::ios_base::sync_with_stdio(false);
  
  int T;
  std::cin >> T;
  
  while(T--){
    int N, M, S;
    std::cin >> N >> M >> S;
    
    graph G;
    edge_adder adder(G);
    
    vertex_desc v_source = boost::add_vertex(G);
    vertex_desc v_sink = boost::add_vertex(G);
    
    std::vector<vertex_desc> buyers(N);
    for(int i = 0; i < N; ++i){
      buyers[i] = boost::add_vertex(G);
      adder.add_edge(v_source, buyers[i], 1, 0);
    }
    
    std::vector<vertex_desc> states(S);
    int l;
    for(int i = 0; i < S; ++i){
      std::cin >> l;
      states[i] = boost::add_vertex(G);
      adder.add_edge(states[i], v_sink, l, 0);
    }
    
    std::vector<vertex_desc> locations(M);
    int s;
    for(int i = 0; i < M; ++i){
      std::cin >> s;
      locations[i] = boost::add_vertex(G);
      adder.add_edge(locations[i], states[s-1], 1, 0);
    }
    
    int max_bid = 0;
    std::vector<std::vector<int>> bids(N, std::vector<int>(M));
    for(int i = 0; i < N; ++i){
      for(int j = 0; j < M; ++j){
        std::cin >> bids[i][j];
        max_bid = std::max(max_bid, bids[i][j]);
      }
    }
    for(int i = 0; i < N; ++i){
      for(int j = 0; j < M; ++j){
        adder.add_edge(buyers[i], locations[j], 1, -bids[i][j] + max_bid);
      }
    }
    
    boost::successive_shortest_path_nonnegative_weights(G, v_source, v_sink);
    int max_profit = boost::find_flow_cost(G);

    int flow = 0;
    const auto c_map = boost::get(boost::edge_capacity, G);
    const auto rc_map = boost::get(boost::edge_residual_capacity, G);
    out_edge_it e, eend;
    for(boost::tie(e, eend) = boost::out_edges(boost::vertex(v_source,G), G); e != eend; ++e) {
      flow += c_map[*e] - rc_map[*e];
    }

    std::cout << flow << ' ' << flow*max_bid - max_profit  << '\n';
  }
  
  return 0;
}