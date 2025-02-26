///3
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

int w, h;

bool isIn(int i, int j){
  return i >= 0 && j >= 0 && i < h && j < w;
}

int main(){
  std::ios_base::sync_with_stdio(false);
  
  int n;
  std::cin >> n;
  while(n--){
    std::cin >> w >> h;
    
    graph G;
    edge_adder adder(G);
    
    vertex_desc v_source = boost::add_vertex(G);
    vertex_desc v_sink = boost::add_vertex(G);
    
    std::vector<std::vector<vertex_desc>> vs(h, std::vector<vertex_desc>(w));
    
    for (int i = 0; i < h; ++i) {
      for (int j = 0; j < w; ++j) {
        vs[i][j] = boost::add_vertex(G);
      }
    }

    long target_flow = 0;
    for(int i = 0; i < h; ++i){
      for(int j = 0; j < w; ++j){
        char ch; std::cin >> ch;
        if(ch != '.') continue;
        
        if((i + j)%2){
          adder.add_edge(v_source, vs[i][j],1);
          
          if(isIn(i+1,j)) 
            adder.add_edge(vs[i][j], vs[i+1][j],1);
          if(isIn(i-1,j) ) 
            adder.add_edge(vs[i][j], vs[i-1][j],1);
          if(isIn(i,j+1)) 
            adder.add_edge(vs[i][j], vs[i][j+1],1);
          if(isIn(i,j-1)) 
            adder.add_edge(vs[i][j], vs[i][j-1],1);
        }
        else{
          adder.add_edge(vs[i][j],v_sink,1);
        }
        target_flow++;
      }
    }
    
    if(target_flow % 2){ 
      std::cout << "no\n";
      continue;
    }
    
    long flow = boost::push_relabel_max_flow(G, v_source, v_sink);
    
    target_flow /= 2;
    
    if(flow == target_flow) std::cout << "yes\n";
    else std::cout << "no\n";
  }
  
  return 0;
}