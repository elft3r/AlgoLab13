#include <iostream>
#include <vector>
#include <queue>
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
typedef GraphTraits::out_edge_iterator edge_iterator;
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
    int numVertices, numEdges, numSweepers;
    cin>>numVertices>>numEdges>>numSweepers;

    Graph g(numVertices+2);
    EdgeCapacityMap capacity = get(edge_capacity, g);
    ReverseEdgeMap rev_edge = get(edge_reverse, g);
    ResidualCapacityMap res_capacity = get(edge_residual_capacity, g);

    const int SOURCE = numVertices;
    const int SINK = numVertices+1;
    vector<int> startLocations, exitLocations;

    //read start and exit locations
    for(int i=0; i<numSweepers; i++) {
        int startLocation;
        cin>>startLocation;
        startLocations.push_back(startLocation);
    }
    for(int i=0; i<numSweepers; i++) {
        int exitLocation;
        cin>>exitLocation;
        exitLocations.push_back(exitLocation);
    }


    // the following is a incorrect interpretation of the task:
    // correct: if there are sweepers they should clean every corridor just once
    // there seems to be no requirement that corridors have to be cleaned
    // if(numEdges > 0 && numSweepers == 0) {
    //     cout<< "no"<< std::endl;
    //     return;
    // }

    //build flow graph
    for(int i=0; i<numEdges; i++) {
        int u,v;
        cin>>u>>v;
        addFlowEdge(g, capacity, rev_edge, u, v, 1);
        addFlowEdge(g, capacity, rev_edge, v, u, 1);
    }

    // check if all vertices of non-zero degree are reachable from some source
    vector<bool> visited(numVertices,false);
    std::queue<Vertex> Q;
    for(int i=0; i<startLocations.size(); i++) {
       int v = startLocations[i]; 
       visited[v] = true;
       Q.push(v);
       while(Q.size() > 0) {
           v = Q.front(); Q.pop();
           edge_iterator out_i, out_end;
           for(tie(out_i, out_end) = out_edges(v,g);
                   out_i!=out_end; ++out_i) {
               Edge e = *out_i;
               int targ = target(e,g); 
               if(targ<visited.size() && !visited[targ]) {
                   Q.push(targ);
                   visited[targ] = true;
               }
           }
       }
    }

    for(int i=0; i<visited.size(); i++) {
        if(!visited[i] && out_degree(i,g)>0) {
            cout<< "no"<< std::endl;
            return;
        }
    }

    //check eulerian tour
    for(int i=0; i<numVertices; i++) {
        int v = vertex(i, g);
        int numStartLocation = count(startLocations.begin(), startLocations.end(), v);
        int numExitLocation = count(exitLocations.begin(), exitLocations.end(), v);

        if((out_degree(v,g)/2+numStartLocation+numExitLocation)%2 == 1) {
            cout<<"no";
            cout<<std::endl;
            return;
        }
    }

    // add source and sink
    for(int i=0; i<startLocations.size(); i++) {
        addFlowEdge(g, capacity, rev_edge, SOURCE, startLocations[i], 1);
    }
    for(int i=0; i<exitLocations.size(); i++) {
        addFlowEdge(g, capacity, rev_edge, exitLocations[i], SINK, 1);
    }

    //compute flow
    long flow = push_relabel_max_flow(g, SOURCE, SINK);

    if(flow == numSweepers)
        cout<<"yes"<<std::endl;
    else 
        cout<<"no"<<std::endl;

    return;

}

int main() {
    ios_base::sync_with_stdio(false);
    int testCases = 0;
    cin >> testCases;
    while(testCases--) testCase();
}

