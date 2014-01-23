#include <iostream>
#include <vector>
#include <queue>
#include <boost/config.hpp>
#include <boost/graph/adjacency_list.hpp>
#include <boost/tuple/tuple.hpp>
#include <boost/graph/biconnected_components.hpp>
#include <boost/graph/connected_components.hpp>

using namespace std;
using namespace boost;

// #define DEBUG 

#ifdef DEBUG
#define D(x) x
#else
#define D(x)
#endif

namespace boost {
    struct edge_component_t {
        enum {
            num=555
        };
        typedef edge_property_tag kind;
    }
    edge_component;
}

typedef adjacency_list<vecS,vecS,undirectedS,no_property,
            property<edge_component_t,std::size_t> > Graph;
typedef graph_traits<Graph> Traits;
typedef Traits::vertex_descriptor Vertex;
typedef Traits::edge_descriptor Edge;
typedef property_map<Graph, vertex_index_t>::type IndexMap;
typedef property_map<Graph, edge_component_t>::type ComponentMap;

vector<int> discoverTimeToVertex;

// Custom visitor used to record DFS order.
struct CustomVisitor : public default_dfs_visitor {
	void discover_vertex(int u, const Graph& G) {
		discoverTimeToVertex.push_back(u);
	}
};

int newColor(int col1, int col2) {
    int newCol = (col1+1)%3;
    if(newCol == col2) {
        newCol = (newCol+1)%3;
    }
    return newCol;
}

void testCase() {
    int numVertices, numEdges;
    cin>>numVertices>>numEdges;

    Graph g(numVertices);
    ComponentMap component = get(edge_component,g);

    for(int i=0; i<numEdges; i++) {
        Edge e;
        int u,v;
        cin>>u>>v;
        tie(e,tuples::ignore) = add_edge(u,v,g);
    }

    //is connected?
    vector<int> con_component(numVertices);
    size_t num_comps = connected_components(g, &con_component[0]);
    if(num_comps!=1) {
        cout<<"no"<<endl;
        return;
    }

    //is biconnected?
    vector<Vertex> low(numVertices);
    vector<Vertex> vertexToDiscoverTime(numVertices);
    vector<Vertex> predecessor(numVertices);
    discoverTimeToVertex.clear();

    size_t num_bicomps 
        = biconnected_components(g,component,
                                    lowpoint_map(&low[0])
                                    .discover_time_map(&vertexToDiscoverTime[0])
                                    .predecessor_map(&predecessor[0])
                                    .visitor(CustomVisitor()));
    if(num_bicomps!=1) {
        cout<<"no"<<endl;
        return;
    } 

    cout<<"yes"<<endl;
    vector<int> color(numVertices, 0);
    vector<bool> visited(numVertices, false);

    D(for(int i=0; i<numVertices; i++) {
        cout<<i << " " << vertexToDiscoverTime[i]<< " " << predecessor[i] << " " << low[i] << endl;
    })

    for(int i=1; i< numVertices; i++) {
        int v = discoverTimeToVertex[i];
        color[v] = newColor(color[predecessor[v]], color[discoverTimeToVertex[low[v]-1]]);
    }

    vector<vector<int> > colors(3);
    for(int i=0; i<numVertices; i++) {
        colors[color[i]].push_back(i);
    }

    for(int i=0; i<3; i++) {
        cout<<colors[i].size();
        for(int j=0; j<colors[i].size();j++) {
            cout<< " ";
            cout<<colors[i][j];
        }
        cout<<endl;
    }

}

int main() {
    ios_base::sync_with_stdio(false);
    int testCases;
    cin>>testCases;
    while(testCases--) testCase();
}
