#include<iostream>
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


void testCase() {
    int width,height;
    cin >> width >> height;

    vector<vector<int> > field = 
        vector<vector<int> >(width, vector<int>(height, -1));
    int curNumTilable = 0;
    Graph g;

    for(int i=0; i<height; i++) {
        for(int j=0; j<width; j++) {
            char place;
            cin>>place;
            if(place == '.') {
                field[j][i] = curNumTilable;
                if(i>0 && field[j][i-1] != -1) {
                    Edge e;
                    tie(e, tuples::ignore)=add_edge(field[j][i-1],curNumTilable,g);
                }
                if(j>0 && field[j-1][i] != -1) {
                    Edge e;
                    tie(e, tuples::ignore)=add_edge(field[j-1][i],curNumTilable,g);
                }
                curNumTilable++;
            }
        }
    }

    vector<Vertex> mate(curNumTilable);
    edmonds_maximum_cardinality_matching(g, &mate[0]);
    // for(i     get(mate,v)
    // graph_traits::null_vertex();
    if(matching_size(g,&mate[0])*2 == curNumTilable) {
        cout<<"yes"<<std::endl;
    } else {
        cout<<"no"<<std::endl;
    }

}

int main() {
    int testCases;
    cin>>testCases;
    while(testCases--) testCase();
    return 0;
}


