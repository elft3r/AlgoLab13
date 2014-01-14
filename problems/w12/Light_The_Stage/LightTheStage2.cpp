#include <CGAL/Exact_predicates_inexact_constructions_kernel.h>
#include <CGAL/Delaunay_triangulation_2.h>
#include <iostream>
#include<vector>

typedef CGAL::Exact_predicates_inexact_constructions_kernel K;
typedef CGAL::Delaunay_triangulation_2<K>  Triangulation;
typedef Triangulation::Face_handle Face;
typedef Triangulation::Vertex_handle Vertex;
typedef K::Point_2 P;
typedef K::Segment_2 S;
typedef K::Circle_2 C;

using namespace std;

int midpoint(int lowerBound, int upperBound) {
    return lowerBound + (upperBound-lowerBound)/2;
}

void winnersGivenNumLamps(const vector<P> &lamps, const int numLamps, const int height, 
                                const vector<int> &peopleR, const vector<P> &people, 
                                vector<int> &winners) {
    // construct triangulation
    Triangulation t;
    t.insert(lamps.begin(), lamps.begin()+numLamps+1);

    for(int i=0; i<people.size(); i++) {
        Vertex nearestLamp = t.nearest_vertex(people[i]);

        K::FT maxDist = height;
        maxDist+=peopleR[i];
        maxDist = maxDist * maxDist;
        K::FT dist = CGAL::squared_distance(nearestLamp->point(),people[i]);

        if(dist >= maxDist) winners.push_back(i);
    }

}

void testcase() {
    int numPeople, numLamps;
    cin >> numPeople >> numLamps;

    vector<P> people(numPeople);
    vector<int> peopleR(numPeople);
    for(int i=0; i<numPeople; i++) {
        int x,y,r;
        cin>>x>>y>>r;
        people[i] = P(x,y);
        peopleR[i] = r;
    }

    int height;
    cin >> height;
    vector<P> lamps(numLamps);
    for(int i=0; i<numLamps; i++) {
        int x,y;
        cin>>x>>y;
        lamps[i] = P(x,y);
    }

    vector<int> winners;

    // try all lamps
    winnersGivenNumLamps(lamps, numLamps, height, 
                                peopleR, people, winners);
    if(winners.size() != 0) {
        for(int i=0; i< winners.size();i++) {
            cout<<winners[i]<<" ";
        }
        cout<<endl;
        return;
    }


    //try binary search
    int maxLampsMin = 0;
    int maxLampsMax = lamps.size()-1;

    while (maxLampsMax >= maxLampsMin) {
        winners.clear();

        int maxLamps = midpoint(maxLampsMin, maxLampsMax);

        winnersGivenNumLamps(lamps, maxLamps, height, 
                                peopleR, people, winners);
        
        if(winners.size() == 0) {
            maxLampsMax = maxLamps-1;
        } else {
            maxLampsMin = maxLamps+1;
        }
    }

    winners.clear();
    winnersGivenNumLamps(lamps, maxLampsMax, height, 
                                peopleR, people, winners);
    for(int i=0; i< winners.size();i++) {
        cout<<winners[i]<<" ";
    }
    cout<<endl;

}

int main() {
    ios_base::sync_with_stdio(false);
    int testcases;
    cin >> testcases;
    while(testcases--) testcase();
    
}


