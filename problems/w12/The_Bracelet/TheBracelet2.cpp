#include <iostream>
#include <vector>
#include <stack>
#include <map>
#include <set>
#include <boost/config.hpp>
#include <boost/graph/adjacency_list.hpp>
#include <boost/tuple/tuple.hpp>
#include <boost/graph/connected_components.hpp>

using namespace std;
using namespace boost;

typedef adjacency_list<vecS, vecS, undirectedS,
                        no_property, property<edge_weight_t, int> > Graph;
typedef graph_traits<Graph> Traits;
typedef Traits::vertex_descriptor Vertex;
typedef Traits::edge_descriptor Edge;
typedef Traits::vertex_iterator vertex_iter;
typedef Traits::out_edge_iterator edge_iterator;
typedef property_map<Graph, edge_weight_t>::type WeightMap;

void eulerTourDfs(Graph &g, WeightMap &marked, Vertex v, vector<int> &tour) {

    edge_iterator out_i, out_end;
    for (tie(out_i, out_end) = out_edges(v, g);
            out_i!=out_end; ++out_i) {
        if(marked[*out_i]==0) {
            marked[*out_i]=1;
            eulerTourDfs(g,marked,target(*out_i, g),tour);
        }
    }
    tour.push_back(v);
}
void eulerTour(Graph &g, WeightMap &marked, Vertex v, vector<int> &tour) {
    stack<int> S;
    S.push(v);
    while(!S.empty()) {
        Vertex v = S.top();
        edge_iterator out_i, out_end;
        bool hasUnmarkedEdge=false;
        for (tie(out_i, out_end) = out_edges(v, g);
                out_i!=out_end; ++out_i) {
            if(marked[*out_i]==0) {
                hasUnmarkedEdge=true;
                marked[*out_i]=1;
                S.push(target(*out_i,g));
                break;
            }
        }

        if(!hasUnmarkedEdge) {
            S.pop();
            tour.push_back(v);
        }
    }
}


void testCase(int num) {
    cout<<"Case #"<<num<<std::endl;
    int numBeads;
    cin>>numBeads;

    Graph g;
    WeightMap marked = get(edge_weight, g);
    vector<int> colorMap(51,-1);
    int colorIndex = 0;

    for(int i=0;i<numBeads;i++) {
        int color1, color2;
        cin>>color1>>color2;

        // map colors to gapless indexes starting with 0 
        if(colorMap[color1] == -1) {
            colorMap[color1] = colorIndex;
            ++colorIndex;
        }
        if(colorMap[color2] == -1) {
            colorMap[color2] = colorIndex;
            ++colorIndex;
        }

        Edge e;
        tie(e, tuples::ignore) =add_edge(colorMap[color1],colorMap[color2],g);
        marked[e]=0;
    }

    vector<int> componentMap(num_vertices(g));
    int components = connected_components(g, &componentMap[0]);
    if(components > 1) {
        cout<<"some beads may be lost"<<std::endl;
        // cout<<"reason 1: "<<components<<std::endl;
        return;
    }

    std::pair<vertex_iter,vertex_iter> vi;
    for(vi=vertices(g); vi.first != vi.second; ++vi.first) {
        int degree = out_degree(*(vi.first),g);
        if(degree%2==1) {
            cout<<"some beads may be lost"<<std::endl;
            // cout<<"reason 2"<<std::endl;
            return;
        }
    }

    //print eulerian tour 
    vector<int> tour;
    // eulerTourDfs(g,marked,0,tour);
    eulerTour(g,marked,0,tour);

    // reverse colorMap
    vector<int> inverseColorMap(51,-1);
    for(int i=0; i<colorMap.size(); i++) {
        if(colorMap[i] != -1) {
            inverseColorMap[colorMap[i]] = i;
        }
    } 

    for(int i=0; i<tour.size()-1; i++) {
        cout<<inverseColorMap[tour[i]]<<" "<<inverseColorMap[tour[i+1]]<<std::endl;
    }
}

int main() {
    ios_base::sync_with_stdio(false);
    int testCases;
    cin >> testCases;
    int i = 0;
    while(testCases--) { 
        testCase(++i);
        if(testCases>0) cout<<std::endl;
    }
}


