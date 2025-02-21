#include <iostream>
#include <vector>

#include <boost/graph/adjacency_list.hpp>
#include <boost/graph/cycle_canceling.hpp>
#include <boost/graph/push_relabel_max_flow.hpp>
#include <boost/graph/successive_shortest_path_nonnegative_weights.hpp>
#include <boost/graph/find_flow_cost.hpp>

typedef boost::adjacency_list_traits<boost::vecS, boost::vecS, boost::directedS> traits;
typedef boost::adjacency_list<boost::vecS, boost::vecS, boost::directedS, boost::no_property,
    boost::property<boost::edge_capacity_t, long,
        boost::property<boost::edge_residual_capacity_t, long,
            boost::property<boost::edge_reverse_t, traits::edge_descriptor,
                boost::property <boost::edge_weight_t, long> > > > > graph; // new! weightmap corresponds to costs

typedef boost::graph_traits<graph>::edge_descriptor             edge_desc;
typedef boost::graph_traits<graph>::vertex_descriptor           vd;
typedef boost::graph_traits<graph>::out_edge_iterator           out_edge_it; // Iterator

class edge_adder {
 graph &G;

 public:
  explicit edge_adder(graph &G) : G(G) {}
  void add_edge(int from, int to, long capacity, long cost) {
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
  
  int t;
  std::cin >> t;
  
  while(t--){
    int e, w, m, d;
    std::cin >> e >> w >> m >> d;
    
    int p, l;
    std::cin >> p >> l;
    
    int PAD = e + w;
    
    graph G(2*PAD);
    edge_adder adder(G);  
    auto c_map = boost::get(boost::edge_capacity, G);
    auto rc_map = boost::get(boost::edge_residual_capacity, G);

    auto source1 = boost::add_vertex(G);
    auto sink1 = boost::add_vertex(G);
    auto source2 = boost::add_vertex(G);
    auto sink2 = boost::add_vertex(G);
    
    // non-diff matches
    std::vector<int> app(e+w, 0);
    int u, v, r;
    for(int i = 0; i < m; ++i){
      std::cin >> u >> v >> r;
      
      app[u]++;
      app[e+v]++;
      
      adder.add_edge(u, e+v, 1, r);
    }

    std::vector<int> appDifficult(e+w, 0);
    for(int i = 0; i < d; ++i){
      std::cin >> u >> v >> r;
      
      appDifficult[u]++;
      appDifficult[e+v]++;
      
      adder.add_edge(PAD+u, PAD+e+v, 1, r);
    }
    
    for(int i = 0; i < e; ++i){
      int rm = app[i] - l;
      
      adder.add_edge(source1, i, l, 0);
      adder.add_edge(source2, i, rm, 0);
      
      adder.add_edge(source2, PAD+i, appDifficult[i], 0);
    }
    
    for(int i = 0; i < w; ++i){
      int rm = app[i+e] - l;
      
      adder.add_edge(i+e, sink1, l, 0);
      adder.add_edge(i+e, sink2, rm, 0);
      
      adder.add_edge(PAD+e+i, sink2, appDifficult[e+i], 0);
    }
    
    adder.add_edge(source1, source2, p - e*l, 0);
    adder.add_edge(sink2, sink1, p - w*l, 0);
    
    boost::successive_shortest_path_nonnegative_weights(G, source1, sink1);
    int cost = boost::find_flow_cost(G);
  
    int flow = 0;
    out_edge_it eit, eend;
    for(boost::tie(eit, eend) = boost::out_edges(boost::vertex(source1,G), G); eit != eend; ++eit) {
      flow += c_map[*eit] - rc_map[*eit];     
    }
  
    if (flow == p){
      std::cout << cost << std::endl;
    }else{
      std::cout << "No schedule!" << std::endl;
    }
    
  }
  
  return 0;
}