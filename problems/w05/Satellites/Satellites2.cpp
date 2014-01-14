/* minimum vertex cover */
#include <iostream>
#include <vector>
#include <boost/config.hpp>
#include <boost/graph/adjacency_list.hpp>
#include <boost/tuple/tuple.hpp>
#include <boost/graph/max_cardinality_matching.hpp>

using namespace std;
using namespace boost;

typedef adjacency_list<vecS, vecS, undirectedS > Graph;
typedef graph_traits<Graph> Traits;
typedef Traits::vertex_descriptor Vertex;
typedef Traits::edge_descriptor Edge;
typedef property_map<Graph, vertex_index_t>::type IndexMap;

void minVCoverBipDfs(Graph &g, vector<Vertex> &mate, vector<bool> &visited, 
        int node, vector<bool> &isLeft) {
    visited[node] = true;
    Traits::out_edge_iterator out_i, out_end;
    for (tie(out_i, out_end) = out_edges(node, g); 
           out_i != out_end; ++out_i) {
        Vertex targ = target(*out_i, g);
        int targetNode = get(vertex_index, g)[targ];
        if(visited[targetNode])
            continue;
        if(isLeft[node]) {
            if(mate[node] != targ) {
                minVCoverBipDfs(g, mate, visited, targetNode, isLeft);
            }
        } else {
            if(mate[node] == targ) {
                minVCoverBipDfs(g, mate, visited, targetNode, isLeft);
            }
        }
    }
}


/* takes bipartite graph and a vector that specifies whether
 * a node is on the left side.
 * Returns a vector containing the min vertex cover
 */
vector<int> findMinVertexCoverBipartite(Graph &g, vector<bool> &isLeft) {
    int numNodes = isLeft.size();
    vector<Vertex> mate(numNodes);
    edmonds_maximum_cardinality_matching(g, &mate[0]);

    vector<bool> visited(numNodes, false);
    const Vertex NULL_VERTEX = graph_traits<Graph>::null_vertex();
    for(int i=0;i<numNodes;i++) {
        if(isLeft[i] && mate[i] == NULL_VERTEX) {
            visited[i]=true;
        } 
    }

    for(int i = 0; i < numNodes; i++) {
        if(isLeft[i] && visited[i]) {
            vector<bool> dfsVisited(numNodes, false);
            minVCoverBipDfs(g, mate, visited, i, isLeft);
        }
    }

    vector<int> minVertexCover;
    for(int i = 0; i < numNodes; i++) {
        if(isLeft[i] && !visited[i]) {
            minVertexCover.push_back(i);
        } else if(!isLeft[i] && visited[i]){
            minVertexCover.push_back(i);
        }
    }
    return minVertexCover;
}

int main() {
    ios_base::sync_with_stdio(false);
    int testCases = 0;
    cin >> testCases;
    for(int testCase=0; testCase < testCases; testCase++) {
        int numStations, numSatellites, numLinks;
        cin >> numStations >> numSatellites >> numLinks;
        Graph g(numStations + numSatellites);
        vector<bool> isLeft(numStations+numSatellites,false);
        for(int i=0; i<numLinks; i++) {
            int station, satellite;
            cin >> station >> satellite;
            Edge e;
            tie(e, tuples::ignore) =add_edge(station,numStations+satellite,g);
            isLeft[station] = true;
        }

        vector<int> minVertexCover = findMinVertexCoverBipartite(g, isLeft);

        vector<int> minStations;
        vector<int> minSatellites;

        for(int i=0; i<minVertexCover.size(); i++) {
            int node = minVertexCover[i];
            if(node<numStations)
                minStations.push_back(node);
            else
                minSatellites.push_back(node-numStations);
        }

        cout << minStations.size() << " " << minSatellites.size() << endl;
        for(vector<int>::iterator it=minStations.begin(); it!=minStations.end(); it++) {
            cout << *it << " ";
        }
        for(vector<int>::iterator it=minSatellites.begin(); it!=minSatellites.end(); it++) {
            cout << *it << " ";
        }

        cout << endl;
    }
}

