#include <CGAL/Exact_predicates_exact_constructions_kernel.h>
#include <CGAL/Min_circle_2.h>
#include <CGAL/Min_circle_2_traits_2.h>
#include <iostream>

using namespace std;

typedef CGAL::Exact_predicates_exact_constructions_kernel K;
typedef CGAL::Min_circle_2_traits_2<K> Traits;
typedef CGAL::Min_circle_2<Traits> Min_circle;
typedef K::Point_2 P;
typedef K::Segment_2 S;

double ceil_to_double(const K::FT& x) {
    double a = std::ceil(CGAL::to_double(x));
    while(a<x) a+=1;
    while(a-1>=x) a-=1;
    return a;
}

struct CityComparator{
    P mainCity;
    bool operator()(P x, P y) {
        K::FT d1 = S(mainCity, x).squared_length();
        K::FT d2 = S(mainCity, y).squared_length();
        return(d1 > d2);
    }

};

int main() {
    ios_base::sync_with_stdio(false);
    int testCases;
    cin>>testCases;

    for(int t=0; t<testCases;t++) {
        int numCities;
        cin>>numCities;
        vector<P> cities;


        for(int i=0; i<numCities;i++) {
            int x,y;
            cin>>x>>y;
            P p(x,y);
            cities.push_back(p);
        }

        P mainCity = cities[0];
    
        CityComparator comp;
        comp.mainCity = mainCity;

        sort(cities.begin(), cities.end(), comp);
        //sort from highest distance to lowest

        K::FT otherRadius;
        K::FT mainCityRadius;
        K::FT beforeMainCityRadius;
        Min_circle otherRadio;
        for(vector<P>::iterator i = cities.begin(); 
                i!=cities.end(); i++) {

            // cout << "begin " << *(cities.begin()) << endl;
            // cout << "i " << *i << endl;
            // cout << "end " << *(cities.end()-1) << endl;
            mainCityRadius = S(*(i+1), mainCity).squared_length();

            otherRadio.insert(*i);
            Traits::Circle otherCircle = otherRadio.circle();
            otherRadius = otherCircle.squared_radius();

            // cout << fixed<<setprecision(2)<<"radii " << mainCityRadius << " " << otherRadius << ":" << otherRadio.number_of_points()<< endl;

            if(otherRadius >= mainCityRadius)
                break;
            beforeMainCityRadius=mainCityRadius;
        }
        cout<<fixed<<setprecision(0)<<ceil_to_double(min(beforeMainCityRadius,otherRadius))<<endl;
            
    }

}

