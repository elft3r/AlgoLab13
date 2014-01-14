#include <iostream>
#include <limits>
#include <set>
#include <vector>
#include <algorithm>

using namespace std;

int numSuperior(vector<pair<int, int> > &interval, int numHumans) {
    sort(interval.begin(), interval.end());
    int rightmost = 0;
    for(int i=0; i<interval.size(); i++) {
        if(interval[i].first > rightmost + 1) 
            return 0;
        else
            rightmost = max(rightmost, interval[i].second);
    }
    if(rightmost < numHumans)
        return 0;

    rightmost = 0;
    int rightMostI = -1;
    int beforeLeft = 0;
    int numSuperiorAliens;
    int beforeI;
    vector<bool> isSuperior(interval.size(), true);

    for(int i=0; i<interval.size(); i++) {
        if(interval[i].first != beforeLeft) {
            if(interval[i].second <= rightmost) {
                isSuperior[i] = false;
                // cout << "right intervall smaller equal rightmost" << endl;
            } 
        } else {
            if(interval[i].second < rightmost) {
                isSuperior[i] = false;
                // cout << "right intervall smaller equal rightmost" << endl;
            } else if(interval[i].second == rightmost) {
                isSuperior[i] = false;
                isSuperior[beforeI] = false;
                //cout << "right intervall equal rightmost";
            } else {
                isSuperior[beforeI] = false;
            }
        }
        if(rightmost <= interval[i].second) {
            rightmost = interval[i].second;
        }
        beforeI = i;
        beforeLeft = interval[i].first;
    }
    numSuperiorAliens=0;
    for(int i=0; i<isSuperior.size(); i++) {
        // cout << "alien: " << interval[i].first << " " << interval[i].second << " ";
        if(isSuperior[i]) {
            numSuperiorAliens++;
            // cout << "is superior!!";
        } 
        // cout << endl;
    }

    return numSuperiorAliens;
}

int main() {
    ios_base::sync_with_stdio(false);
    int testCases;
    cin >> testCases;

    for(int t=0; t < testCases; t++) {
        int numHumans, numAliens;
        cin >> numAliens >> numHumans;

        vector<pair<int, int> > interval;
        for(int i=0; i < numAliens; i++) {
            int intervalLeft;
            int intervalRight;
            cin >> intervalLeft >> intervalRight;
            if(intervalLeft != 0 && intervalRight!=0) {
                pair<int, int> anInterval = pair<int, int>(intervalLeft, intervalRight);
                interval.push_back(anInterval);
            }
        }
        cout << numSuperior(interval, numHumans) << endl;
    }
}


