#include <iostream>
#include <vector>
#include <tuple>

#include <CGAL/QP_models.h>
#include <CGAL/QP_functions.h>
#include <CGAL/Gmpz.h>

typedef int IT;
typedef CGAL::Gmpz ET;
typedef CGAL::Quadratic_program<IT> Program;
typedef CGAL::Quadratic_program_solution<ET> Solution;

int n,m;

std::vector<std::vector<int>> r, graph;
std::vector<int> d, d_idx;

double floor_to_double(const CGAL::Quotient<ET> & x){
  double a = std::floor(CGAL::to_double(x));
  
  while (a > x) a -= 1;
  while (a+1 <= x) a += 1;
  
  return a;
}

void dfs(Program &lp, int root, int curr_node){
  if(curr_node != root && d_idx[curr_node] >= 0){
    for(int j = 0 ; j < m; ++j){
      lp.set_a(d_idx[curr_node]*m + j, d_idx[root] * (m+1) + j, -1);
    }
  }
  else{
    if(curr_node != root){
        for(int j = 0; j < m; ++j){
          r[root][j] += r[curr_node][j];
        }
    }
   
    for(auto neigh : graph[curr_node]){
        dfs(lp, root, neigh);
    }
  }
  
  if(curr_node == root){
    for(int j = 0; j < m; ++j){
      lp.set_a(d_idx[root] * m + j, d_idx[root] * (m+1) + j, 2);
      lp.set_b(d_idx[root] * (m+1) + j, 2*r[root][j]);
    }
  }
}


int main(){
  std::ios_base::sync_with_stdio(false);
  
  int t;
  std::cin >> t;
  while(t--){
    
    std::cin >> n >> m;
    Program lp (CGAL::SMALLER, true, 0, false, 0); 
    
    int idx = 1; 
    
    d = std::vector<int> (n);
    d_idx = std::vector<int> (n,-1);
    r = std::vector<std::vector<int>> (n, std::vector<int>(m));
    
    for(int i = 0; i < n; ++i){
      std::cin >> d[i];
      
      if(d[i] >= 0){
        d_idx[i] = idx;
        
        for(int j = 0; j < m; ++j){
          lp.set_a(d_idx[i]*m + j, d_idx[i]*(m+1)+m, 1);
        }
        lp.set_b(d_idx[i]*(m+1) + m, d[i]);
        
        idx++;
      }
      
      for(int j = 0; j < m; ++j){
        std::cin >> r[i][j];
      }
    }
    
    int u,v;
    graph = std::vector<std::vector<int>> (n);
    for(int i = 0; i < n-1; ++i){
      std::cin >> u >> v;
      
      graph[v].push_back(u);
    }
    
    for(int i = 0; i < n; ++i){
      if(d[i] >= 0)
        dfs(lp, i, i);
    }
    d_idx[0] = 0;
    dfs(lp, 0, 0);
    
    int c, s, p;
    int PADDING = idx;
    for (int j = 0; j < m; j++) {
      std::cin >> c >> s >> p;
      lp.set_a(d_idx[0]*m + j, PADDING * (m+1) + j, -1); /// exit + bought >= c  ==>  -exit - bought <= -c
      lp.set_a(PADDING*m + j, PADDING * (m+1) + j, -1);
      lp.set_b(PADDING*(m + 1) + j, -c);
  
      lp.set_a(PADDING*m + j, PADDING*(m + 2) + j, 1);  /// bought <= supply
      lp.set_b(PADDING*(m + 2) + j, s);
  
      lp.set_c(PADDING*m + j, p);
    }
    
    Solution sol = CGAL::solve_linear_program(lp, ET());
    
    if(sol.is_infeasible()){
      std::cout << "Impossible!\n";
    }
    else{
      std::cout << (long)floor_to_double(sol.objective_value()) << '\n';
    }
  }
  
  return 0;
}