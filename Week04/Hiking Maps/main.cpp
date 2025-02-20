#include <iostream>
#include <vector>
#include <unordered_map>

#include <CGAL/Exact_predicates_exact_constructions_kernel.h>

typedef CGAL::Exact_predicates_exact_constructions_kernel K;
typedef K::Point_2 P;
typedef K::Line_2 L;

bool is_covered(P t1, P t2, P t3, P p){
  int isLeft1 = CGAL::orientation(t1, t2, p);
  int isLeft2 = CGAL::orientation(t2, t3, p);
  int isLeft3 = CGAL::orientation(t3, t1, p);
  
  if(isLeft1 >= 0 && isLeft2 >= 0 && isLeft3 >= 0) return true;
  if(isLeft1 <= 0 && isLeft2 <= 0 && isLeft3 <= 0) return true;
  
  return false;
}

int main(){
  std::ios_base::sync_with_stdio(false);
  
  int c;
  std::cin >> c;
  
  while(c--){
    int m, n;
    std::cin >> m >> n;
    
    std::vector <P> paths(m);
    for(int i = 0; i < m; ++i){
      int a, b;
      std::cin >> a >> b;
      paths[i] = P(a,b);
    }
    
    std::vector<std::vector<int>> legs_covered_by_triangles(n);
    std::vector<P> q(6);
    for(int i = 0; i < n; ++i){
      for(int j = 0; j < 6; ++j){
        int qi, qj;
        std::cin >> qi >> qj;
        q[j] = P(qi, qj);
      }
      
      L l1 = L(q[0], q[1]);
      L l2 = L(q[2], q[3]);
      L l3 = L(q[4], q[5]);
      
      auto o1 = CGAL::intersection(l1, l2);
      P t1 = *boost::get<P>(&*o1);
      auto o2 = CGAL::intersection(l2, l3);
      P t2 = *boost::get<P>(&*o2);
      auto o3 = CGAL::intersection(l3, l1);
      P t3 = *boost::get<P>(&*o3);
      
      std::vector<bool> isCovered(m);
      for(int j = 0; j < m; ++j){
        isCovered[j] = is_covered(t1,t2,t3,paths[j]);
      }
      
      for(int j = 0; j < m-1; ++j){
        if(isCovered[j] && isCovered[j+1]){
          legs_covered_by_triangles[i].push_back(j);
        } 
      }
    }
    
    int best_sol = n;
    int left = 0, right = 0;
    int legs_covered = 0;
    std::vector<int> leg_covered(m-1, 0);
    
    legs_covered = (int)legs_covered_by_triangles[0].size();
    for(int el : legs_covered_by_triangles[0]){
      leg_covered[el]++;
    }
    
    while(left < n && right < n){
      if(legs_covered < m-1){
        right++;
        
        if(right == n) break;
         
        for(int el : legs_covered_by_triangles[right]){
          if(leg_covered[el] == 0) legs_covered++;
          leg_covered[el]++;
        }
      }
      else{
        if(legs_covered == m-1){
          best_sol = std::min(best_sol, right-left+1);
          
          for(int el : legs_covered_by_triangles[left]){
            leg_covered[el]--;
            if(leg_covered[el] == 0) legs_covered--;
          }
          
          left++;
        }
      }
    }
    
    std::cout << best_sol << '\n';
  }
  
  return 0;
}