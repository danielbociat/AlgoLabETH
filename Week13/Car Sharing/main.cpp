///5
#include <iostream>
#include <vector>
#include <map>
#include <set>

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

typedef boost::graph_traits<graph>::vertex_descriptor             vertex_desc;
typedef boost::graph_traits<graph>::edge_descriptor             edge_desc;
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
  
  int T;
  std::cin >> T;
  
  while(T--){
    int N,S;
    
    std::cin >> N >> S;
    
    graph G;
    edge_adder adder(G);
    
    int source = boost::add_vertex(G);
    int sink = boost::add_vertex(G);
    
    std::map<std::pair<int, int>, int> nodes;
    
    long max_time = 100000;
    std::vector<std::set<int>> times(S+1);
    
    std::vector <int> l(S);
    
    long total_cars = 0;
    for(int i = 0; i < S; ++i){
      std::cin >> l[i];
      
      total_cars += l[i];
      
      times[i].insert(0);
      times[i].insert(max_time);
    }
    
    int s, t, d, a, p;
    long MAX_P = 100;
    for(int i = 0; i < N; ++i){
      std::cin >> s >> t >> d >> a >> p;
      
      times[s-1].insert(d);
      times[t-1].insert(a);
      
      if(nodes.find({s-1, d}) == nodes.end()){
        nodes[{s-1,d}] = boost::add_vertex(G);
      }
      
      if(nodes.find({t-1, a}) == nodes.end()){
        nodes[{t-1,a}] = boost::add_vertex(G);
      }
      
      adder.add_edge(nodes[{s-1,d}], nodes[{t-1,a}], 1, (a-d)*MAX_P - p);
    }
    
    int last_time; 
    for(int i = 0; i < S; ++i){
      if(nodes.find({i, 0}) == nodes.end()){
        nodes[{i,0}] = boost::add_vertex(G);
      }
      if(nodes.find({i, max_time}) == nodes.end()){
        nodes[{i,max_time}] = boost::add_vertex(G);
      }
      
      adder.add_edge(source, nodes[{i,0}], l[i], 0);
      adder.add_edge(nodes[{i,max_time}], sink, total_cars, 0);
      
      last_time = 0;
      for(auto tm : times[i]){
        if(tm == 0) continue;
        
        adder.add_edge(nodes[{i,last_time}], nodes[{i,tm}], total_cars, (tm-last_time)*MAX_P);
        last_time = tm;
      }
    }
    
    boost::successive_shortest_path_nonnegative_weights(G, source, sink);
    long cost = boost::find_flow_cost(G);
  
    std::cout << (total_cars * MAX_P * max_time) - cost << "\n";
  }
  
  return 0;
}