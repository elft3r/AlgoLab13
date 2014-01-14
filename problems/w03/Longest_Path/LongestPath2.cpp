#include <iostream>
#include <vector>

using namespace std;

pair<int, int> longestPath(int node, int ancestor, const vector<vector<int> > &adjList) {
    const vector<int> *neighbors = &adjList[node];
    if(neighbors->size() == 1 && ancestor != -1) {
        return pair<int, int>(1,1);
    } else if (neighbors->size() == 2 && ancestor != -1) {
        for(vector<int>::const_iterator it = neighbors->begin(); it != neighbors->end(); it++) {
            if(*it != ancestor) {
                pair<int, int> child = longestPath(*it, node, adjList);
                if(child.first + 1 > child.second) {
                    return pair<int, int>(child.first+1, child.first+1);
                } else {
                    return pair<int, int>(child.first+1, child.second);
                }
            }
        }
    } else {
        int maxHeight = 0;
        int secondMaxHeight = 0;
        int maxPath = 0;
        for(vector<int>::const_iterator it = neighbors->begin(); it != neighbors->end(); it++) {
            if(*it != ancestor) {
                pair<int, int> child = longestPath(*it, node, adjList);
                if(child.first > maxHeight) {
                    secondMaxHeight = maxHeight;
                    maxHeight = child.first;
                } else if(child.first > secondMaxHeight) {
                    secondMaxHeight = child.first;
                }
                if(child.second > maxPath) {
                    maxPath = child.second;
                }
            }
        }
        int altMaxPath = maxHeight+secondMaxHeight+1;
        if(altMaxPath > maxPath) {
            return pair<int, int>(maxHeight+1, altMaxPath);
        } else {
            return pair<int, int>(maxHeight+1, maxPath);
        }
    }
}

int main() {
    ios_base::sync_with_stdio(false);
    int testCases = 0;
    cin >> testCases;
    for(int testCase=0; testCase < testCases; testCase++) {
        int numVertices;
        cin >> numVertices;
        vector<vector<int> > adjList(numVertices);
        int accArray[numVertices];

        for(int i=0; i<numVertices-1; i++) {
            int v1, v2;
            cin >> v1 >> v2;
            adjList[v1].push_back(v2);
            adjList[v2].push_back(v1);
            accArray[i] = -1;
        }
        accArray[numVertices-1] = -1;
        cout << longestPath(0, -1, adjList).second << endl;
    }
    return 0;
}

