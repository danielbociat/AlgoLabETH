#include <iostream>
#include <vector>
#include <CGAL/Exact_predicates_exact_constructions_kernel.h>

typedef CGAL::Exact_predicates_exact_constructions_kernel K;
typedef K::Point_2 P;
typedef K::Segment_2 S;
typedef K::Ray_2 R;

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
    long x,y,a,b;
    std::cin >> x >> y >> a >> b;
    
    P best_point(0,0);
    
    P src(x,y), trg(a,b);
    R Ray(src, trg);
    
    bool hits = false;
    
    S BestSeg(src, trg);
    
    std::vector<S> segments(n);
    
    for(int i = 0; i < n; ++i){
      long r,s,t,u;
      std::cin >> r >> s >> t >> u;
      
      P p1(r,s), p2(t,u);
      S Seg(p1,p2);
      
      segments[i] = Seg;
    }
    random_shuffle(segments.begin(), segments.end());
    
    for(int i = 0; i < n; ++i){
      P p1 = segments[i].source();
      P p2 = segments[i].target();
      
      if((hits && CGAL::do_intersect(BestSeg,segments[i])) || 
         (!hits && CGAL::do_intersect(Ray,segments[i]))) { 
        auto o = hits ? CGAL::intersection(BestSeg,segments[i]) : CGAL::intersection(Ray,segments[i]);
        
        if (const P* op = boost::get<P>(&*o)){
          best_point = *op;
          BestSeg = S(src, best_point);
        }
        else{
          best_point = CGAL::has_smaller_distance_to_point(src, p1, p2) ? p1 : p2;
          BestSeg = S(src, best_point);
        }
        
        hits = true;
      }
    }
    
    if(hits)
      std::cout << long(floor_to_double(best_point.x())) << ' ' <<  long(floor_to_double(best_point.y())) << "\n";
    else
      std::cout<< "no\n";
    
    std::cin >> n;
  }
  
  return 0;
}