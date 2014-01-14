#include <iostream>
#include <vector>
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

int sum(vector<int> &aVector) {
    int sum_of_elems = 0;
    for(std::vector<int>::iterator j=aVector.begin();j!=aVector.end();++j)
        sum_of_elems += *j;
    return sum_of_elems;
}


int main() {
    ios_base::sync_with_stdio(false);
    int testCases = 0;
    cin >> testCases;
    for(int testCase=0; testCase < testCases; testCase++) {
        int numPlayers, numRounds;
        int outcome = 0; //-1 false, 0 don't know, 1 true
        cin >> numPlayers >> numRounds;

        vector<int> scoreBoard(numPlayers, 0);
        const int SOURCE = numPlayers;
        const int SINK = numPlayers+1;
        vector<vector<int> > helperNode(numPlayers, vector<int>(numPlayers, -1));
        Graph g;
        EdgeCapacityMap capacity = get(edge_capacity, g);
        ReverseEdgeMap rev_edge = get(edge_reverse, g);
        ResidualCapacityMap res_capacity = get(edge_residual_capacity, g);
        int outOfSink = 0;

        for(int i=0; i<numRounds;i++) {
            int playerA, playerB, outcome;
            cin >> playerA >> playerB >> outcome;
            if(outcome == 1) {
                scoreBoard[playerA] -=1;
            } else if(outcome == 2) {
                scoreBoard[playerB] -=1;
            } else {
                //construct graph
                if(helperNode[playerA][playerB] == -1) {
                    //no helper node before
                    int aHelperNode = SINK+1+i;
                    helperNode[playerA][playerB] = aHelperNode;
                    helperNode[playerB][playerA] = aHelperNode;

                    addFlowEdge(g, capacity, rev_edge, SOURCE, aHelperNode, 1); 
                    addFlowEdge(g, capacity, rev_edge, aHelperNode, playerA, 1); 
                    addFlowEdge(g, capacity, rev_edge, aHelperNode, playerB, 1); 
                } else {
                    int aHelperNode = helperNode[playerA][playerB];
                    Edge sourceToHelper, helperToA, helperToB;
                    sourceToHelper = edge(SOURCE, aHelperNode, g).first;
                    helperToA = edge(aHelperNode, playerA, g).first;
                    helperToB = edge(aHelperNode, playerB, g).first;
                    capacity[sourceToHelper] += 1;
                    capacity[helperToA] += 1;
                    capacity[helperToB] += 1;
                }
                outOfSink++;
            }
        }
        for(int i=0; i<numPlayers; i++) {
            int desiredScore;
            cin >> desiredScore;
            scoreBoard[i] += desiredScore;
            if(scoreBoard[i] < 0) {
                //a player won more often than in the desired scoreboard
                outcome = -1;
            }
        }
        if(outcome >= 0 && outOfSink==sum(scoreBoard)) {

            // if(scoreBoard.sum() < outCapacitySource) {
                // outcome = -1;
            // }

            // add edges to sink
            for(int i=0; i<numPlayers; i++) {
                if(scoreBoard[i] > 0) {
                    addFlowEdge(g, capacity, rev_edge, i, SINK, scoreBoard[i]);
                }
            }
            

            long flow = push_relabel_max_flow(g, SOURCE, SINK);
            if(flow != sum(scoreBoard)) {
                // cout << "flow != sum(scoreBoard)" << endl;
                cout << "no" << endl;
            } else {
                cout << "yes" << endl;
                // cout << flow << " " << sum(scoreBoard) << endl;
            }
        } else {
            // cout << "a player won more often than in the desired scoreboard" << endl;
            cout << "no" << endl;
        }
    }
}


