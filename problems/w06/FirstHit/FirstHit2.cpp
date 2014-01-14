#include <CGAL/Exact_predicates_exact_constructions_kernel.h>
#include <iostream>

using namespace std;

typedef CGAL::Exact_predicates_exact_constructions_kernel K;
typedef K::Point_2 P;
typedef K::Segment_2 S;

double floor_to_double(const K::FT& x) {
    double a = std::floor(CGAL::to_double(x));
    while(a>x) a-=1;
    while(a+1<=x) a+=1;
    return a;
}

bool hitsObstacle(int numObstacles, K::Point_2 &isPos) {

    bool isHit = false;
    double x, y, a, b;
    cin >> x >> y >> a >> b;
    P philStart(x,y);
    K::Ray_2 phil(philStart, K::Point_2(a,b));
    P minIntersectionPoint;
    // cerr << "Phil " << phil << endl;

    for(int i=0; i<numObstacles; i++) {
        // cerr << "Loop " << i << endl;
        double r,s,t,u;
        cin >> r >> s >> t >> u;
        K::Segment_2 obstacle(K::Point_2(r,s), K::Point_2(t,u));
        if(CGAL::do_intersect(phil, obstacle)) {
            // cerr << "Intersection " << i << endl;
            P intersectionPoint;
            CGAL::Object o = CGAL::intersection(phil, obstacle);
            if(const P* op = CGAL::object_cast<P>(&o)) {
                intersectionPoint = *op;
            } else if(const S* os = CGAL::object_cast<S>(&o)) {
                //take the segment end that is nearer
                intersectionPoint = CGAL::squared_distance(philStart, os->source())
                                    < CGAL::squared_distance(philStart, os->target())
                                    ? os->source() : os->target();
            }
            else {
                throw std::runtime_error("strange segment intersection");
            }

            // cerr << "Know Intersection " << i << endl;
            if(isHit) {
                if(CGAL::squared_distance(philStart, intersectionPoint)
                        < CGAL::squared_distance(philStart, minIntersectionPoint)) {
                    // cerr << "new intersection" << i << endl;
                    minIntersectionPoint = intersectionPoint;
                }
            } else {
                // cerr << "min intersection NULL " << i << endl;
                minIntersectionPoint = intersectionPoint;
            }

            // cerr << "end of loop" << i << endl;
            isHit=true;
        }
    }

    // cerr << "return" << endl;
    if(isHit) {
        isPos = minIntersectionPoint;
    }
    // cerr << "return" << endl;
    return isHit;
}

int main() {
    ios_base::sync_with_stdio(false);

    while(true) {
        // cerr << "beginLoop" << endl;
        int numObstacles;
        cin >> numObstacles;
        // cerr << "numObstacles " << numObstacles << endl; 
        if(numObstacles == 0) { 
            return 0;
        }
        K::Point_2 isPos;
        if(hitsObstacle(numObstacles, isPos)) {
            // cerr << "afterreturn" << endl;
            cout << std::setiosflags(std::ios::fixed) << setprecision(0) 
                 << floor_to_double(isPos.x()) 
                 << " " 
                 << floor_to_double(isPos.y()) << endl;
            // cerr << "afterout" << endl;
        } else {
            cout << "no" << endl;
        }
        // cerr << "endloop" << endl;
    }
}
