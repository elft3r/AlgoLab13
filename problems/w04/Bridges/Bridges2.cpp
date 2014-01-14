#include <iostream>
#include <vector>
#include <map>
#include <set>
#include <boost/config.hpp>
#include <boost/graph/adjacency_list.hpp>
#include <boost/tuple/tuple.hpp>
#include <boost/graph/biconnected_components.hpp>
#include <boost/graph/connected_components.hpp>

using namespace std;
using namespace boost;

namespace boost
{
  struct edge_component_t
  {
    enum
    { num = 555 };
    typedef edge_property_tag kind;
  }
  edge_component;
}


typedef adjacency_list<setS, vecS, undirectedS, no_property, 
        property<edge_component_t, std::size_t> > Graph;
typedef graph_traits<Graph> Traits;
typedef Traits::vertex_descriptor Vertex;
typedef Traits::edge_descriptor Edge;
typedef property_map<Graph, vertex_index_t>::type IndexMap;
typedef property_map<Graph, edge_component_t>::type ComponentMap;

void bridgesWithBiconnected(Graph &g, std::set<pair<int, int> > &criticalBridges) {
    ComponentMap componentMap = get(edge_component, g);
    size_t num_comps = biconnected_components(g, componentMap);
    vector<vector<Edge> > numEleOfComp(num_comps, vector<Edge>());

    IndexMap index = get(vertex_index, g);
    Traits::edge_iterator ei, ei_end;
    // cout << "new:"<<endl;
    for (tie(ei, ei_end) = edges(g); ei != ei_end; ++ei) {
        // std::cout << "(" << index[source(*ei, g)] 
        //       << "," << index[target(*ei, g)] << ") ";
        // cout << " comp: " << componentMap[*ei] << endl;
        numEleOfComp[componentMap[*ei]].push_back(*ei); 
    }

    
    //every edge that is alone in a biconnected component is an important bridge
    for(int i=0; i<num_comps; i++) {
        if(numEleOfComp[i].size() ==1) {
            Edge e = numEleOfComp[i][0];
            int u = index[source(e, g)];
            int v = index[target(e, g)];
            pair<int,int> uv = (u<v)?pair<int,int>(u, v):pair<int,int>(v, u);
            criticalBridges.insert(uv);
        }
    }
}
void bridgesWithBruteForce(Graph &g, std::set<pair<int, int> > &criticalBridges) {
    vector<Edge> edgeVector;
    std::vector<int> componentMap(num_vertices(g));
    int components = connected_components(g, &componentMap[0]);

    Traits::edge_iterator ei, ei_end;
    for (tie(ei, ei_end) = edges(g); ei != ei_end; ++ei) {
        edgeVector.push_back(*ei);
    }
    for(int i=0; i<edgeVector.size(); i++) {
        Edge anEdge = edgeVector[i];
        int sourceNode = source(anEdge, g);
        int targetNode = target(anEdge,g);
        remove_edge(sourceNode, targetNode, g);
        if(connected_components(g, &componentMap[0]) > components)
            criticalBridges.insert(pair<int,int>(source(anEdge, g), target(anEdge,g)));
        Edge e;
        tie(e, tuples::ignore) = add_edge(sourceNode,targetNode,g);
    }

}

int main() {
    ios_base::sync_with_stdio(false);
    int testCases = 0;
    cin >> testCases;
    for(int testCase=0; testCase < testCases; testCase++) {
        int numCities, numBridges;
        cin >> numCities >> numBridges;

        Graph g(numCities);
        for(int i=0; i < numBridges; i++) {
            Edge e;
            int u,v;
            cin >> u >> v; 
            tie(e, tuples::ignore) = add_edge(u,v,g);
        }

        std::set<pair<int, int> > criticalBridges;

        // if(numCities > 50) {
            bridgesWithBiconnected(g,criticalBridges);
        // } else {
            // bridgesWithBruteForce(g,criticalBridges);
        // }

        int nCriticalBridges = criticalBridges.size();
        cout << nCriticalBridges << endl;
        for(set<pair<int,int> >::iterator eit=criticalBridges.begin(); 
                eit!=criticalBridges.end(); eit++){
             cout << (*eit).first 
              << " " << (*eit).second << endl;
        }
    }
}

