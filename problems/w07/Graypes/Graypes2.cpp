// #include <CGAL/Exact_predicates_exact_constructions_kernel_with_sqrt.h>
#include <CGAL/Exact_predicates_inexact_constructions_kernel.h>
#include <CGAL/Delaunay_triangulation_2.h>
#include <iostream>
#include <cmath>

typedef CGAL::Exact_predicates_inexact_constructions_kernel K;
typedef CGAL::Delaunay_triangulation_2<K>  Triangulation;
typedef Triangulation::Edge_iterator  Edge_iterator;
typedef CGAL::Segment_2<K> Segment;

double ceil_to_double(const K::FT& x) {
    double a = std::ceil(CGAL::to_double(x));
    while(a<x) a+=1;
    while(a-1>=x) a-=1;
    return a;
}

int main() {
    std::ios_base::sync_with_stdio(false);

    int numGraypes;
    while(std::cin >> numGraypes) {
        if(numGraypes==0) break;
        std::vector<K::Point_2> graype(numGraypes);
        Triangulation t;
        for(int i=0; i<numGraypes;i++) {
            double x, y;
            std::cin >> x >> y;
            graype[i] = K::Point_2(x,y);
        }
        t.insert(graype.begin(), graype.end());

        Segment minEdge;
        bool firstEdge = true;
        // output all edges
        for(Edge_iterator e = t.finite_edges_begin(); e != t.finite_edges_end(); ++e) {
            if(firstEdge == true || minEdge.squared_length() > t.segment(e).squared_length()) {
              firstEdge = false;
              minEdge = t.segment(e);
            }
        }
        std::cout << ceil_to_double(50*sqrt(minEdge.squared_length())) << std::endl;
    }
}
