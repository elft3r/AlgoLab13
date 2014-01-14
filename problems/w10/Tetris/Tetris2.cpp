#include <iostream>
#include <vector>
#include <boost/config.hpp>
#include <boost/graph/adjacency_list.hpp>
#include <boost/tuple/tuple.hpp>
#include <boost/graph/push_relabel_max_flow.hpp>

using namespace std;
using namespace boost;

typedef adjacency_list_traits<vecS, vecS, directedS> Traits;
typedef adjacency_list<vecS, vecS, directedS, no_property,
  property<edge_capacity_t, long,
  property<edge_residual_capacity_t, long,
  property<edge_reverse_t, Traits::edge_descriptor> > > > Graph;

typedef graph_traits<Graph> GraphTraits;
typedef GraphTraits::vertex_descriptor Vertex;
typedef GraphTraits::edge_descriptor Edge;
typedef property_map<Graph, vertex_index_t>::type IndexMap;
typedef property_map<Graph, edge_capacity_t>::type EdgeCapacityMap;
typedef property_map<Graph, edge_residual_capacity_t>::type ResidualCapacityMap;
typedef property_map<Graph, edge_reverse_t>::type ReverseEdgeMap;

void addFlowEdge(Graph &g, EdgeCapacityMap &capacity, ReverseEdgeMap &rev_edge, int u, int v, int c) {
    Edge e, reverseE;
    tie(e, tuples::ignore) = add_edge(u,v,g);
    tie(reverseE, tuples::ignore) = add_edge(v, u, g);
    capacity[e] = c;
    capacity[reverseE] = 0;
    rev_edge[e] = reverseE;
    rev_edge[reverseE] = e;
}

void testCase() {
    int width, numBricks;
    cin >> width >> numBricks;

    Graph g;
    EdgeCapacityMap capacity = get(edge_capacity, g);
    ReverseEdgeMap rev_edge = get(edge_reverse, g);
    ResidualCapacityMap res_capacity = get(edge_residual_capacity, g);


    // does not work for bordercases
    // for(int i=0; i<numBricks; i++) {
    //     int u,v;
    //     cin>>u>>v;
    //     int from = min(u,v);
    //     int to = max(u,v);
    //     addFlowEdge(g, capacity, rev_edge, 2*from+1, 2*to, 1);
    // }
    // for(int i=1; i<width; i++) {
    //     addFlowEdge(g, capacity, rev_edge, 2*i, 2*i+1, 1);
    // }

    for(int i=0; i<numBricks; i++) {
        int u,v;
        cin>>u>>v;
        int from = min(u,v);
        int to = max(u,v);
        if(from == 0) {
            addFlowEdge(g, capacity, rev_edge, from, to, 1);
        } else {
            addFlowEdge(g, capacity, rev_edge, from+width, to, 1);
        }
    }
    for(int i=1; i<width; i++) {
        addFlowEdge(g, capacity, rev_edge, i, i+width, 1);
    }

    long flow = push_relabel_max_flow(g, 0, width);

    cout<<flow<<std::endl;
}

int main() {
    ios_base::sync_with_stdio(false);
    int testCases = 0;
    cin >> testCases;
    while(testCases--) testCase();
}
