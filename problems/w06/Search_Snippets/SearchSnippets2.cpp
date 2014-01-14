#include <iostream>
#include <queue>
#include <vector>
#include <limits>

using namespace std;

const int MAX = numeric_limits<int>::max();

pair<int,int> minOfVector(vector<int> &v) {
		int min = MAX;
		int minPos;
		for(vector<int>::iterator it=v.begin(); it!= v.end(); it++) {
				if(min > *it) {
						min = *it;
						minPos = it-v.begin();
				}
		}
		return pair<int,int>(min,minPos);
}

int computeInterval(priority_queue<pair<int,int> > &Q, int numWords) {
		vector<int> currentPosition(numWords);
		vector<bool> hasObserved(numWords, false);
		int numNotObserved = numWords;

		int minInterval = MAX;
		int interval1 = 0;
		int interval2 = 0;
		while(Q.size() > 0){
				pair<int, int> occurence = Q.top();
				Q.pop();
				int word = occurence.second;
				int pos = -occurence.first;
//				cout << "current wordpos of " << word << " is " << pos << endl;

				currentPosition[word] = pos;
				if(hasObserved[word] == false) {
						hasObserved[word] = true;
						numNotObserved--;
				}
				if(numNotObserved > 0)
						continue;

				pair<int,int> aMinOfVector = minOfVector(currentPosition);
				int curInterval = pos - aMinOfVector.first +1;

				if(curInterval<minInterval) {
						minInterval=curInterval;
						interval1 =aMinOfVector.second;
						interval2 =pos;
//						cout << minInterval << " between ["<<  interval1<<  ","<<  interval2<<  "]" << endl;
				}
		}

		return minInterval;
}

int main() {
	ios_base::sync_with_stdio(false);
	int testCases;
	cin >> testCases;
	for(int i=0; i<testCases; i++) {
		int numWords;
		cin >> numWords;
		int numAppears[numWords];
		for(int j=0;j<numWords; j++) {
				int num;
				cin >> num;
				numAppears[j] = num;
		}

		priority_queue<pair<int,int> > Q;

		for(int j=0;j<numWords; j++) {
				for(int k=0; k<numAppears[j]; k++) {
						pair<int, int> occurence;
						int position;
						cin>>position;
						Q.push(pair<int,int>(-position, j));
				}
		}
		cout << computeInterval(Q, numWords) << endl;

		}
}
