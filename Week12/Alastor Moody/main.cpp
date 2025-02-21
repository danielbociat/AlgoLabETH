#include <iostream>
#include <vector>

#include <boost/graph/adjacency_list.hpp>
#include <boost/graph/dijkstra_shortest_paths.hpp>

#include <boost/graph/push_relabel_max_flow.hpp>
#include <boost/graph/successive_shortest_path_nonnegative_weights.hpp>
#include <boost/graph/find_flow_cost.hpp>

typedef boost::adjacency_list<boost::vecS, boost::vecS, boost::undirectedS,
  boost::no_property, boost::property<boost::edge_weight_t, int> >      weighted_graph;
typedef boost::property_map<weighted_graph, boost::edge_weight_t>::type weight_map;
typedef boost::graph_traits<weighted_graph>::edge_descriptor            edge_desc;
typedef boost::graph_traits<weighted_graph>::vertex_descriptor          vertex_desc;


typedef boost::adjacency_list_traits<boost::vecS, boost::vecS, boost::directedS> traits;
typedef boost::adjacency_list<boost::vecS, boost::vecS, boost::directedS, boost::no_property,
    boost::property<boost::edge_capacity_t, long,
        boost::property<boost::edge_residual_capacity_t, long,
            boost::property<boost::edge_reverse_t, traits::edge_descriptor,
                boost::property <boost::edge_weight_t, long> > > > > graph; // new! weightmap corresponds to costs

typedef boost::graph_traits<graph>::edge_descriptor             edge_desc_flow;
typedef boost::graph_traits<graph>::vertex_descriptor             vertex_desc_flow;
typedef boost::graph_traits<graph>::out_edge_iterator           out_edge_it; // Iterator

class edge_adder {
 graph &G;

 public:
  explicit edge_adder(graph &G) : G(G) {}
  void add_edge(int from, int to, long capacity, long cost) {
    auto c_map = boost::get(boost::edge_capacity, G);
    auto r_map = boost::get(boost::edge_reverse, G);
    auto w_map = boost::get(boost::edge_weight, G); // new!
    const edge_desc_flow e = boost::add_edge(from, to, G).first;
    const edge_desc_flow rev_e = boost::add_edge(to, from, G).first;
    c_map[e] = capacity;
    c_map[rev_e] = 0; // reverse edge has no capacity!
    r_map[e] = rev_e;
    r_map[rev_e] = e;
    w_map[e] = cost;   // new assign cost
    w_map[rev_e] = -cost;   // new negative cost
  }
};

int dijkstra_dist(const weighted_graph &G, int s, int t) {
  int n = boost::num_vertices(G);
  std::vector<int> dist_map(n);

  boost::dijkstra_shortest_paths(G, s,
    boost::distance_map(boost::make_iterator_property_map(
      dist_map.begin(), boost::get(boost::vertex_index, G))));

  return dist_map[t];
}

bool test_flow(graph &G_flow, edge_adder &adder, int target_val, int target_cost_per_unit, int s, int p){
  auto c_map = boost::get(boost::edge_capacity, G_flow);
  auto rc_map = boost::get(boost::edge_residual_capacity, G_flow);
  
  auto v_source = boost::add_vertex(G_flow);
  auto v_sink = boost::add_vertex(G_flow);
  
  adder.add_edge(v_source, s, target_val, 0);
  adder.add_edge(p, v_sink, target_val, 0);
  
  boost::successive_shortest_path_nonnegative_weights(G_flow, v_source, v_sink);
  int cost = boost::find_flow_cost(G_flow);
  
  bool ret = true;
  
  int s_flow = 0;
  out_edge_it e, eend;
  for(boost::tie(e, eend) = boost::out_edges(boost::vertex(v_source,G_flow), G_flow); e != eend; ++e) {
      s_flow += c_map[*e] - rc_map[*e];     
  }
  
  if(s_flow < target_val || cost != target_val * target_cost_per_unit){
    ret = false;
  }
  
  boost::remove_vertex(v_source, G_flow);
  boost::remove_vertex(v_sink, G_flow);
  
  return ret;
}

int main(){
  std::ios_base::sync_with_stdio(false);
  
  int t;
  std::cin>> t;
  
  while(t--){
    int n, m, s, p;
    std::cin >> n >> m >> s >> p;
    
    weighted_graph G(n);
    weight_map weights = boost::get(boost::edge_weight, G);
    
    graph G_flow(n);
    edge_adder adder(G_flow);  
    
    int u,v,c,d;
    edge_desc e;
    int max_c = -1;
    int max_paths_from_s = 0;
    int max_paths_from_p = 0;
    for(int i = 0; i < m; ++i){
      std::cin >> u >> v >> c >> d;
      max_c = std::max(c, max_c);
      
      if(u == s || v == s) max_paths_from_s++;
      if(u == p || v == p) max_paths_from_p++;
      
      e = boost::add_edge(u, v, G).first; weights[e] = d;
      
      adder.add_edge(u, v, c, d);
      adder.add_edge(v, u, c, d);
    }
    
    int target_dist = dijkstra_dist(G, s, p);
    
    int left = 1, right = std::min(max_paths_from_p, max_paths_from_s)*max_c;
    int mid;
    int best_sol = 0;
    while(left <= right) {
      mid = left + (right-left)/2;
      
      //min = target flow
      //target_dist = cost per unit of flow
      if(test_flow(G_flow, adder, mid, target_dist, s, p)){
        left = mid+1;
        best_sol = std::max(best_sol, mid);
      }
      else{
        right = mid-1;
      }
    }

    std::cout <<  best_sol << '\n';
  }
  
  return 0;
}