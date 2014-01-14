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

pair<int, int> computeTask(Graph &g, WeightMap &weightMap) {
    vector<Edge> spanning_tree;
    kruskal_minimum_spanning_tree(g, back_inserter(spanning_tree)); 

    int sumMSTWeights = 0;
    // cout << "Print the edges in the MST:" << endl;
  for (vector < Edge >::iterator ei = spanning_tree.begin();
       ei != spanning_tree.end(); ++ei) {
    // cout << source(*ei, g) << " <--> " << target(*ei, g) << " with weight of " << weightMap[*ei] << endl;
    sumMSTWeights += weightMap[*ei];
  }

  //get vertex 0
  graph_traits<Graph>::vertex_iterator vert_0;
  tie(vert_0, tuples::ignore) = vertices(g);
  vector<int> distances(num_vertices(g));
  vector<Vertex> predecessor(num_vertices(g));

  dijkstra_shortest_paths(g, 0, 
          predecessor_map(&predecessor[0]).distance_map(&distances[0])); 
  return pair<int, int>(sumMSTWeights, *max_element(distances.begin(), distances.end()));

}
int main() {
    ios_base::sync_with_stdio(false);
    int testCases = 0;
    cin >> testCases;
    for(int testCase=0; testCase < testCases; testCase++) {
        int numVertices, numEdges;
        cin >> numVertices >> numEdges;
        Graph g(numVertices);
        WeightMap weightMap = get(edge_weight, g);
        for(int edgeIndex=0; edgeIndex < numEdges; edgeIndex++) {
            int vo,vi,w;
            cin >> vo >> vi >> w;
            Edge e;
            tie(e,tuples::ignore) = add_edge(vi,vo,g);
            weightMap[e] = w;
        }
        // cout << computeTask(g) << endl;
        pair<int, int> result = computeTask(g, weightMap);
        cout << result.first << " " << result.second << endl;

        
    }

    return 0;
}
