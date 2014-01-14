#include <iostream>
#include <vector>
#include <boost/config.hpp>
#include <boost/graph/adjacency_list.hpp>
#include <boost/tuple/tuple.hpp>
#include <boost/graph/strong_components.hpp>

using namespace std;
using namespace boost;

typedef adjacency_list<vecS, vecS, directedS> Graph;
typedef graph_traits<Graph> Traits;
typedef Traits::edge_descriptor Edge;
typedef Traits::vertex_descriptor Vertex;
typedef Traits::edge_iterator Eit;

int main() {
    ios_base::sync_with_stdio(false);
    int testCases = 0;
    cin >> testCases;
    for(int testCase=0; testCase < testCases; testCase++) {
        int numLocations, numRoads;
        cin >> numLocations >> numRoads;

        Graph g(numLocations);
        vector<int> cost(numLocations);
        for(int i=0;i<numRoads;i++) {
            Edge e;
            int u,v;
            cin >> u >> v; 
            tie(e, tuples::ignore) = add_edge(u-1,v-1,g);
        }
        for(int i=0;i<numLocations;i++) {
            int c;
            cin >> c;
            cost[i] = c; 
        }

        vector<int> componentMap(numLocations);
        int numComponents = strong_components(g, &componentMap[0]); 
        vector<int> componentCosts(numComponents, 100);

        // cout << "numcomp " << numComponents << endl;

        for(int i=0; i<numLocations; i++) {
            if(cost[i] < componentCosts[componentMap[i]]) {
                componentCosts[componentMap[i]] = cost[i];
            }
        }

        Eit ei, ei_end;
        for(tie(ei, ei_end) = edges(g); ei != ei_end; ++ei) {
            Vertex sV = source(*ei,g);
            Vertex tV = target(*ei,g);

            if(componentMap[sV] != componentMap[tV]) {
                componentCosts[componentMap[tV]] = 0;
            }
        }

        int minPStation = 0;
        for(int i=0;i<numComponents;i++) {
            minPStation += componentCosts[i];
        }
        cout << minPStation << endl;
    }
}


