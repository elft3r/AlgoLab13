#include <iostream>
#include <queue>
#include <set>
#include <algorithm>
#include <limits>

using namespace std;

const int MAX = numeric_limits<int>::max();

pair<int,int> optimalPosition(vector<int> &parasols, set<int> &bestPos) {
    int numParasols = parasols.size();
    sort(parasols.begin(), parasols.end());
    
    int bestNumParasols = 0;
    int bestMaxDistToWalk = MAX;
    priority_queue<int, vector<int>, greater<int> > Q;
    for(int i=0; i<numParasols; i++) {
        int rightMostParasol = parasols[i];
        Q.push(rightMostParasol);
        while(Q.top() < rightMostParasol-200) {
            Q.pop();
        }
        int leftMostParasol = Q.top();
        int numCovered = Q.size();
        int span = rightMostParasol-leftMostParasol;
        int barPos = (span/2)+ leftMostParasol;
        int barPos2 = barPos;
        if(span%2!=0) barPos2 = barPos+1;
        int maxDistToWalk = max(rightMostParasol-barPos,
                                -(leftMostParasol-barPos));

        if(numCovered > bestNumParasols) {
           bestNumParasols = numCovered;
           bestMaxDistToWalk = maxDistToWalk; 
           bestPos.clear();
           bestPos.insert(barPos);
           bestPos.insert(barPos2);
        } else if(numCovered == bestNumParasols) {
            if(maxDistToWalk < bestMaxDistToWalk) {
                bestPos.clear();
                bestPos.insert(barPos);
                bestPos.insert(barPos2);
                bestMaxDistToWalk = maxDistToWalk;
            } else if(maxDistToWalk == bestMaxDistToWalk) {
                bestPos.insert(barPos);
                bestPos.insert(barPos2);
            }
        }
    }

    return pair<int,int>(bestNumParasols, bestMaxDistToWalk);
}

int main() {
    ios_base::sync_with_stdio(false);
    int testCases;
    cin >> testCases;
    for(int t=0;t<testCases;t++) {
        int numParasol;
        cin >> numParasol;
        vector<int> parasols(numParasol);
        for(int i=0;i<numParasol;i++) {
            int pos;
            cin >> pos;
            parasols[i] = pos;
        }
        set<int> optimalPos;
        pair<int, int> optimalValues = optimalPosition(parasols, optimalPos);
        cout<<optimalValues.first<<" "<<optimalValues.second<<endl;
        for(set<int>::iterator it = optimalPos.begin();
                it!=optimalPos.end(); it++) {
            if(it!=optimalPos.begin()) {
                cout<<" ";
            }
            cout<<*it;
        }
        cout<<endl;
    }
}
