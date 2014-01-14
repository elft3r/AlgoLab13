#include <boost/config.hpp>
#include <boost/tuple/tuple.hpp>
#include <boost/graph/adjacency_list.hpp>
#include <boost/graph/max_cardinality_matching.hpp>
#include <iostream>
#include <set>

using namespace std;
using namespace boost;

typedef adjacency_list<vecS, vecS, undirectedS > Graph;
typedef graph_traits<Graph> Traits;
typedef Traits::vertex_descriptor Vertex;
typedef Traits::edge_descriptor Edge;
typedef property_map<Graph, vertex_index_t>::type IndexMap;

int main() {
    ios_base::sync_with_stdio(false);
    int testCases = 0;
    cin >> testCases;
    for(int testCase=0; testCase < testCases; testCase++) {
        int numStudents, numChars, numCommChar;
        cin >> numStudents >> numChars >> numCommChar;

        Graph g(numStudents);
        vector<set<string> > chars(numStudents, set<string>());

        // cout << "testset:" << testCase << endl;
        for(int i=0; i < numStudents; i++) {
            for(int j=0; j < numChars; j++) {
                string characteristic;
                cin >> characteristic;
                chars[i].insert(characteristic);
            }
            for(int j=0; j < i; j++) {
                std::vector<string> inters;
                std::set_intersection(chars[i].begin(), chars[i].end(),
                                        chars[j].begin(), chars[j].end(),
                                        std::back_inserter(inters));
                if(inters.size() > numCommChar) {
                    // cout << "v:" << i << " u:" << j << endl;
                    //add edge
                    Edge e;
                    tie(e, tuples::ignore)=add_edge(i,j,g);
                }
            }
        }
        vector<Vertex> mate(numStudents);
        bool foundMatching = checked_edmonds_maximum_cardinality_matching(g, &mate[0]);
        if(matching_size(g, &mate[0])*2 >= numStudents) {
            cout << "not optimal" << endl;
        } else {
            cout << "optimal" << endl;
        }
    }
}

