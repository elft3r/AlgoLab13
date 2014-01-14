#include <CGAL/Exact_predicates_inexact_constructions_kernel.h>
#include <CGAL/Delaunay_triangulation_2.h>
#include <CGAL/Triangulation_face_base_with_info_2.h>
#include <iostream>
#include <vector>
#include <algorithm>

typedef CGAL::Exact_predicates_inexact_constructions_kernel K;
typedef CGAL::Triangulation_vertex_base_2<K> Vb;
typedef CGAL::Triangulation_face_base_with_info_2<K::FT,K> Fb;
typedef CGAL::Triangulation_data_structure_2<Vb,Fb> Tds;
typedef CGAL::Delaunay_triangulation_2<K,Tds>  Triangulation;
typedef Triangulation::Face_iterator Face_iterator;
typedef Triangulation::All_vertices_iterator Vertex_iterator;
typedef Triangulation::Face_handle Face_handle;
typedef Triangulation::Edge Edge;
typedef CGAL::Point_2<K> Point;


struct faceComparator {
    bool operator() (const Face_handle &x, const Face_handle &y) const {
        return (x->info() > y->info());
    }
};

bool isInfinite(Triangulation &t, Face_handle &f) {
    return t.is_infinite(Edge(f,0))
            || t.is_infinite(Edge(f,1))
            || t.is_infinite(Edge(f,2));

}

bool has_infinite_vertex(Triangulation &t, Face_handle &f) {
    return t.is_infinite(f->vertex(0))
        || t.is_infinite(f->vertex(1))
        || t.is_infinite(f->vertex(2));
}

void initFaces(Triangulation &t, int startValue) {
    //initialize escape radius
    for (Triangulation::All_faces_iterator i = t.all_faces_begin(); 
            i != t.all_faces_end(); i++) {
        i->info() = K::FT(startValue);
    }
}


void maxEscapeRadiusPerFace(Triangulation &t) {
    //initialize escape radius
    for (Face_iterator i = t.finite_faces_begin(); 
            i != t.finite_faces_end(); i++) {
        i->info() = K::FT(0);
    }


    //initialize boundary
    Triangulation::Face_circulator fib = t.incident_faces(t.infinite_vertex());
    Triangulation::Face_circulator fie = fib;

    do {
        for(int i=0; i<3; i++) {
            if(!t.is_infinite(fib->neighbor(i))) {
                fib->neighbor(i)->info() 
                    = t.segment(Triangulation::Edge(fib,i)).squared_length()/4;
                Face_handle mf = fib->neighbor(i);
                break;
            }
        }
    } while(++fib != fie);

    //build search data structure -> Dijkstra
    std::set<Face_handle, faceComparator> faces;
    for(Face_iterator i=t.finite_faces_begin(); 
            i!=t.finite_faces_end(); i++) {
        faces.insert(Face_handle(i));
    }

    while(!faces.empty()) {
        //remove face with the best escape radius
        Face_handle mf = *(faces.begin());
        faces.erase(faces.begin());
        // std::cout << mf->info() << ": " << mf->vertex(1)->point() << "," << mf->vertex(2)->point() << "," << mf->vertex(3)->point() << std::endl;
        //and relax all edges
        for(int i=0; i<3; i++) {
            if(!t.is_infinite(mf->neighbor(i))) {
                K::FT tentativeRadius = t.segment(Edge(mf,i)).squared_length()/4;
                K::FT w;
                if(mf->info() < tentativeRadius) {
                    w = mf->info();
                } else {
                    w = tentativeRadius;
                }

                if(w > mf->neighbor(i)->info()) {
                        faces.erase(mf->neighbor(i));
                        mf->neighbor(i)->info() = w;
                        faces.insert(mf->neighbor(i));
                }
            }
        }
    }
}


bool canEscape(Triangulation &t, Point &p, K::FT &squaredRadius) {
    if(squaredRadius <= 0) {
        std::cout << "one" << std::endl;
        return true;
    }

    Face_handle f = t.locate(p);
    
    // std::cout << CGAL::squared_distance(p, t.nearest_vertex(p,f)->point()) << " " << squaredRadius << std::endl; 
    if(CGAL::squared_distance(p, t.nearest_vertex(p,f)->point()) < squaredRadius) {
        // std::cout << "three" << std::endl;
        return false;
    }

    if(t.is_infinite(f)) {
        // std::cout << "two" << std::endl;
        return true;
    }

    std::cout << f->info() << ": " << f->vertex(0)->point() << "," << f->vertex(1)->point() << "," << f->vertex(2)->point() << (has_infinite_vertex(t,f) ? " has infinite vertex" : "") << std::endl;
    return(f->info() >= squaredRadius);
}

bool canEscapeSlow(Triangulation &t, Point &s, K::FT &r, int queryNumber) {
    int i = queryNumber;
    if(r <= 0)
        return true;
    Face_handle f = t.locate(s);
    if(CGAL::squared_distance(s, t.nearest_vertex(s, f)->point()) < r)
        return false;
    
    // DFS
    std::vector<Face_handle> stack;
    stack.push_back(f);
    f->info() = i;
    while(!stack.empty()) {
        f = stack.back();
        stack.pop_back();
        // std::cout << f->info() << ": " << f->vertex(0)->point() << "," << f->vertex(1)->point() << "," << f->vertex(2)->point() << (has_infinite_vertex(t,f) ? " has infinite vertex" : "") << std::endl;
        if(t.is_infinite(f))
            return true;
        for(int j = 0; j < 3; ++j) {
            if(f->neighbor(j)->info() < i 
                    && t.segment(Triangulation::Edge(f,j)).squared_length() >= 4* r) {
                stack.push_back(f->neighbor(j));
                // std::cout << "push back" << std::endl;
                f->neighbor(j)->info() = i;
            }
        }
    }
    // std::cout << "four" << std::endl;
    return false;
}

int main() {
    std::ios_base::sync_with_stdio(false);

    int numVertices;
    while(std::cin >> numVertices) {
        if(numVertices <= 0) break;

        std::vector<Point> verticesList(numVertices);
        for(int i=0; i<numVertices; i++) {
            int x,y;
            std::cin >> x >> y;
            Point p(x,y);
            verticesList[i] = p;
        }

        Triangulation t;
        t.insert(verticesList.begin(), verticesList.end());
        // maxEscapeRadiusPerFace(t);

        // std::cout << "faces: " << std::endl;
        // for(Face_iterator i = t.faces_begin(); i !=t.faces_end(); i++) {
        //     std::cout << i->info() << ": " << i->vertex(1)->point() << "," << i->vertex(2)->point() << "," << i->vertex(3)->point() << std::endl;
        // }
        // std::cout << "faces end " << std::endl;
        initFaces(t,-1);

        int numCircles;
        std::cin >> numCircles;
        for(int i=0; i<numCircles; i++) {
            int x,y;
            K::FT squaredRadius;
            std::cin >> x >> y >> squaredRadius;
            Point p(x,y);
            std::cout << (canEscapeSlow(t, p, squaredRadius,i) ? "y" : "n");
        }

        std::cout << std::endl;
    }
}

