#include <iostream>
#include <limits>
#include <set>
#include <vector>
#include <algorithm>
using namespace std;

class boat {
    public: 
        int length;
        int position;
        boat(int len, int pos) {
            length = len;
            position = pos;
        }
        bool operator< (const boat& other) const {
            return position < other.position;
        }
        bool operator== (const boat& other) const {
            return length == other.length
                && position == other.position;
        }
};

int numBoats(int numWizards, set<boat> &boats) {
    int curPos = numeric_limits<int>::min();
    int numBoats = 0;
    int i = 0;
    while(boats.size() > 0 ) {
        set<boat>::iterator it;
        for(it=boats.begin(); (*it).position < curPos; it++) {
            boats.erase(it);
        }
        // if(it==boats.end()) { 
        //     return numBoats;
        // }
        int bestEnd = numeric_limits<int>::max();
        // boat bestBoat(bestEnd, bestEnd);
        const boat *bestBoat = NULL;
        set<boat>::iterator bestBoatIterator;
        for(it=boats.begin();it!=boats.end(); it++) {
            int begin = ((*it).position - (*it).length > curPos) ? ((*it).position-(*it).length) : curPos;
            int end = begin + (*it).length;
            if(end < bestEnd) {
                bestEnd = end;
                bestBoat = &(*it);
                bestBoatIterator = it;
            }
            if((*it).position > bestEnd) {
                break;
            }
        }
        curPos=bestEnd;
        bestBoat = bestBoat;
        numBoats+=1;
        i++;
        // cout << "boat " << i << " with p:" << bestBoat->position << " l:" << bestBoat->length << " , end is at " << curPos << endl; 
        boats.erase(bestBoatIterator);
    }
    return numBoats;

}

int numBoatsFast(int numWizards, vector<boat> &boats) {
    int i = 0;
    sort(boats.begin(), boats.end()); 
    // int curPos = (*boats.begin()).position;
    int curPos = numeric_limits<int>::min();
    // i++;
    // int numBoats = 1;
    int numBoats = 0;
    while(i < numWizards) {
        for(;i < numWizards && boats[i].position < curPos; i++);
        if(i==numWizards) break;
        int newMaxPos = numeric_limits<int>::max();
        int j = i;
        while(j < numWizards && boats[j].position < newMaxPos){
            int proposed = max(boats[j].position, curPos + boats[j].length);
            if(proposed < newMaxPos) {
                newMaxPos = proposed;
            }
            j++;
        }
        curPos = newMaxPos;
        // cout << "boat " << j-1 << " with p:" << boats[j-1].position << " l:" << boats[j-1].length << " , end is at " << curPos << endl; 
        numBoats++;
        i+=(j-i);
        // i++;
    }
    return numBoats;
}

int main() {
    ios_base::sync_with_stdio(false);
    int testCases;
    cin >> testCases;

    for(int i=0; i < testCases; i++) {
        int numWizards;
        cin >> numWizards;
        vector<boat> boats (numWizards, boat(0,0));
        for(int j=0; j < numWizards; j++) {
            int length;
            int position;
            cin >> length >> position;
            boats[j] = boat(length, position);
        }
        cout << numBoatsFast(numWizards, boats) << endl;
    }
}
