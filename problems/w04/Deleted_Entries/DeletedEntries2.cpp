#include <iostream>
#include <vector>
#include <boost/config.hpp>
#include <boost/graph/adjacency_list.hpp>
#include <boost/tuple/tuple.hpp>
#include <boost/graph/graphviz.hpp>
#include <boost/graph/kruskal_min_spanning_tree.hpp>
#include <boost/graph/dijkstra_shortest_paths.hpp>
#include <boost/graph/sequential_vertex_coloring.hpp>

using namespace std;
using namespace boost;

typedef adjacency_list<vecS, vecS, undirectedS > Graph;
typedef graph_traits<Graph> Traits;
typedef Traits::vertex_descriptor Vertex;
typedef Traits::edge_descriptor Edge;
typedef property_map<Graph, vertex_index_t>::type IndexMap;

void printGraph(Graph g) {
    graph_traits<Graph>::edge_iterator eiter, eiter_end;
    for (tie(eiter, eiter_end) = edges(g); eiter != eiter_end; ++eiter) {
        std::cout << source(*eiter, g) << " <--> " << target(*eiter, g)
          << std::endl;
    }
}

// typedef vector<Vertex> PredecessorMap;
// typedef vector<int> ColorMap;

// class colorize : public default_bfs_visitor {
//     private:
//         PredecessorMap m_predecessor;
//         ColorMap m_colorMap;
//         ColorMap m_prevColorMap;
//         int maxColors;
//     public: 
//         //give numColors as parameter
//         colorize(PredecessorMap p, ColorMap c, ColorMap prevC, int numColors) 
//             : m_predecessor(p), m_colorMap(c), 
//                 m_prevColorMap(prevC) {
//                     maxColors = numColors;
//                 
//             }
//         int newColor(int c) {
//             return (c+1)%maxColors;
//         }
//         void tree_edge(Edge e, Graph g) {
//             Vertex s = source(e, g);
//             Vertex t = target(e, g);
//             put(m_predecessor, t, s);
//             int color = newColor(m_prevColorMap[s]); 
//             if(color == m_colorMap[s]) {
//                 color = newColor(color);
//             }
//             put(m_colorMap, t, color);
//             put(m_prevColorMap, s, color);
//         }
//         //void on_start_vertex default color
// };

void colorize(Graph &g, int numColors, vector<int> &color) {
    IndexMap index = get(vertex_index, g);
    vector<bool> visited(num_vertices(g), false);
    std::queue<Vertex> bfs_queue;

    //get start vertex
    Traits::vertex_iterator vi;
    tie(vi, tuples::ignore) = vertices(g);
    Vertex startV = *vi;

    color[index[startV]] = 0;
    visited[index[startV]] = true;
    bfs_queue.push(startV);

    int curColor = 0;
    while (!bfs_queue.empty()) {
        Vertex v = bfs_queue.front();
        bfs_queue.pop();
        int parentColor = color[index[v]];
        // cout << "current " << index[v] << " col:" << color[index[v]] << endl;

        Traits::adjacency_iterator adjV, adEnd;
        tie(adjV, adEnd) = adjacent_vertices(v, g);
        for (; adjV != adEnd; adjV++) {
            int vIndex = index[*adjV];
            if(!visited[vIndex]) {
                visited[vIndex] = true;
                curColor=(curColor+1)%numColors;
                if(curColor==parentColor) 
                    curColor=(curColor+1)%numColors;
                color[vIndex] = curColor;
                bfs_queue.push(*adjV);
            }

        }
    }
}

int main() {
    ios_base::sync_with_stdio(false);
    int testCases = 0;
    cin >> testCases;
    for(int testCase=0; testCase < testCases; testCase++) {
        int numStudents, numEdges, numGroups;
        cin >> numStudents >> numEdges >> numGroups;
        Graph g(numStudents);
        for(int i = 0; i < numEdges; i++) {
            Edge e;
            int u,v;
            cin >> u >> v;
            tie(e, tuples::ignore) =add_edge(u,v,g);
        }
        // cout << "whole graph: " << endl;
        // printGraph(g);

        // vector<Edge> spanning_tree;
        // //connected components
        // kruskal_minimum_spanning_tree(g, back_inserter(spanning_tree)); 

        // vector<vector<int> > groupList(numGroups);
        Graph MST = g;
        // Graph MST(spanning_tree.begin(), spanning_tree.end(), numStudents);
        // for(vector<Edge>::iterator it=spanning_tree.begin(); it !=spanning_tree.end(); it++) {
        //     Edge e;
        //     tie(e, tuples::ignore) =add_edge(source(*it,g),target(*it,g),MST);
        // }

        // cout << "MS tree: " << endl;
        // printGraph(MST);
        
        //not enough vertices? -> no
        if(num_vertices(MST) < numGroups) {
            cout << "no" << endl;
            continue;
        }
        vector<int> color(num_vertices(g), -1);
        colorize(MST, numGroups, color);

        // not connected? -> no
        if(find(color.begin(), color.end(), -1) != color.end()) {
            cout << "no" << endl;
            continue;
        }


        vector< vector<int> > groups(numGroups, vector<int>());
        for(int i=0; i<numStudents; i++) {
            int studGroup = color[i];
            groups[studGroup].push_back(i);
        }

        cout << "yes" << endl;
        for(vector<vector<int> >::iterator it=groups.begin(); it != groups.end(); it++) {
            cout << (*it).size();
            for(int i=0; i < (*it).size(); i++) {
                cout << " " <<(*it)[i];
            }
            cout << endl;
        }

    }
    return 0;
}

