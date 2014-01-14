#include <iostream>
#include <vector>
#include <boost/config.hpp>
#include <boost/graph/adjacency_list.hpp>
#include <boost/tuple/tuple.hpp>
#include <boost/graph/boyer_myrvold_planar_test.hpp>

using namespace std;
using namespace boost;

typedef adjacency_list<vecS, vecS, undirectedS> Graph;
typedef graph_traits<Graph> Traits;
typedef Traits::vertex_descriptor Vertex;
typedef Traits::edge_descriptor Edge;

int main() {
    ios_base::sync_with_stdio(false);
    int testCases = 0;
    cin >> testCases;
    for(int testCase=0; testCase < testCases; testCase++) {
        int numCoders, numFriends;
        cin >> numCoders >> numFriends;
        Graph g(numCoders+1);

        for(int i=0; i<numFriends; i++) {
            Edge e;
            int u,v;
            cin >> u >> v; 
            tie(e, tuples::ignore) =add_edge(u,v,g);
        }


        //add vertex, connected to all vertices
        for(int i=0; i<numCoders; i++) {
            Edge e;
            tie(e, tuples::ignore) = add_edge(numCoders,i,g);
        }
        if(boyer_myrvold_planarity_test(g)){
            cout << "yes" << endl;
        } else {
            cout << "no" << endl;
        }
    }
    return 0;
}

