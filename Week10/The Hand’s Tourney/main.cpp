///4
#include <iostream>
#include <vector>
#include <algorithm>

#include <CGAL/Exact_predicates_inexact_constructions_kernel.h>
#include <CGAL/Delaunay_triangulation_2.h>
#include <CGAL/squared_distance_2.h>
#include <CGAL/Triangulation_face_base_with_info_2.h>
#include <CGAL/Triangulation_vertex_base_with_info_2.h>

#include <boost/pending/disjoint_sets.hpp>
#include <boost/graph/connected_components.hpp>
#include <boost/graph/adjacency_list.hpp>
#include <boost/pending/detail/disjoint_sets.hpp>

typedef boost::adjacency_list< boost::vecS, boost::vecS, boost::undirectedS > Graph;

typedef CGAL::Exact_predicates_inexact_constructions_kernel K;

typedef CGAL::Triangulation_vertex_base_with_info_2<int, K> Vb;
typedef CGAL::Triangulation_face_base_with_info_2<int, K> Fb;
typedef CGAL::Triangulation_data_structure_2<Vb,Fb> Tds;
typedef CGAL::Delaunay_triangulation_2<K, Tds>  Triangulation;

typedef K::Point_2 P;
typedef std::pair<P, int> P_info;
typedef std::tuple<int, int, long> Edge;

bool cmp_edge(const Edge e1, const Edge e2){
  return std::get<2>(e1) < std::get<2>(e2);
}

int compute_f(int k, std::vector<int> ccomp_size){
  int mn;
  int sol_f = 0;
  
  switch(k){
    case 1:
      for(int i = 1; i < 5; ++i)
        sol_f += ccomp_size[i];
      break;
    case 2:
      for(int i = 2; i < 5; ++i)
        sol_f += ccomp_size[i];
      sol_f += ccomp_size[1] / 2;
      break;
    case 3:
      for(int i = 3; i < 5; ++i)
        sol_f += ccomp_size[i];
      mn = std::min(ccomp_size[1], ccomp_size[2]);
      sol_f += mn;
      sol_f += (ccomp_size[1]-mn)/3;
      sol_f += (ccomp_size[2]-mn)/2;
      break;
      
    case 4:
      sol_f += ccomp_size[4];
      
      mn = std::min(ccomp_size[3], ccomp_size[1]);
      sol_f += mn;
      
      sol_f += (ccomp_size[3] + ccomp_size[2] - mn) / 2;
      sol_f += (ccomp_size[1]-mn) / 4;
      
      if(((ccomp_size[3] - mn + ccomp_size[2]) % 2 == 1) && ((ccomp_size[1]-mn)%4>=2))
        sol_f++;
      
      break;
  }
  
  return sol_f;
}

int main(){
  std::ios_base::sync_with_stdio(false);
  
  int t;
  std::cin >> t;
  
  while(t--){
    long long n, k, f0, s0;
    std::cin >> n >> k >> f0 >> s0;
    
    int x,y;
    std::vector<P_info> tents(n);
    for(int i = 0; i < n; ++i){
      std::cin >> x >> y;
      tents[i] = {P(x,y), i}; 
    }
    
    Triangulation dt;
    dt.insert(tents.begin(), tents.end());
    
    Graph G(n);
    std::vector<Edge> edges;
    
    for (auto e = dt.finite_edges_begin(); e != dt.finite_edges_end(); ++e) {
      int i1 = e->first->vertex((e->second+1)%3)->info();
      int i2 = e->first->vertex((e->second+2)%3)->info();
      
      edges.push_back({i1, i2, dt.segment(e).squared_length()});
      
      
      if(dt.segment(e).squared_length() < s0){
        boost::add_edge(i1, i2, G);
      }
    }
    
    sort(edges.begin(), edges.end(), cmp_edge);
    
    boost::disjoint_sets_with_storage<> uf(n);
    
    double sol_s = 0;
    int sol_f = 0;
    
    std::vector<int> nums(n, 1);
    std::vector<int> ccomp_size2(5, 0);
    ccomp_size2[1] = n;
    
    for(auto edge : edges){
      int i1 = std::get<0>(edge);
      int i2 = std::get<1>(edge);
      
      int c1 = uf.find_set(i1);
      int c2 = uf.find_set(i2);
      
      sol_s = std::get<2>(edge);
      
      if(c1 != c2){
        int n1 = nums[c1];
        int n2 = nums[c2];
        
        uf.link(c1, c2);
        int c3 = uf.find_set(i1);
        
        nums[c1] = 0;
        nums[c2] = 0;
        nums[c3] = std::min(n1 + n2, 4);
        
        ccomp_size2[nums[c3]]++;
        ccomp_size2[n1]--;
        ccomp_size2[n2]--;
        
        if(compute_f(k, ccomp_size2) < f0) break;
      }
    }
    
    std::cout << long(sol_s) << ' ';
    
    std::vector< int > component(num_vertices(G));
    int num = connected_components(G, &component[0]);
    
    std::vector<int> comp_size(num, 0);
    for(int i = 0; i < n; ++i){
      comp_size[component[i]]++;
    }
    
    std::vector<int> ccomp_size(5, 0);
    for(auto c_size : comp_size){
      if(c_size >= 4){
        ccomp_size[4]++;
      }
      else{
        ccomp_size[c_size]++;
      }
    }

    sol_f = compute_f(k, ccomp_size);
    
    std::cout << sol_f << '\n';
    
  }
  return 0;
}