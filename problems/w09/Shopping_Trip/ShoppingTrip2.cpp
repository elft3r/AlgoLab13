#include <boost/config.hpp>
#include <boost/tuple/tuple.hpp>
#include <boost/graph/adjacency_list.hpp>
#include <boost/graph/push_relabel_max_flow.hpp>
#include <iostream>
#include <limits>

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

void printGraph(Graph g, EdgeCapacityMap &capacity) {
    graph_traits<Graph>::edge_iterator eiter, eiter_end;
    for (tie(eiter, eiter_end) = edges(g); eiter != eiter_end; ++eiter) {
        if(capacity[*eiter] > 0) {
            int aSource = source(*eiter, g);
            int aTarget = target(*eiter, g);
            std::cout << aSource <<" " 
                " -" << capacity[*eiter] << "-> " << aTarget
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
		int numVertices, numEdges, numStores;
		cin >> numVertices >> numEdges >> numStores;

		Graph g(numVertices);
		EdgeCapacityMap capacity = get(edge_capacity, g);
        ReverseEdgeMap rev_edge = get(edge_reverse, g);
        ResidualCapacityMap res_capacity = get(edge_residual_capacity, g);

		//add sink
		const int SOURCE = 0;
		//addFlowEdge(g, capacity, rev_edge, isStore, SINK, 1);
		const int SINK = numVertices;

		for(int i=0; i<numStores;i++) {
			int isStore;
			cin >> isStore;
			addFlowEdge(g, capacity, rev_edge, isStore, SINK, 1);
		}
		
		for(int i=0; i<numEdges;i++) {
			int u,v;
			cin >> u >> v;
			addFlowEdge(g, capacity, rev_edge, u, v,1);
			addFlowEdge(g, capacity, rev_edge, v, u,1);
		}

		//printGraph(g, capacity) ;

		long flow = push_relabel_max_flow(g, SOURCE, SINK);

		cout << ((flow >= numStores) ? "yes" : "no") << endl;
	}
}
