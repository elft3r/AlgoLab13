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

void printGraph(Graph g) {
    graph_traits<Graph>::edge_iterator eiter, eiter_end;
    for (tie(eiter, eiter_end) = edges(g); eiter != eiter_end; ++eiter) {
        std::cout << source(*eiter, g) << " <--> " << target(*eiter, g)
          << std::endl;
    }
}

void findCut(Graph &g, EdgeCapacityMap &capacity, 
                ResidualCapacityMap &res_capacity,  
                std::set<Vertex> &ret, 
                Vertex sourceV, Vertex sinkV) {
    IndexMap index = get(vertex_index, g);
    vector<bool> visited(num_vertices(g), false);
    std::queue<Vertex> bfs_queue;

    //get start vertex
    Vertex startV = sourceV;

    visited[index[startV]] = true;
    bfs_queue.push(startV);
    ret.insert(startV);

    while (!bfs_queue.empty()) {
        Vertex v = bfs_queue.front();
        bfs_queue.pop();

        graph_traits<Graph>::adjacency_iterator adjV, adEnd;
        GraphTraits::out_edge_iterator out_i, out_end;
        for (tie(out_i, out_end) = out_edges(v, g); 
           out_i != out_end; ++out_i) {
            Edge e = *out_i;
            Vertex src = source(e, g), targ = target(e, g);
            int flow = capacity[e] - res_capacity[e];
            // cout << "edge " << src << " " << targ << " c:" << capacity[e] << " f:" << flow << endl;
            int vIndex = index[targ];
            if(flow < capacity[e] && !visited[vIndex]) {
                bfs_queue.push(targ);
                visited[vIndex] = true;
                ret.insert(targ);
            }
        }
    }
}

int cutCapacity(Graph &g, set<Vertex> &sSet, EdgeCapacityMap &capacity) {
    int cutCapacity=0;
    for(set<Vertex>::iterator it=sSet.begin(); 
                it != sSet.end(); it++) {
        Vertex v = *it;
        GraphTraits::out_edge_iterator out_i, out_end;
        for (tie(out_i, out_end) = out_edges(v, g); 
           out_i != out_end; ++out_i) {
            Edge e = *out_i;
            Vertex targ = target(e, g);
            if(find(sSet.begin(), sSet.end(), targ) == sSet.end()) {
                //in T set
                cutCapacity+=capacity[e];
            }
        }
    }
    return cutCapacity;
}

pair<int, int> findBestSourceSinkSlow(Graph &g, int numFigures) {
    pair<int, int> bestSourceSink;
    int minMaxFlow = numeric_limits<int>::max();
    for(int i=0; i<numFigures; i++) {
        for(int j=i+1; j<numFigures; j++) {
            long flow = push_relabel_max_flow(g, i, j);
            if(flow < minMaxFlow) {
                bestSourceSink = pair<int, int>(i, j);
                minMaxFlow = flow;
            }
            flow = push_relabel_max_flow(g, j, i);
            if(flow < minMaxFlow) {
                bestSourceSink = pair<int, int>(j, i);
                minMaxFlow = flow;
            }
        }
    }
    return bestSourceSink;
}


pair<int, int> findBestSourceSinkFast(Graph &g, int numFigures) {
    pair<int, int> bestSourceSink;
    int minMaxFlow = numeric_limits<int>::max();
    int u = 0;
    for(int i=1; i<numFigures; i++) {
        long flow = push_relabel_max_flow(g, u, i);
        if(flow < minMaxFlow) {
            bestSourceSink = pair<int, int>(u,i);
            minMaxFlow = flow;
        }
    }
    for(int i=1; i<numFigures; i++) {
        long flow = push_relabel_max_flow(g, i, u);
        if(flow < minMaxFlow) {
            bestSourceSink = pair<int, int>(i,u);
            minMaxFlow = flow;
        }
    }
    return bestSourceSink;
}
int main() {
    ios_base::sync_with_stdio(false);
    int testCases = 0;
    cin >> testCases;
    for(int testCase=0; testCase < testCases; testCase++) {
        int numFigures, numLimbs;
        cin >> numFigures >> numLimbs;
        Graph g(numFigures);
        EdgeCapacityMap capacity = get(edge_capacity, g);
        ReverseEdgeMap rev_edge = get(edge_reverse, g);
        ResidualCapacityMap res_capacity = get(edge_residual_capacity, g);

        for(int i=0; i<numLimbs;i++) {
            int u, v, c;
            cin >> u >> v >> c;
            Edge e, reverseE;
            tie(e, tuples::ignore) = add_edge(u,v,g);
            tie(reverseE, tuples::ignore) = add_edge(v, u, g);
            capacity[e] = c;
            capacity[reverseE] = 0;
            rev_edge[e] = reverseE;
            rev_edge[reverseE] = e;
        }

        pair<int, int> bestSourceSink = findBestSourceSinkFast(g, numFigures);
        int sourceV = bestSourceSink.first;
        int sinkV = bestSourceSink.second;
        long flow = push_relabel_max_flow(g, sourceV, sinkV);
        cout << flow << endl;
        std::set<Vertex> oneSet;
        findCut(g, capacity, res_capacity, oneSet, sourceV, sinkV);
        IndexMap index = get(vertex_index, g);
        cout << oneSet.size() << " ";
        for(set<Vertex>::iterator it=oneSet.begin(); 
                it != oneSet.end(); it++) {
            cout << index[*it] << " ";
        }
        cout << endl;
        // cout << "cut capacity " << cutCapacity(g, oneSet, capacity) << endl;
        
    }
}



