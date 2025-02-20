///1
#include <iostream>
#include <vector>

#include <boost/graph/adjacency_list.hpp>
#include <boost/graph/push_relabel_max_flow.hpp>

typedef boost::adjacency_list_traits<boost::vecS, boost::vecS, boost::directedS> traits;
typedef boost::adjacency_list<boost::vecS, boost::vecS, boost::directedS, boost::no_property,
    boost::property<boost::edge_capacity_t, long,
        boost::property<boost::edge_residual_capacity_t, long,
            boost::property<boost::edge_reverse_t, traits::edge_descriptor>>>> graph;

typedef traits::vertex_descriptor vertex_desc;
typedef traits::edge_descriptor edge_desc;

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
    int m, n, k, C;
    std::cin >> m >> n >> k >> C;
    
    graph G;
    edge_adder adder(G);
    
    vertex_desc v_source = boost::add_vertex(G);
    vertex_desc v_sink = boost::add_vertex(G);
    
    std::vector<std::vector<vertex_desc>> interIn(n, std::vector<vertex_desc>(m));
    std::vector<std::vector<vertex_desc>> interOut(n, std::vector<vertex_desc>(m));
    
    for(int i = 0; i < n; ++i){
      for(int j = 0; j < m; ++j){
        interIn[i][j] = boost::add_vertex(G);
        interOut[i][j] = boost::add_vertex(G);
        adder.add_edge(interIn[i][j], interOut[i][j], C);
      }
    }
    
    for(int i = 0; i < n; ++i){
      for(int j = 0; j < m; ++j){
        if(i == 0 || j == 0 || i == n-1 || j == m-1){
          int exits = (i == 0) + (j == 0) + (i == n-1) + (j == m-1);
          adder.add_edge(interOut[i][j], v_sink, exits);
        }
        
        /// Not exits
        if(i > 0){
          adder.add_edge(interOut[i][j], interIn[i-1][j], 1);
        }
        if(j > 0){
          adder.add_edge(interOut[i][j], interIn[i][j-1], 1);
        }
        if(i < n-1){
          adder.add_edge(interOut[i][j], interIn[i+1][j], 1);
        }
        if(j < m-1){
          adder.add_edge(interOut[i][j], interIn[i][j+1], 1);
        }
      }
    }
    
    int ki, kj;
    for(int i = 0; i < k; ++i){
      std::cin >> kj >> ki;
      adder.add_edge(v_source, interIn[ki][kj], 1);
    }
    
    long flow = boost::push_relabel_max_flow(G, v_source, v_sink);
    
    std::cout << flow << '\n';
  }
  
  return 0;
}