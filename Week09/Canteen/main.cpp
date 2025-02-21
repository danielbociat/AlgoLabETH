///1
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
  
  int t; std::cin >> t;
  
  while(t--){
    int n;
    std::cin >> n;
    
    graph G;
    edge_adder adder(G);
    
    const vertex_desc v_source = boost::add_vertex(G);
    const vertex_desc v_sink = boost::add_vertex(G);
    
    std::vector<int> amount_menus(n);
    std::vector<int> cost_per_menu(n);
    
    std::vector<vertex_desc> days(n);
    int a, c;
    for(int i = 0; i < n; ++i){
      std::cin >> a >> c;
      
      days[i] = boost::add_vertex(G);
      adder.add_edge(v_source, days[i], a, c);
    }
    
    int total_students = 0;
    
    std::vector<int> num_students(n);
    std::vector<int> price_per_menu(n);
    int s,p;
    for(int i = 0; i < n; ++i){
      std::cin >> s >> p;
      total_students += s;
      adder.add_edge(days[i], v_sink, s, 20-p);
    }
    
    int v,e;
    int v_curr = 0;
    for(int i = 0; i < n-1; ++i){
      std::cin >> v >> e;
      adder.add_edge(days[i], days[i+1], v, e);
    }
    
    auto c_map = boost::get(boost::edge_capacity, G);
    auto rc_map = boost::get(boost::edge_residual_capacity, G);
    
    boost::successive_shortest_path_nonnegative_weights(G, v_source, v_sink);
    int min_cost = boost::find_flow_cost(G);
  
    int s_flow = 0;
    out_edge_it ee, eend;
    for(boost::tie(ee, eend) = boost::out_edges(boost::vertex(v_source,G), G); ee != eend; ++ee) {
        s_flow += c_map[*ee] - rc_map[*ee];     
    }
    
    if(s_flow == total_students){
      std::cout << "possible ";
    }
    else{
      std::cout << "impossible ";
    }
    std::cout << s_flow << ' ' << 20*s_flow - min_cost << '\n';
    
  }
  
  return 0;
}