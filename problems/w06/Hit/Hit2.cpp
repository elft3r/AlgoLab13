#include <CGAL/Exact_predicates_exact_constructions_kernel.h>
#include <iostream>

using namespace std;

typedef CGAL::Exact_predicates_exact_constructions_kernel K;


bool hitsObstacle(int numObstacles) {

    bool ret = false;
    double x, y, a, b;
    cin >> x >> y >> a >> b;
    K::Ray_2 phil(K::Point_2(x,y), K::Point_2(a,b));
    // cout << "Phil " << phil << endl;

    for(int i=0; i<numObstacles; i++) {
        double r,s,t,u;
        cin >> r >> s >> t >> u;
        K::Segment_2 obstacle(K::Point_2(r,s), K::Point_2(t,u));
        if(!ret && CGAL::do_intersect(phil, obstacle)) {
            ret=true;
        }
    }

    return ret;
}

int main() {
    ios_base::sync_with_stdio(false);

    while(true) {
        int numObstacles;
        cin >> numObstacles;
        // cout << "numObstacles " << numObstacles << endl; 
        if(numObstacles == 0) { 
            return 0;
        }
        if(hitsObstacle(numObstacles)) {
            cout << "yes" << endl;
        } else {
            cout << "no" << endl;
        }
    }
}
