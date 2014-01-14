#include <CGAL/Exact_predicates_inexact_constructions_kernel.h>
#include <CGAL/Delaunay_triangulation_2.h>
#include <iostream>
#include <cmath>

typedef CGAL::Exact_predicates_inexact_constructions_kernel K;
typedef CGAL::Delaunay_triangulation_2<K>  Triangulation;
typedef Triangulation::Edge_iterator  Edge_iterator;
typedef CGAL::Segment_2<K> Segment;
int main() {
    std::ios_base::sync_with_stdio(false);

    int numBistros;
    while(std::cin >> numBistros) {
        if(numBistros==0) break;

        // std::cout << numBistros << std::endl;
        std::vector<K::Point_2> bistro(numBistros);
        Triangulation t;
        for(int i=0; i<numBistros;i++) {
            double x, y;
            std::cin >> x >> y;
            bistro[i] = K::Point_2(x,y);
            // std::cout << bistro[i] << std::endl;
        }
        t.insert(bistro.begin(), bistro.end());

        int numNewBistros;
        std::cin >> numNewBistros;
        // std::cout << numNewBistros << std::endl;
        for(int i=0; i<numNewBistros;i++) {
            double x, y;
            std::cin >> x >> y;
            K::Point_2 newBistro(x,y);
            Triangulation::Vertex_handle nearestBistro = t.nearest_vertex(newBistro);
            Segment shortestPath(nearestBistro->point(), newBistro);
            std::cout << std::setiosflags(std::ios::fixed) << std::setprecision(0)
                << shortestPath.squared_length() << std::endl;
        }

        
    }
}


