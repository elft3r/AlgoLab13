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

double computeSlow(vector<K::Point_2> P) {
    K::FT radius;
    for(vector<K::Point_2>::iterator it = P.begin(); 
            it != P.end(); it++) {
        vector<K::Point_2> tmpP(P.begin(), it);
        tmpP.insert(tmpP.end(), it+1, P.end());

        Min_circle radio(tmpP.begin(), tmpP.end(), true);
        Traits::Circle c = radio.circle();
        K::FT aRadius = c.squared_radius();
        if(it!=P.begin()) {
            if(aRadius < radius)
                radius = aRadius;
        } else {
            radius = aRadius;
        }
    }
    return ceil_to_double(sqrt(radius));
}

//remove only the defining points of the whole minCircle
double computeFast(vector<K::Point_2> P) {
    Min_circle radio(P.begin(), P.end(), true);

    K::FT radius;
    for(int i=0; i<radio.number_of_support_points(); i++) {
        K::Point_2 sp = radio.support_point(i);
        vector<K::Point_2> tmpP(P.begin(), P.end());
        vector<K::Point_2>::iterator toSp = find(tmpP.begin(),tmpP.end(),sp);
        tmpP.erase(toSp);

        Min_circle partRadio(tmpP.begin(), tmpP.end(), true);
        Traits::Circle c = partRadio.circle();
        K::FT aRadius = c.squared_radius();
        if(i!=0) {
            if(aRadius < radius)
                radius = aRadius;
        } else {
            radius = aRadius;
        }

    }

    return ceil_to_double(sqrt(radius));
}

int main() {
    ios_base::sync_with_stdio(false);

    while(true) {
        int numCitizens;
        cin >> numCitizens;
        if(numCitizens == 0)
            break;

        vector<K::Point_2> P(numCitizens);
        for(int i=0; i<numCitizens; i++) {
            double x,y;
            cin >> x >> y;
            P[i] = K::Point_2(x, y);
        }
            cout << std::setiosflags(std::ios::fixed) << setprecision(0) 
                << computeFast(P) << endl;
    }
}
