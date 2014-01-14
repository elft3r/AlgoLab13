#include <iostream>
#include <vector>
#include <queue>
#include <limits>
#include <boost/config.hpp>
#include <boost/graph/adjacency_list.hpp>
#include <boost/graph/dijkstra_shortest_paths.hpp>

using namespace std;
using namespace boost;

typedef adjacency_list<vecS, vecS, directedS, no_property,
property<edge_weight_t, int> > Graph;
typedef graph_traits<Graph>::vertex_descriptor Vertex;
typedef graph_traits<Graph>::edge_descriptor Edge;

void testCase() {
    int numVertices, numEdges;
    cin>>numVertices>>numEdges;
    Graph g(numVertices*4);
    // Graph g2(numVertices);

    int source,target;
    cin>>source>>target;

    target = numVertices*3 + target;

    property_map<Graph, edge_weight_t>::type weightMap = get(edge_weight, g);
    // property_map<Graph, edge_weight_t>::type weightMap2 = get(edge_weight, g2);
    for(int i = 0; i < numEdges; ++i)
    {
        int u,v,w;
        cin>>u>>v>>w;
        bool success;

        Edge e;
        if(w%2==1) {
            tie(e, success) = add_edge(u, v+3*numVertices, g);
            weightMap[e] = w;
            tie(e, success) = add_edge(u+numVertices, v+2*numVertices, g);
            weightMap[e] = w;
            tie(e, success) = add_edge(u+2*numVertices, v+numVertices, g);
            weightMap[e] = w;
            tie(e, success) = add_edge(u+3*numVertices, v, g);
            weightMap[e] = w;

        } else {
            tie(e, success) = add_edge(u, v+2*numVertices, g);
            weightMap[e] = w;
            tie(e, success) = add_edge(u+numVertices, v+3*numVertices, g);
            weightMap[e] = w;
            tie(e, success) = add_edge(u+2*numVertices, v, g);
            weightMap[e] = w;
            tie(e, success) = add_edge(u+3*numVertices, v+numVertices, g);
            weightMap[e] = w;


        }
                // tie(e, success) = add_edge(v, u, g2);
        // weightMap2[e] = w;
    }

    std::vector<Vertex> predecessors(num_vertices(g));
    std::vector<int> distancesFromSource(num_vertices(g));

    dijkstra_shortest_paths(g, source,
        predecessor_map(&predecessors[0]).distance_map(&distancesFromSource[0]));

    if(distancesFromSource[target] < numeric_limits<int>::max()) {
        cout<<distancesFromSource[target]<<endl;
    } else {
        cout<<"no"<<endl;
    }

    // std::vector<Vertex> predecessors2(num_vertices(g));
    // std::vector<int> distancesFromTarget(num_vertices(g));
    // dijkstra_shortest_paths(g2, target,
    //     predecessor_map(&predecessors2[0]).distance_map(&distancesFromTarget[0]));


    // int min = numeric_limits<int>::max();
    // int minI = -1;
    // for(int i = 0; i<numVertices;i++) {
    //     int tempMin = distancesFromSource[i] + distancesFromTarget[i];
    //     // cout<<distancesFromSource[i] <<" "<< distancesFromTarget[i]<<" "<<tempMin<<endl;
    //     if(tempMin%2==1 && tempMin < min) {
    //         min = tempMin;
    //         minI = i;
    //     }
    // }

    // if(min == numeric_limits<int>::max()) {
    //     cout<<"no";
    // } else {
    //     cout<<min<<" "<<minI;
    // }
    // cout<<endl;
}


int main()
{
    int testCases;
    cin>>testCases;

    while(testCases--) testCase();
}
