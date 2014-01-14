#include <iostream>
#include <limits>
#include <vector>
#include <queue>
#include <set>
#include <sstream>
#include <cstdlib>

using namespace std;

template <typename T>
    string NumberToString ( T Number )
    {
     ostringstream ss;
     ss << Number;
     return ss.str();
    }

class point {
    private:
        int x;
        int y;

    public:
        point(int X, int Y) {
            x = X;
            y = Y;
        }
        point(const point &p) {
            x = p.x;
            y = p.y;
        }
        bool operator== (const point &other) const {
            return (x == other.getX() &&
                    y == other.getY());
        }
        bool operator!= (const point &other) const {
            return !(*this == other);
        }
        bool operator< (const point &other) const {
            if(x < other.x) {
                return true;
            } else if((x == other.x) && (y < other.y)) {
                return true;
            }
            return false;
        }
        int getX() const { return x; }
        int getY() const { return y; }
        string str() const { return "(" + NumberToString(x) + " " + NumberToString(y) + ")"; }
};

class vertex {
    private:
        point position;
        point velocity;
        int distance;
    public:
        vertex(point &i_position, point &i_velocity) :
                position(0,0), velocity(0,0) {
            position = i_position;
            velocity = i_velocity;
            distance = numeric_limits<int>::max();
        }
        bool operator== (const vertex &other) const {
            return (position == other.position 
                    && velocity == other.velocity 
                );
        }
        bool operator!= (const vertex &other) const {
            return !(*this == other);
        }
        bool operator < (const vertex& other) const {
            if(position < other.getPosition()) {
                    return true;
                } else if(position == other.position) {
                    return (velocity < other.velocity);
                }             
            return false;
        }
        point getPosition() const { return position; }
        point getVelocity() const { return velocity; }
        int getDistance() const { return distance; }
        void setDistance(int distance_arg) { distance = distance_arg; return;}
        string str() const { return "(" + NumberToString(distance) + "," + position.str() + "," + velocity.str() + ")"; }
};

struct OrderByDistance {
    bool operator() (const vertex& v1, const vertex& v2) const {
        return (v1.getDistance() > v2.getDistance());
    }
};

struct Obstacle {
    int x1;
    int x2;
    int y1;
    int y2;
};

bool validPosition(point p, int width, int height, Obstacle obstacles[], int numObstacles) {
    if(!((0<=p.getX()) && (p.getX()<width) && (0<=p.getY()) && (p.getY()<height))) {
        return false;
    }
    for(int i=0; i < numObstacles; i++) {
        Obstacle o = obstacles[i];
        if((o.x1 <= p.getX()) && (p.getX() <= o.x2) &&
                (o.y1 <= p.getY()) && (p.getY() <= o.y2)) {
            // cout << o.x1 << " " << p.getX() << " " << o.x2 << endl;
            // cout << o.y1 << " " << p.getY() << " " << o.y2 << endl;
            return false;

        }
    }
    return true;
}

void printQueue(priority_queue<vertex,vector<vertex>,OrderByDistance> queue) {
    cout << "Queue: ";
    while(queue.size() > 0) {
        cout << queue.top().str() << ","; queue.pop();
    }
    cout << endl;
}

void printSet(set<vertex> visit) {
    cout << "Visited: ";
    while(visit.size() > 0) {
        set<vertex>::iterator firstEle = visit.begin();
        cout << (*firstEle).str() << ","; visit.erase(firstEle);
    }
    cout << endl;
}

int vertexToHash(vertex v, int width, int height) {
    // x y xa ya
    return (v.getPosition().getX() * (height * 7 * 7)
            + v.getPosition().getY() * 7 * 7
            + (v.getVelocity().getX()+3) * 7
            + (v.getVelocity().getY()+3));
}

vertex hashToVertex(int hash, int width, int height) {
    int ya = (hash%7)-3;
    hash = hash/7;
    int xa = (hash%7)-3;
    hash = hash/7;
    int y = hash%height;
    hash = hash/height;
    int x = hash;
    point pos =point(x,y);
    point vel = point(xa,ya);
    return vertex(pos,vel);
}

void printVisited(bool *visit, int width, int height) {
    int n = width*height*7*7;
    cout << "Visited: ";
    for(int i=0; i < n; i++) {
        if(visit[i]) {
            cout << hashToVertex(i, width, height).str() << ",";
        }
    }
}


int shortestPath(point start, point end, int width, int height, Obstacle obstacles[], int numObstacles) {
    // set<vertex> visit;
    int numVisit = width*height*7*7;
    bool visit[numVisit];
    for(int v=0; v < numVisit; v++) {
        visit[v] = false;
    }
    // flags to set nodes state to ’ visited ’
    priority_queue<vertex,vector<vertex>,OrderByDistance> queue ;

    point zero(0,0);
    vertex startVertex(start, zero);
    startVertex.setDistance(-1);
    queue.push(startVertex);

    while (!queue.empty()) {
        vertex cur = queue.top(); queue.pop();
        if(visit[vertexToHash(cur,width,height)])
           continue;

        // printQueue(queue);
        // cout<<"current vertex: " << cur.str() << endl;
        // printVisited(visit, width, height);
        visit[vertexToHash(cur,width,height)] = true;
        if(cur.getPosition() == end) {
            return cur.getDistance();
        }

        point newPosition(cur.getPosition().getX() + cur.getVelocity().getX(),
                            cur.getPosition().getY() + cur.getVelocity().getY());

        if(validPosition(newPosition, width, height, obstacles, numObstacles)) {
            for(int xa=-1; xa <=1; xa++) {
                for(int ya=-1; ya <= 1; ya++) {
                    // if(xa == 0 && ya == 0 && ) { 
                    //     continue;
                    // }
                    if(abs(cur.getVelocity().getX() + xa) > 3
                            || abs(cur.getVelocity().getY() + ya) > 3) {
                        continue;
                    }

                    point newVelocity(cur.getVelocity().getX()+xa,
                                                    cur.getVelocity().getY()+ya);
                    vertex to = vertex(newPosition, newVelocity);
                    int altDistance = cur.getDistance() + 1;
                    if((!visit[vertexToHash(to,width,height)]) //if not in visited set
                        && altDistance < to.getDistance() ) {
                            to.setDistance(altDistance);
                            queue.push(to); //push again to the queue? not bad I think 
                    }
                    
                }

            }

        }
    }
    return -2;
}


int main() {
    cin.sync_with_stdio(false);
    int testCases;
    cin >> testCases;

    for(int i=0; i < testCases; i++) {

        int width, height;
        cin >> width >> height;
        int sx, sy, ex, ey;
        cin >> sx >> sy >> ex >> ey;
        point start(sx,sy), end(ex,ey);

        int numObstacles;
        cin >> numObstacles;
        Obstacle obstacles[numObstacles];
        for(int j=0; j<numObstacles; j++) {
            cin >> obstacles[j].x1 >> obstacles[j].y1 >> obstacles[j].x2 >> obstacles[j].y2;
        }
        int sp = shortestPath(start, end, width, height, obstacles, numObstacles);
        if(sp<-1) {
            cout << "No solution." << endl;
        } else if(sp==-1) {
            cout << "Optimal solution takes 0 hops." << endl;
        } else {
            cout << "Optimal solution takes " << sp << " hops." << endl;
        }
    }
}


