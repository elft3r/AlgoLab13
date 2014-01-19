#include <iostream>
#include <vector>
#include <boost/config.hpp>
#include <boost/graph/adjacency_list.hpp>
#include <boost/tuple/tuple.hpp>
#include <boost/graph/push_relabel_max_flow.hpp>

using namespace std;
using namespace boost;

// #define DEBUG

#ifdef DEBUG
#define D(x) x
#else 
#define D(x) 
#endif

typedef adjacency_list_traits<vecS,vecS,directedS> Traits;
typedef adjacency_list<vecS, vecS, directedS, no_property,
        property<edge_capacity_t, long,
        property<edge_residual_capacity_t, long,
        property<edge_reverse_t, Traits::edge_descriptor> > > > Graph;

typedef graph_traits<Graph> GraphTraits;
typedef GraphTraits::vertex_descriptor Vertex;
typedef GraphTraits::edge_descriptor Edge;
typedef property_map<Graph,vertex_index_t>::type IndexMap;
typedef property_map<Graph,edge_capacity_t>::type EdgeCapacityMap;
typedef property_map<Graph,edge_residual_capacity_t>::type ResidualCapacityMap;
typedef property_map<Graph,edge_reverse_t>::type ReverseEdgeMap;

void addFlowEdge(Graph &g, EdgeCapacityMap &capacity, ReverseEdgeMap &rev_edge, int u, int v, int c) {
    Edge e, reverseE;
    tie(e,tuples::ignore) = add_edge(u,v,g);
    tie(reverseE,tuples::ignore) = add_edge(v,u,g);
    capacity[e] = c;
    capacity[reverseE] = 0;
    rev_edge[e] = reverseE;
    rev_edge[reverseE] = e;
}

void testCase() {
    // graph is given as a grid
    int gridX, gridY, numSources;
    cin>>gridX>>gridY>>numSources;

    Graph g;
    EdgeCapacityMap capacity = get(edge_capacity, g);
    ReverseEdgeMap rev_edge = get(edge_reverse, g);
    ResidualCapacityMap res_capacity = get(edge_residual_capacity, g);

    //need two "layers" one for in-vertices, the other for out-vertices because
    //the "vertex"-capacity is 1
    //the in-vertex is on the first layer, meaning its index <= numVerticesOnOneLayer
    int numVerticesOnLayer = gridX*gridY;

    const int SOURCE = numVerticesOnLayer*2;
    const int SINK = numVerticesOnLayer*2+1;

    for(int i=0; i<gridX; i++) {
        for(int j=0; j<gridY; j++) {
            int curVertex = numVerticesOnLayer + i*gridY+j;
            // upper row
            if(j>0) {
                addFlowEdge(g, capacity, rev_edge, curVertex, i*gridY+j-1,1); 
            } else {
                addFlowEdge(g, capacity, rev_edge, curVertex, SINK,1); 
            }
            // right column 
            if(i<gridX-1) {
                addFlowEdge(g, capacity, rev_edge, curVertex, (i+1)*gridY+j,1); 
            } else {
                addFlowEdge(g, capacity, rev_edge, curVertex, SINK,1); 
            }
            // lower row
            if(j<gridY-1) {
                addFlowEdge(g, capacity, rev_edge, curVertex, i*gridY+j+1,1); 
            } else {
                addFlowEdge(g, capacity, rev_edge, curVertex, SINK,1); 
            }
            // left column 
            if(i>0) {
                addFlowEdge(g, capacity, rev_edge, curVertex, (i-1)*gridY+j,1); 
            } else {
                addFlowEdge(g, capacity, rev_edge, curVertex, SINK,1); 
            }

            //connect out-vertex to its in-counterpart
            addFlowEdge(g, capacity, rev_edge, curVertex-numVerticesOnLayer, curVertex,1); 
        }
    }

    for(int i=0; i<numSources; i++) {
        int x,y;
        cin>>x>>y;
        addFlowEdge(g, capacity, rev_edge, SOURCE, x*gridY+y,1); 
    }

    D(cout<<"num vertices "<<num_vertices(g)<<endl;)

    long flow = push_relabel_max_flow(g, SOURCE, SINK);

    cout<<flow<<endl;

}

int main() {
    int numCases;
    cin >> numCases;
    while(numCases--) testCase();
    return 0;
}

