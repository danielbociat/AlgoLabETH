#include <iostream>
#include <vector>
#include <algorithm>

// BGL includes
#include <boost/graph/adjacency_list.hpp>
#include <boost/graph/cycle_canceling.hpp>
#include <boost/graph/push_relabel_max_flow.hpp>
#include <boost/graph/successive_shortest_path_nonnegative_weights.hpp>
#include <boost/graph/find_flow_cost.hpp>

// Graph Type with nested interior edge properties for Cost Flow Algorithms
typedef boost::adjacency_list_traits<boost::vecS, boost::vecS, boost::directedS> traits;
typedef boost::adjacency_list<boost::vecS, boost::vecS, boost::directedS, boost::no_property,
    boost::property<boost::edge_capacity_t, long,
        boost::property<boost::edge_residual_capacity_t, long,
            boost::property<boost::edge_reverse_t, traits::edge_descriptor,
                boost::property <boost::edge_weight_t, long> > > > > graph; // new! weightmap corresponds to costs

typedef boost::graph_traits<graph>::edge_descriptor             edge_desc;
typedef boost::graph_traits<graph>::vertex_descriptor           vd;
typedef boost::graph_traits<graph>::out_edge_iterator           out_edge_it; // Iterator

// Custom edge adder class
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

bool cmp(int a, int b){
  return a > b;
}

int main(){
  std::ios_base::sync_with_stdio(false);
  
  int t;
  std::cin >> t;
  
  while(t--){
    int n, m;
    int a,b,d, MAX_D = 1<<7;
    
    std::cin >> n >> m;
    
    graph G(n);
    edge_adder adder(G);  

    auto source = boost::add_vertex(G);
    auto sink = boost::add_vertex(G);
    
    std::vector<int> c(n-1);
    int sum = 0;
    for(int i = 0; i < n-1; ++i){
      std::cin >> c[i];
      sum += c[i];
      
      adder.add_edge(source, i, c[i], 0);
      adder.add_edge(i+1, sink, c[i], 0);
      
      adder.add_edge(i, i+1, c[i], MAX_D);
    }
    
    std::vector<int> items[505][505];
    for(int i = 0; i < m; ++i){
      std::cin >> a >> b >> d;
      items[a][b].push_back(d);
    }
    
    for(int i = 0; i < n-1; ++i){
      for(int j = i; j < n; ++j){
        
        if(items[i][j].size() == 0) continue;
        
        int min_cap = 99999999;
        
        for(int k = i; k < j; ++k){
          min_cap = std::min(min_cap, c[k]);
        }
        
        std::sort(items[i][j].begin(), items[i][j].end(), cmp);
        
        int mn = std::min(min_cap, (int)items[i][j].size());
        
        for(int k = 0; k < mn; ++k){
          adder.add_edge(i, j, 1, (j-i)*MAX_D - items[i][j][k]);
        }
      }
    }
    
    boost::successive_shortest_path_nonnegative_weights(G, source, sink);
    int cost = boost::find_flow_cost(G);

    std::cout << sum * MAX_D - cost << '\n';
  }
  
  return 0;
}