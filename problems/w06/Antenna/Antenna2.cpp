#include <CGAL/Exact_predicates_exact_constructions_kernel_with_sqrt.h>
#include <CGAL/Min_circle_2.h>
#include <CGAL/Min_circle_2_traits_2.h>
#include <iostream>

using namespace std;

typedef CGAL::Exact_predicates_exact_constructions_kernel_with_sqrt K;
typedef CGAL::Min_circle_2_traits_2<K> Traits;
typedef CGAL::Min_circle_2<Traits> Min_circle;

double floor_to_double(const K::FT& x) {
    double a = std::floor(CGAL::to_double(x));
    while(a>x) a-=1;
    while(a+1<=x) a+=1;
    return a;
}

double ceil_to_double(const K::FT& x) {
    double a = std::ceil(CGAL::to_double(x));
    while(a<x) a+=1;
    while(a-1>=x) a-=1;
    return a;
}


int main() {
    ios_base::sync_with_stdio(false);

    while(true) {
        int numCitizens;
        cin >> numCitizens;
        if(numCitizens == 0)
            break;

        K::Point_2 P[numCitizens];
        for(int i=0; i<numCitizens; i++) {
            double x,y;
            cin >> x >> y;
            P[i] = K::Point_2(x, y);
        }
        Min_circle radio(P, P+numCitizens, true);
        Traits::Circle c = radio.circle();
        K::FT radius = sqrt(c.squared_radius());
        cout << std::setiosflags(std::ios::fixed) << setprecision(0) << ceil_to_double(radius) << endl;
        // cout << std::round(CGAL::to_double(radius)) << endl;
    }
}

