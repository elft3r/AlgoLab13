#include <CGAL/Exact_predicates_inexact_constructions_kernel.h>
#include <iostream>
#include <set>
#include <vector>
#include <queue>
#include <limits>

using namespace std;

typedef CGAL::Exact_predicates_inexact_constructions_kernel K;
typedef K::Point_2 Point;
typedef pair<Point, Point> Segment;
typedef vector<Point> Triangle;

pair<int,int> minOfVector(vector<int> &v) {
		int min = numeric_limits<int>::max();
		int minPos;
		for(vector<int>::iterator it=v.begin(); it!= v.end(); it++) {
				if(min > *it) {
						min = *it;
						minPos = it-v.begin();
				}
		}
		return pair<int,int>(min,minPos);
}

bool contained(const Segment &s, const Triangle &t) {
    //test whether endpoints of the segments have the same orientation to 
    //the points on the side of the triangle as those points have to each
    //other (same orientation or collinear)

    bool right1 = CGAL::right_turn(t[0], t[1], t[2]);
    bool right2 = CGAL::right_turn(t[2], t[3], t[4]);
    bool right3 = CGAL::right_turn(t[4], t[5], t[0]);

    CGAL::Orientation orient1 = !right1 ? CGAL::RIGHT_TURN : CGAL::LEFT_TURN;
    CGAL::Orientation orient2 = !right2 ? CGAL::RIGHT_TURN : CGAL::LEFT_TURN;
    CGAL::Orientation orient3 = !right3 ? CGAL::RIGHT_TURN : CGAL::LEFT_TURN;


    bool ret = CGAL::orientation(t[0], t[1], s.first) != orient1
                && CGAL::orientation(t[2], t[3], s.first) != orient2
                && CGAL::orientation(t[4], t[5], s.first) != orient3
                && CGAL::orientation(t[0], t[1], s.second) != orient1
                && CGAL::orientation(t[2], t[3], s.second) != orient2
                && CGAL::orientation(t[4], t[5], s.second) != orient3;
    return ret;
}

void myApproach(const vector<Segment>& legs, int numLegs, int numMaps) {
    vector<Triangle > maps(numMaps, Triangle(6));
    int minInterval = numeric_limits<int>::max();
    vector<int> newestCover(numLegs); //map from leg to triangle
    vector<bool> isCovered(numLegs, false); //for the beginning to test whether all are overed
    int numNotCovered = numLegs;

    int curInterval = 0;
    bool mustRecompute = true;
    int curMinIndex = -1;

    for(int i=0;i<numMaps;i++) {
        for(int j=0; j<6; j++) {
            int x,y;
            cin >> x >> y;
            maps[i][j] = Point(x,y);
        }
        
        //test intersection
        for(int j=0; j<legs.size(); j++) {
            if(contained(legs[j], maps[i])) {
                newestCover[j] = i;
                if(j==curMinIndex) mustRecompute = true;
                if(isCovered[j] == false) {
                    isCovered[j] = true;
                    numNotCovered--;
                }
            }
            
        }
        if(numNotCovered > 0)
            continue;

        if(mustRecompute) {
            pair<int,int> resultOfMin = minOfVector(newestCover);
            curInterval = i-resultOfMin.first+1; 
            curMinIndex = resultOfMin.second;
            mustRecompute = false;
            if(curInterval < minInterval)
                minInterval=curInterval;

        }
    }
            
    cout << minInterval << endl;

} 

void benApproach(const vector<Segment> &legs, int numLegs, int numMaps) {
    vector<Triangle > maps(numMaps, Triangle(6));
    int minInterval = numeric_limits<int>::max();
    vector<queue<int> > coveredBy(numLegs); //map from leg to triangle

    for(int i=0;i<numMaps;i++) {
        for(int j=0; j<6; j++) {
            int x,y;
            cin >> x >> y;
            maps[i][j] = Point(x,y);
        }
        
        //test intersection
        for(int j=0; j<legs.size(); j++) {
            if(contained(legs[j], maps[i])) {
                // cout<<"leg "<<j<<" covered by map "<< i<<std::endl;
                coveredBy[j].push(i);
            }
        }
    }

    //find the shortest interval s.t. all legs are covered
    priority_queue<int> maxMapSTAllCovered;
    priority_queue<pair<int,int>,vector<pair<int,int> >, greater<pair<int,int> > > 
        minMapSTAllCovered;

    for(int i=0; i<numLegs; i++) {
        int val = coveredBy[i].front(); coveredBy[i].pop();
        maxMapSTAllCovered.push(val);
        minMapSTAllCovered.push(make_pair(val,i));
    }

    while(true) {
        int minMap, minMapLeg;
        boost::tie(minMap,minMapLeg) 
            = minMapSTAllCovered.top(); minMapSTAllCovered.pop();
        int maxMap = maxMapSTAllCovered.top(); //maxMapSTAllCovered.pop();

        int curInterval = maxMap - minMap + 1;
        minInterval = min(curInterval, minInterval);

        if(coveredBy[minMapLeg].empty()) break;

        int newMapInInterval = coveredBy[minMapLeg].front();
        coveredBy[minMapLeg].pop();
        maxMapSTAllCovered.push(newMapInInterval);
        minMapSTAllCovered.push(make_pair(newMapInInterval, minMapLeg));
    }
            
    cout << minInterval << endl;

}

int main() {
    ios_base::sync_with_stdio(false);

    int numTestCases;
    cin >> numTestCases;

    for(int t=0; t<numTestCases; t++) {
        int numLegs, numMaps;
        cin >> numLegs >> numMaps;

        numLegs-=1;
        vector<Segment> legs(numLegs);
        int x,y;
        cin >> x>>y;
        Point p1(x,y);
        for(int i=0;i<numLegs;i++) {
            cin >> x >> y;
            Point p2(x,y);
            legs[i]=Segment(p1,p2);
            p1 = p2;
        }

        //100points
        // myApproach(legs, numLegs, numMaps);
        //gives only 80 points for me, but interesting sweepline algorithm
        benApproach(legs, numLegs, numMaps);
    }
}


