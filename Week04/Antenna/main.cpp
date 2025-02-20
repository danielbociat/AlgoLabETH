#include <iostream>
#include <vector>
#include <CGAL/Exact_predicates_exact_constructions_kernel_with_sqrt.h>
#include <CGAL/Min_circle_2.h>
#include <CGAL/Min_circle_2_traits_2.h>

typedef CGAL::Exact_predicates_exact_constructions_kernel_with_sqrt K;
typedef K::Point_2 P;
typedef CGAL::Min_circle_2_traits_2<K>  Traits;
typedef CGAL::Min_circle_2<Traits>      Min_circle;

double floor_to_double(const K::FT& x)
{
  double a = std::floor(CGAL::to_double(x));
  while (a > x) a -= 1;
  while (a+1 <= x) a += 1;
  return a;
}

int main(){
  std::ios_base::sync_with_stdio(false);
  
  int n;
  std::cin >> n;
  while(n){
    std::vector<P> citizens(n);
    
    for(int i = 0; i < n; ++i){
      long a,b;
      std::cin >> a >> b;
      
      citizens[i] = P(a,b);
    }
    
    Min_circle mc(citizens.begin(), citizens.end(), true);
    Traits::Circle c = mc.circle();
    
    std::cout << long(-floor_to_double(-CGAL::sqrt(c.squared_radius()))) << '\n';
    
    std::cin >> n;
  }
  
  return 0;
}