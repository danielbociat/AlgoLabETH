///1
#include <iostream>
#include <vector>

// BGL include
#include <boost/graph/adjacency_list.hpp>

// BGL flow include *NEW*
#include <boost/graph/push_relabel_max_flow.hpp>

// Graph Type with nested interior edge properties for flow algorithms
typedef boost::adjacency_list_traits<boost::vecS, boost::vecS, boost::directedS> traits;
typedef boost::adjacency_list<boost::vecS, boost::vecS, boost::directedS, boost::no_property,
    boost::property<boost::edge_capacity_t, long,
        boost::property<boost::edge_residual_capacity_t, long,
            boost::property<boost::edge_reverse_t, traits::edge_descriptor>>>> graph;

typedef traits::vertex_descriptor vertex_desc;
typedef traits::edge_descriptor edge_desc;

// Custom edge adder class, highly recommended
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

bool isInside(int n, int i, int j){
  return i >= 0 && j >= 0 && i < n && j < n;
}

void try_add_edge(int ox, int oy, int i, int j, int n, std::vector<std::vector<vertex_desc>> &knights, edge_adder &adder){
  if(isInside(n, i, j)){
    adder.add_edge(knights[ox][oy], knights[i][j], 1);
  }
}

int main(){
  std::ios_base::sync_with_stdio(false);
  
  int t; 
  std::cin >> t;
  
  while(t--){
    int n;
    std::cin >> n;
    
    graph G(n*n);
    edge_adder adder(G);
    
    const vertex_desc v_source = boost::add_vertex(G);
    const vertex_desc v_sink = boost::add_vertex(G);
    
    int total_slots = 0;
    
    std::vector<std::vector<vertex_desc>> knights(n, std::vector<vertex_desc>(n));
    std::vector<std::vector<int>> board(n, std::vector<int>(n));
    
    for(int i = 0 ; i < n; ++i){
      for(int j = 0 ; j < n; ++j){
        knights[i][j] = boost::add_vertex(G);
      }
    }
    
    for(int i = 0 ; i < n; ++i){
      for(int j = 0 ; j < n; ++j){
        std::cin >> board[i][j];
        
        if(board[i][j]){
          total_slots++;
          
          if((i + j) % 2){
            adder.add_edge(v_source, knights[i][j], 1);
            try_add_edge(i,j, i-1, j-2, n, knights, adder);
            try_add_edge(i,j, i-1, j+2, n, knights, adder);
            try_add_edge(i,j, i+1, j-2, n, knights, adder);
            try_add_edge(i,j, i+1, j+2, n, knights, adder);
            try_add_edge(i,j, i-2, j-1, n, knights, adder);
            try_add_edge(i,j, i-2, j+1, n, knights, adder);
            try_add_edge(i,j, i+2, j-1, n, knights, adder);
            try_add_edge(i,j, i+2, j+1, n, knights, adder);
          }
          else{
            adder.add_edge(knights[i][j], v_sink, 1);
          }
        }
      }  
    }
    
    long flow = boost::push_relabel_max_flow(G, v_source, v_sink);
    
    std::cout << total_slots - flow << '\n';
  }
  
  
  return 0;
}