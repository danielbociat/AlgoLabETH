#include <iostream>
#include <CGAL/Exact_predicates_exact_constructions_kernel.h>

typedef CGAL::Exact_predicates_inexact_constructions_kernel K;
typedef K::Point_2 P;
typedef K::Segment_2 S;
typedef K::Ray_2 R;

int main(){
  std::ios_base::sync_with_stdio(false);
  
  int n;
  std::cin >> n;
  
  while(n){
    long x,y,a,b;
    std::cin >> x >> y >> a >> b;
    
    P X(x,y), Y(a,b);
    R R(X,Y);
    
    bool hits = false;
    
    for(int i = 0; i < n; ++i){
      long r,s,t,u;
      std::cin >> r >> s >> t >> u;
      
      if(!hits){
        P A(r,s), B(t,u);
        S S(A,B);
         
        if(CGAL::do_intersect(R,S)) { 
          hits = true;
        }
      }
    }
    
    if(hits)
      std::cout << "yes\n";
    else
      std::cout<< "no\n";
    
    std::cin >> n;
  }
  
  return 0;
}