#include <boost/config.hpp>
#include <boost/tuple/tuple.hpp>
#include <boost/graph/adjacency_list.hpp>
#include <boost/graph/push_relabel_max_flow.hpp>
#include <iostream>
#include <limits>
#include <queue>

using namespace std;
using namespace boost;

typedef adjacency_list_traits<vecS, vecS, directedS> Traits;
typedef adjacency_list<vecS, vecS, directedS, no_property,
  property<edge_capacity_t, long,
  property<edge_residual_capacity_t, long,
  property<edge_reverse_t, Traits::edge_descriptor> > > > Graph;
typedef property_map<Graph, edge_capacity_t>::type EdgeCapacityMap;
typedef property_map<Graph, edge_residual_capacity_t>::type ResidualCapacityMap;
typedef property_map<Graph, edge_reverse_t>::type ReverseEdgeMap;
typedef property_map<Graph, vertex_index_t>::type IndexMap;

typedef graph_traits<Graph>::vertex_descriptor Vertex;
typedef graph_traits<Graph>::edge_descriptor Edge;
typedef graph_traits<Graph> GraphTraits;

void printGraph(Graph g, EdgeCapacityMap &capacity, vector<pair<int, int> > &locProps) {
    graph_traits<Graph>::edge_iterator eiter, eiter_end;
    for (tie(eiter, eiter_end) = edges(g); eiter != eiter_end; ++eiter) {
        if(capacity[*eiter] > 0) {
            int aSource = source(*eiter, g);
            int aTarget = target(*eiter, g);
            std::cout << aSource <<": (" 
                << locProps[aSource].first<< "|" << locProps[aSource].second << ")" <<
                " -" << capacity[*eiter] << "-> " << aTarget
                << ": (" << locProps[aTarget].first<< "|" << locProps[aTarget].second << ")"
              << std::endl;
        }
    }
}

void addFlowEdge(Graph &g, EdgeCapacityMap &capacity, ReverseEdgeMap &rev_edge, int u, int v, int c) {
    Edge e, reverseE;
    tie(e, tuples::ignore) = add_edge(u,v,g);
    tie(reverseE, tuples::ignore) = add_edge(v, u, g);
    capacity[e] = c;
    capacity[reverseE] = 0;
    rev_edge[e] = reverseE;
    rev_edge[reverseE] = e;
}

int main() {
    ios_base::sync_with_stdio(false);
    int testCases = 0;
    cin >> testCases;
    for(int testCase=0; testCase < testCases; testCase++) {
        int numVertices, numEdges;
        cin >> numVertices >> numEdges;
        vector<pair<int, int> > locProp(numVertices, pair<int, int>(0,0));

        for(int i=0; i<numVertices; i++) {
            int numStationed,numNeeded;
            cin >> numStationed >> numNeeded;
            locProp[i] = pair<int, int>(numStationed, numNeeded);
        }

        Graph g(numVertices);
        EdgeCapacityMap capacity = get(edge_capacity, g);
        ReverseEdgeMap rev_edge = get(edge_reverse, g);
        ResidualCapacityMap res_capacity = get(edge_residual_capacity, g);
        for(int i=0; i<numEdges; i++) {
            int from,to, minPassing, maxPassing;
            cin >> from >> to >> minPassing >> maxPassing;

            //force soldiers to walk along
            locProp[from].first = locProp[from].first - minPassing;
            locProp[to].first = locProp[to].first + minPassing;
            maxPassing-=minPassing;

            //build actual graph
            addFlowEdge(g, capacity, rev_edge, from, to, maxPassing);
        }

        // printGraph(g, capacity, locProp);

        const int SOURCE = numVertices;
        const int SINK = numVertices+1;
        int numNonForcedSoldiers=0;
        //add source and sink
        for(int i=0; i<numVertices; i++) {
            int soldiersStationed = locProp[i].first;
            int soldiersNeeded = locProp[i].second;
            if(soldiersStationed > 0) {
                addFlowEdge(g, capacity, rev_edge, SOURCE, i, soldiersStationed);
                addFlowEdge(g, capacity, rev_edge, i, SINK, soldiersNeeded);
            } else {
                soldiersNeeded-=soldiersStationed;
                addFlowEdge(g, capacity, rev_edge, i, SINK, soldiersNeeded);
            }
            numNonForcedSoldiers+=soldiersNeeded;
        }
        // cout << "after adding source and sink:" << endl; 
        // printGraph(g, capacity, locProp);
        
        //compute maxFlow
        long flow = push_relabel_max_flow(g, SOURCE, SINK);
        // long flow = 0;
        
        // cout << numNonForcedSoldiers << endl;
        if(flow==numNonForcedSoldiers) {
            cout << "yes";
        } else {
           cout << "no"; 
        }
        cout << endl;
    }
}

