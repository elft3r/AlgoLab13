#include <iostream>
#include <vector>
#include <queue>
#include <limits>

using namespace std;

// #define DEBUG
#ifdef DEBUG
#define D(x) x
#else 
#define D(x) 
#endif

typedef vector<vector<int> > AdjacencyList;

const int MAX = numeric_limits<int>::max();

int shortestPath(const AdjacencyList &adjacencyList, int start, int end, vector<int> &predecessorMap) {
    queue<int> Q;
    vector<bool> visited(adjacencyList.size(),false);
    vector<int> distance(adjacencyList.size());
    Q.push(start);
    visited[start] = true;
    distance[start] = 0;
    if(start == end) {
        return 0;
    }

    while(!Q.empty()) {
        int v = Q.front();
        Q.pop();
        for(int i =0; i< adjacencyList[v].size(); i++) {
            int childV = adjacencyList[v][i];
            if(!visited[childV]) {
                visited[childV] = true;
                predecessorMap[childV] = v;
                distance[childV] = distance[v] + 1;
                if(childV == end) { 
                    return distance[childV];
                } else {
                    Q.push(childV);
                }
            }
        }
    }
    return -1;
}

void testCase() {
    int numEdges, numVertices;
    cin>>numVertices>>numEdges;
    int start, end;
    cin>>start>>end;
    start-=1; end-=1;

    AdjacencyList adjacencyList(numVertices, vector<int>(0));
    for(int i=0; i<numEdges; i++) {
        int u,v;
        cin>>u>>v;
        adjacencyList[u-1].push_back(v-1);
    }

    vector<int> predecessorMap(numVertices, -1);
    int secondShortestPathLength = MAX; 
    int shortestPathLength = shortestPath(adjacencyList, start, end, predecessorMap);
    D(cout<<"shortest: "<<shortestPathLength<<endl;)

    if(shortestPathLength == -1) {
        cout<<"no"<<endl;
        D(cout<<"reason1"<<endl;)
        return;
    }

    vector<int> path;
    int curVertex = end;
    D(cout<<"backwards path: ";)
    while(curVertex != -1) {
        D(cout<<curVertex <<" ";)
        path.push_back(curVertex);
        curVertex = predecessorMap[curVertex];
    }
    D(cout<<endl;)

    for(int i = 0; i<path.size(); i++) {
        int v = path[i];
        int pathLenSoFar = path.size() - i - 1;
        for(int j =0; j< adjacencyList[v].size(); j++) {
            int childV = adjacencyList[v][j];
            if(i==0 || childV != path[i-1]) {
                int shortestPathFromChild = shortestPath(adjacencyList, childV , end, predecessorMap);
                D(cout<<"vertex "<<v<< " path len so far on shortest: " << pathLenSoFar <<"; shortest path from " << childV << " is " << shortestPathFromChild << endl;)
                if(shortestPathFromChild >= 0) {
                    secondShortestPathLength 
                        = min(secondShortestPathLength, pathLenSoFar + 1 + shortestPathFromChild);
                }
            }
        }
    }

    if(secondShortestPathLength==MAX) {
        // secondShortestPathLength = shortestPathLength;
        cout<<"no"<<endl;
        D(cout<<"reason2"<<endl;)
        return;
    }

    cout<<secondShortestPathLength<<endl;

}

int main() {
    int testCases;
    cin>>testCases;
    while(testCases--) testCase();
    return 0;
}
