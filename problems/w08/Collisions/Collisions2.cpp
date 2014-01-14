#include <CGAL/Exact_predicates_inexact_constructions_kernel.h>
#include <CGAL/Delaunay_triangulation_2.h>
#include <iostream>
#include <vector>

typedef CGAL::Exact_predicates_inexact_constructions_kernel K;
typedef CGAL::Delaunay_triangulation_2<K> Triangulation;
typedef Triangulation::Vertex_iterator Vertex_iterator;
typedef K::Point_2 Point;
typedef CGAL::Segment_2<K> Segment;

int main() {
    std::ios_base::sync_with_stdio(false);
    int testCases;
    std::cin >> testCases;
    for(int i=0; i<testCases; i++) {
        int numPlanes;
        K::FT minDistance;
        std::cin >> numPlanes>> minDistance;

	std::vector<Point> planes(numPlanes);

	Triangulation t;

        for(int j=0; j<numPlanes;j++) {
            int x, y;
            std::cin >> x >> y;
            //t.insert(Triangulation::Point(x,y));
	        planes[j] = Point(x,y);
        }

	t.insert(planes.begin(), planes.end());
	
	int numEndangered = 0;
	K::FT minSquaredDistance = minDistance * minDistance;

	for(Vertex_iterator v=t.finite_vertices_begin();
	    v!=t.finite_vertices_end(); v++) {
	    // std::cout<<v->point() << std::endl;
	    Triangulation::Edge_circulator c = t.incident_edges(v);
	    K::FT minDist;
	    bool firstEdge = true;

	    do {
	      if(!t.is_infinite(c)) {
		    Triangulation::Vertex_handle v1 = c->first->vertex((c->second+1)%3);
		    Triangulation::Vertex_handle v2 = c->first->vertex((c->second+2)%3);

		    K::FT candidateDist = Segment(v1->point(), v2->point()).squared_length();
		    // std::cout << "candidate min dist for " << v->point() << " between " << v1->point() << " " << v2->point() << " is " << candidateDist << std::endl;
		    if(firstEdge == true || minDist > candidateDist) {
		      minDist = candidateDist;
		      firstEdge = false;
		    }
		}
	     } while(++c != t.incident_edges(v));

	     if(!firstEdge && minDist < minSquaredDistance)
		numEndangered++;
	}

	std::cout << numEndangered << std::endl;

    }
}
