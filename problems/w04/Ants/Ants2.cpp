#include <iostream>
#include <vector>
#include <boost/config.hpp>
#include <boost/graph/adjacency_list.hpp>
#include <boost/tuple/tuple.hpp>
#include <boost/graph/graphviz.hpp>
#include <boost/graph/kruskal_min_spanning_tree.hpp>
#include <boost/graph/dijkstra_shortest_paths.hpp>

using namespace std;
using namespace boost;

typedef adjacency_list<vecS, vecS, undirectedS, no_property,
property<edge_weight_t, int> > Graph;
typedef graph_traits<Graph> Traits;
typedef Traits::vertex_descriptor Vertex;
typedef Traits::edge_descriptor Edge;
typedef property_map<Graph, edge_weight_t>::type WeightMap;

void printGraph(Graph g, WeightMap weight) {
    graph_traits<Graph>::edge_iterator eiter, eiter_end;
    for (tie(eiter, eiter_end) = edges(g); eiter != eiter_end; ++eiter) {
        std::cout << source(*eiter, g) << " <--> " << target(*eiter, g)
          << " with weight of " << weight[*eiter]
          << std::endl;
    }
}


int main() {
    ios_base::sync_with_stdio(false);
    int testCases = 0;
    cin >> testCases;
    for(int testCase=0; testCase < testCases; testCase++) {
        int numTreeNodes, numTreeEdges;
        int numSpecies, startTree, finishTree;
        cin >> numTreeNodes >> numTreeEdges >> numSpecies
                >> startTree >> finishTree;

        //construct graph for each species
        Graph speciesGraph[numSpecies];
        WeightMap weightMap[numSpecies];
        for(int i=0; i < numSpecies; i++) { 
            speciesGraph[i] = Graph(numTreeNodes);
            weightMap[i] = get(edge_weight, speciesGraph[i]);
        }
        for(int e=0; e < numTreeEdges; e++) {
            int v1, v2;
            cin >> v1 >> v2;
            for(int i=0; i < numSpecies; i++) { 
                Edge e;
                int w;
                cin >> w;
                tie(e,tuples::ignore) = add_edge(v1,v2,speciesGraph[i]);
                weightMap[i][e] = w;
            }

        }
        int hive[numSpecies];
        for(int i=0; i < numSpecies; i++) { 
            int aHive;
            cin >> aHive;
            hive[i] = aHive;
            // cout << "species " << i << endl;
            // printGraph(speciesGraph[i], weightMap[i]);
        }

        // compute minimum spanning tree for each species
        vector<Edge> spanning_tree[numSpecies];
        Graph speciesMST[numSpecies];
        WeightMap weightMapMST[numSpecies];
        for(int i=0; i < numSpecies; i++) { 
            kruskal_minimum_spanning_tree(speciesGraph[i], back_inserter(spanning_tree[i])); 
            //construct new tree
            for(vector<Edge>::iterator it=spanning_tree[i].begin(); it!=spanning_tree[i].end(); it++) {
                Edge e;
                tie(e,tuples::ignore) = add_edge(source(*it,speciesGraph[i]),
                                                target(*it,speciesGraph[i]),
                                                speciesMST[i]);
                weightMapMST[i][e] = weightMap[i][*it];
            }
            // cout << "species MST" << i << endl;
            // printGraph(speciesMST[i], weightMapMST[i]);
        }

        // build combined graph
        Graph combinedGraph(numTreeNodes);
        WeightMap combinedWeightMap;
        combinedWeightMap = get(edge_weight, combinedGraph);

        graph_traits<Graph>::edge_iterator eiter, eiter_end;
        //iterate over all possible edges
        for (tie(eiter, eiter_end) = edges(speciesGraph[0]); eiter != eiter_end; ++eiter) {
            Vertex u = get(vertex_index, speciesGraph[0], source(*eiter, speciesGraph[0]));
            Vertex v = get(vertex_index, speciesGraph[0], target(*eiter, speciesGraph[0]));

            int minWeight = numeric_limits<int>::max();
            for(int s=0; s < numSpecies;s++) {
                Edge speciesEdge;
                bool hasEdge = false;
                tie(speciesEdge, hasEdge) = edge(u,v,speciesMST[s]);
                if(hasEdge) {
                    int weight = get(weightMapMST[s],speciesEdge);
                    if(weight < minWeight) {
                        minWeight = weight;
                    }
                }
            }
            if(minWeight < numeric_limits<int>::max()) {
                Edge e;
                tie(e,tuples::ignore) = add_edge(u,v,combinedGraph);
                combinedWeightMap[e] = minWeight;
            }

        }


        // cout << "combined graph" << endl;
        // printGraph(combinedGraph, combinedWeightMap);

        // dijkstra
        vector<int> distances(num_vertices(combinedGraph));
        dijkstra_shortest_paths(combinedGraph, startTree, 
          distance_map(&distances[0])); 
        // for(vector<int>::iterator it=distances.begin(); it!=distances.end(); it++) {
        //     cout << "in dlist: " << *it << endl;
        // }
        cout << distances[finishTree] << endl;
    }
}


