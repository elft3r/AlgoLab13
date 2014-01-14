#include <CGAL/Exact_predicates_inexact_constructions_kernel.h>
#include <CGAL/Delaunay_triangulation_2.h>
#include <iostream>
#include <cmath>
#include <queue>

typedef CGAL::Exact_predicates_inexact_constructions_kernel K;
typedef CGAL::Delaunay_triangulation_2<K>  Triangulation;
typedef Triangulation::Edge_iterator  Edge_iterator;
typedef CGAL::Segment_2<K> Segment;
typedef K::Point_2 Point;

double ceil_to_double(const K::FT& x) {
    double a = std::ceil(CGAL::to_double(x));
    while(a<x) a+=1;
    while(a-1>=x) a-=1;
    return a;
}

int main() {
    std::ios_base::sync_with_stdio(false);

    int numBacteria;
    while(std::cin >> numBacteria) {
        if(numBacteria==0) break;
        int l,b,r,t;
        std::cin >> l >> b >> r >> t;
        std::vector<Segment> dish(4); 
        dish[0] = Segment(Point(l,b),Point(r,b));
        dish[1] = Segment(Point(r,b),Point(r,t));
        dish[2] = Segment(Point(r,t),Point(l,t));
        dish[3] = Segment(Point(l,t),Point(l,b));


        std::vector<K::Point_2> bacteria(numBacteria);
        for(int i=0;i<numBacteria;i++) {
            int x,y;
            std::cin >> x >> y;
            bacteria[i] = K::Point_2(x,y);
        }

        std::priority_queue<K::FT> q;
        if(numBacteria > 1) {
            Triangulation tria;
            tria.insert(bacteria.begin(), bacteria.end());

            Triangulation::Vertex_iterator v = tria.finite_vertices_begin();
            for(Triangulation::Vertex_iterator v = tria.finite_vertices_begin(); 
                v != tria.finite_vertices_end(); ++v) {
                bool firstEdge = true;
                K::FT minDist; 


                Triangulation::Edge_circulator c = tria.incident_edges(v);
                do {
                    if(!tria.is_infinite(c)) { 
                        Triangulation::Vertex_handle v1 = c->first->vertex((c->second + 1) % 3);
                        Triangulation::Vertex_handle v2 = c->first->vertex((c->second + 2) % 3);

                        K::FT candidateDist = Segment(v1->point(),v2->point()).squared_length();
                        if(firstEdge == true || minDist > candidateDist) {
                            minDist = candidateDist;
                            // std::cout << "canididate minimal distance for "<< v->point() << " between " << v1->point() << " and " << v2->point() << ": " << minDist << std::endl;
                            firstEdge = false;
                        }
                    }
                } while(++c != tria.incident_edges(v));
                //other bacterium extends as well
                minDist = sqrt(minDist)/2;
                // std::cout << "tentative minimal distance for "<< v->point() <<": " << minDist << std::endl;

                //compare with dish
                for(std::vector<Segment>::iterator s = dish.begin(); s!=dish.end(); s++) {
                    K::FT candidateDist = sqrt(squared_distance(v->point(),*s));
                    if(firstEdge == true || minDist > candidateDist) {
                        minDist = candidateDist; 
                        firstEdge = false;
                    }

                }
                //compute time
                K::FT time = sqrt(minDist-0.5);

                q.push(-time);
                // std::cout << "minimal distance for "<< v->point() <<": " << minDist << std::endl;

            }

        } else {
            bool firstEdge = true;
            K::FT minDist; 

            //compare with dish
            for(std::vector<Segment>::iterator s = dish.begin(); s!=dish.end(); s++) {
                K::FT candidateDist = squared_distance(bacteria[0],*s);
                if(firstEdge == true || minDist > candidateDist) {
                    minDist = candidateDist; 
                    bool firstEdge = false;
                }

            }
            // std::cout << "minimal distance: " << minDist << std::endl;
            //compute time
            K::FT time = sqrt(sqrt(minDist)-0.5);

            q.push(-time);

        }

        std::cout << ceil_to_double(-q.top()) << " ";
        int mid = numBacteria/2;
        for(int i=0; i < mid; i++) {
            q.pop();
            // std::cout << "1popped" << std::endl;
        }
        std::cout << ceil_to_double(-q.top()) << " ";
        // std::cout << "mid: " << mid << "numBacteria: " << numBacteria  << std::endl;
        for(int i=mid; i < numBacteria-1; i++) {
            q.pop();
            // std::cout << "2popped" << std::endl;
        }
        std::cout << ceil_to_double(-q.top());
        std::cout << std::endl;
    }
}



