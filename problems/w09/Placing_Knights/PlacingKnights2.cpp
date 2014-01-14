#include <iostream>
#include <vector>
#include <set>
#include <cstdlib>
#include <boost/graph/max_cardinality_matching.hpp>
#include <boost/tuple/tuple.hpp>
#include <boost/graph/adjacency_list.hpp>
#include <boost/config.hpp>

using namespace std;
using namespace boost;

typedef pair<int,int> Pos;
typedef adjacency_list<setS, vecS, undirectedS > Graph;
typedef graph_traits<Graph> Traits;
typedef Traits::vertex_descriptor Vertex;
typedef Traits::edge_descriptor Edge;
typedef property_map<Graph, vertex_index_t>::type IndexMap;


//returns the knights that the given knight threatens
vector<Pos> threatens(vector<vector<int> > &board, Pos &knight) {
    vector<Pos> ret;  
    for(int i=-2;i<=2;i++) {
        for(int j=-2;j<=2;j++) {
            if(!((abs(i)== 1 && abs(j) ==2) 
                    || (abs(i)== 2 && abs(j) ==1))) continue; //move not allowed
            unsigned int newX = knight.first+i;
            unsigned int newY = knight.second+j;
            if(0 <= newX && newX < board.size()
                    && 0 <= newY && newY < board.size()
                    && board[newX][newY] != 0) {
                ret.push_back(Pos(newX, newY));
            }
        }
    }
    return ret;
}

void setToZero(vector<vector<int> > &board, vector<Pos> &threatened) {
    for(unsigned int i=0; i<threatened.size(); i++) {
        Pos pos = threatened[i];
        board[pos.first][pos.second] = 0;
    }
}

void printBoard(vector<vector<int> > &board) {
    for(unsigned int i=0; i<board.size(); i++) {
        for(unsigned int j=0; j<board.size(); j++) {
            cout << board[i][j];
        }
        cout << endl;
    }
}


void reduceNumThreatened(set<pair<int, pair<int, int> > > &Q, vector<vector<int> > &board, vector<vector<int> > &boardThreatening, vector<Pos> &threatened) {
   for(unsigned int i=0; i<threatened.size(); i++) {
       Pos pos = threatened[i];
       int numThreatens = boardThreatening[pos.first][pos.second];
       Q.erase(pair<int, Pos>(numThreatens, pos));

       vector<Pos> toReduce = threatens(board, pos);
       for(unsigned int j=0; j<toReduce.size(); j++) {
           Pos toReducePos = toReduce[j];
           numThreatens = boardThreatening[toReducePos.first][toReducePos.second];
           Q.erase(pair<int, Pos>(numThreatens, toReducePos));
           numThreatens--;
           Q.insert(pair<int, Pos>(numThreatens, toReducePos));
           boardThreatening[toReducePos.first][toReducePos.second] = numThreatens;
       }
   }
}

int numKnights(vector<vector<int> > board) {
    int numKnights = 0;
    int numPosLoc = 0;
    set<pair<int, pair<int, int> > > Q;
    int n = board.size();
    vector<vector<int> > boardThreatening(n,vector<int>(n));

    for(int i=0;i<n;i++) {
        for(int j=0;j<n;j++) {
            if(board[i][j] != 0) {
                Pos pos =Pos(i,j);
                int numThreatening = threatens(board, pos).size();
                Q.insert(pair<int, pair<int, int> >(numThreatening,pos));
                boardThreatening[pos.first][pos.second] = numThreatening;
                numPosLoc++;
            }
        }
    }
    while(Q.size() > 0 && numPosLoc > 0) {
        // cout<< "board" << std::endl;
        // printBoard(board);
        // cout<<"num threatening" <<std::endl;
        // printBoard(boardThreatening);
        pair<int, pair<int, int> > p = *(Q.begin());
        Q.erase(Q.begin());
        pair<int, int> pos = p.second;
        if(board[pos.first][pos.second] != 0) {
            vector<Pos> threatened = threatens(board, pos);  
            setToZero(board, threatened);
            board[pos.first][pos.second] = 0;
            numPosLoc -= (threatened.size() + 1);
            numKnights++;
            threatened.push_back(pos);
            reduceNumThreatened(Q, board, boardThreatening, threatened);
        }
    }
    return numKnights;
}

int posToInt(pair<int,int> pos, int n) {
    return pos.first*n+pos.second;
}

int numKnightsCorrect(vector<vector<int> > board) {
    Graph g;
    int n = board.size();
    int numVertices = 0;

    //build bipartite graph
    for(int i=0;i<n;i++) {
        for(int j=0;j<n;j++) {
            if(board[i][j] == 0) continue;
            Pos curPos(i,j);
            vector<Pos> threatened = threatens(board, curPos);
            numVertices++;
            for(int k=0; k<threatened.size(); k++) {
                Edge e;
                tie(e, tuples::ignore) = add_edge(
                        posToInt(curPos,n),
                        posToInt(threatened[k],n),g);
            }
        }
    }
    vector<Vertex> mate(num_vertices(g));
    edmonds_maximum_cardinality_matching(g, &mate[0]);
    const Vertex NULL_VERTEX = graph_traits<Graph>::null_vertex();
    int numMatched = 0;
    for(int i=0; i<mate.size(); i++) {
        if(mate[i] != NULL_VERTEX) numMatched++;
    }
    return numVertices - (numMatched/2);
}

int main() {
    ios_base::sync_with_stdio(false);
    int testCases;
    cin >> testCases;
    for(int t=0; t < testCases; t++) {
        int n;
        cin >> n;
        vector<vector<int> > board(n,vector<int>(n));

        for(int i=0;i<n;i++) {
            for(int j=0;j<n;j++) {
                int status;
                cin >> status;
                board[i][j] = status;
            }
        }

        cout << numKnightsCorrect(board) << std::endl;

    }
    return 0;
}
