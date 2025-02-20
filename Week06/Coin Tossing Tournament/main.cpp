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

typedef std::pair<int, int> game;

int main(){
  std::ios_base::sync_with_stdio(false);
  
  int t;
  std::cin >> t;
  
  while(t--){
    int n, m;
    std::cin >> n >> m;
    
    std::vector<int> players(n, 0);
    std::vector<game> undecided_games;
    
    int a,b,c;
    for (int i = 0; i < m; ++i){
      std::cin >> a >> b >> c;
      
      if(c == 0){
        undecided_games.push_back({a, b});
      }
      else if(c == 1){
        players[a]++;
      }
      else{
        players[b]++;
      }
    }
    
    int final_score;
    int target_flow = 0;
    bool flag = true;
    for(int i = 0; i < n; ++i){
      std::cin >> final_score;
      players[i] = final_score - players[i];
      
      if(players[i] < 0){
        flag = false;
      }
      
      target_flow += players[i];
    }
    
    int undecided_games_no = undecided_games.size();
    if(target_flow != undecided_games_no){
      std::cout << "no\n";
      continue;
    }
    
    if(!flag){
      std::cout << "no\n";
      continue;
    }
    
    graph G;
    edge_adder adder(G);
    
    vertex_desc v_source = boost::add_vertex(G);
    vertex_desc v_sink = boost::add_vertex(G);
    
    std::vector<vertex_desc> player_nodes(n);
    for(int i = 0; i < n; ++i){
      player_nodes[i] = boost::add_vertex(G);
      adder.add_edge(player_nodes[i], v_sink, players[i]);
    }
    
    
    std::vector<vertex_desc> games(undecided_games_no);
    for(int i = 0; i < undecided_games_no; ++i){
      games[i] = boost::add_vertex(G);
      adder.add_edge(v_source, games[i], 1);
      adder.add_edge(games[i], player_nodes[undecided_games[i].first], 1);
      adder.add_edge(games[i], player_nodes[undecided_games[i].second], 1);
    }
    
    long flow = boost::push_relabel_max_flow(G, v_source, v_sink);
    
    if(flow != target_flow){
      std::cout << "no\n";
    }
    else{
      std::cout << "yes\n";
    }
    
  }
  
  return 0;
}