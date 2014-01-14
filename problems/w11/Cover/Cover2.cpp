#include <CGAL/Exact_predicates_exact_constructions_kernel.h>
#include <CGAL/Delaunay_triangulation_2.h>
#include <iostream>
#include <cmath>
#include <vector>
#include <limits>

typedef CGAL::Exact_predicates_exact_constructions_kernel K;
typedef CGAL::Delaunay_triangulation_2<K>  Triangulation;
typedef Triangulation::Edge_iterator  Edge_iterator;
typedef CGAL::Segment_2<K> Segment;
typedef CGAL::Point_2<K> Point;
typedef CGAL::Ray_2<K> Ray;
typedef Triangulation::Finite_faces_iterator FFiter;
typedef CGAL::Iso_rectangle_2<K> Rect;
typedef CGAL::Line_2<K> Line;
// typedef CGAL::Triangulation_data_structure_2 Tds;
typedef Triangulation::Face_handle Face_handle;
typedef Triangulation::Vertex_handle Vertex_handle;

const int MAX = std::numeric_limits<int>::max();


double ceil_to_double(double x) {
    double a = std::ceil(x);
    while(a<x) a+=1;
    while(a-1>=x) a-=1;
    return a;
}

K::FT sdistance(Point x, Point y) {
    return Segment(x,y).squared_length();
}

std::vector<Segment> rectToSegment(Rect r) {
    std::vector<Segment> v(4);
    v[0] = Segment(r.vertex(0), r.vertex(1));
    v[1] = Segment(r.vertex(1), r.vertex(2));
    v[2] = Segment(r.vertex(2), r.vertex(3));
    v[3] = Segment(r.vertex(3), r.vertex(4));
    return v;
}

template<typename T>
K::FT check_intersection_segments(T &voronoiEdge, Rect &area, Triangulation &t) {
        std::vector<Segment> areaSegments = rectToSegment(area);
        Segment intersectedAreaEdge;
        bool isIntersected =false;
        for(int i=0;i<4;i++) {
            if(CGAL::do_intersect(voronoiEdge,areaSegments[i])) {
                intersectedAreaEdge = areaSegments[i];
                isIntersected =true;
            }
        }
        if(!isIntersected) return -1;

        CGAL::Object o2 = CGAL::intersection(voronoiEdge, intersectedAreaEdge);
        if(const Point* op = CGAL::object_cast<Point>(&o2)) {
            Vertex_handle vh = t.nearest_vertex(*op);
            return sdistance(*op, vh->point()); 

        } else {
            std::cerr<<"runtime error";
            return -1;
            throw std::runtime_error("strange segment intersection");
        }
}


void testcase(int numAntenna) {
    double x1,x2,y1,y2;
    std::cin >> x1 >> y1 >> x2 >> y2;
    Rect area = Rect(Point(x1,y1),Point(x2,y2));
    std::vector<Point > antennas(numAntenna);
    Triangulation t;
    for(int i=0; i<numAntenna; i++) {
        double x,y;
        std::cin>>x>>y;
        antennas[i] = Point(x,y);
    }

    t.insert(antennas.begin(), antennas.end());
    K::FT maxDist = -1;

    //case 1: go over finite faces get circumcenter
    for(FFiter i=t.finite_faces_begin();
            i!=t.finite_faces_end();
            i++) {
        // Point c = t.circumcenter(i);
        Point c = t.dual(i);
        if(c.x() >= x1 && c.x() <= x2 && c.y() >= y1 && c.y() <= y2) {
            K::FT tentativeMax = sdistance(c, i->vertex(1)->point());
            if(tentativeMax > maxDist)
                maxDist = tentativeMax;
        }
    }
    // std::cout<<"maxDist " <<ceil_to_double_sqrt(maxDist) << std::endl;

    //case 2: go over corners of area, get nearest vertex
    //for every corner
    for(int i=0; i<4; i++) {
        Point corner = area.vertex(i);
        //for every nearestVertex
        Vertex_handle vh = t.nearest_vertex(corner);
        K::FT tentMax = sdistance(vh->point(), corner); 

        if(tentMax > maxDist) {
            maxDist = tentMax;
        }
    }
    // std::cout<<"maxDist " <<ceil_to_double_sqrt(maxDist) << std::endl;

    //case 3: go over infinite faces
    // process all Voronoi edges
    for(Edge_iterator e = t.finite_edges_begin(); e != t.finite_edges_end(); ++e) {
        CGAL::Object o = t.dual(e);
        // o can be a segment, a ray or a line ...
        Point* areaIntersection;
        if(const Ray* oray = CGAL::object_cast<Ray>(&o)) {
            maxDist = max(maxDist,check_intersection_segments(*oray, area, t));
        } else if(const Line* oray = CGAL::object_cast<Line>(&o)) {
            maxDist = max(maxDist,check_intersection_segments(*oray, area, t));
        } else if(const Segment* oray = CGAL::object_cast<Segment>(&o)) {
            maxDist = max(maxDist,check_intersection_segments(*oray, area, t));
        }
    }
    std::cout<<std::setiosflags(std::ios::fixed) << std::setprecision(0)<< ceil_to_double(sqrt(CGAL::to_double(maxDist)))
            <<std::endl;
    // std::cout<< numAntenna<<std::endl;
}
int main() {
    std::ios_base::sync_with_stdio(false);
    while(true) {
        int numAntenna;
        std::cin>>numAntenna;
        if(numAntenna == 0) return 0;
        testcase(numAntenna);
    }
}
