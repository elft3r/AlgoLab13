#include <iostream>
#include <vector>
#include <sstream>
#include <string.h>
#include <stdio.h>

using namespace std;

/*
 * debug macro
 */
#define DEBUG
#ifdef DEBUG
#define D(x) x
#else 
#define D(x) 
#endif

/*
 * fast I/O
 */
ios_base::sync_with_stdio(false);

/*
 * output floating point numbers in fixed point notation
 * setprecision: number of digits after the decimal point
 */
std::cout<<std::setiosflags(std::ios::fixed) << std::setprecision(0)<< ceil_to_double(sqrt(CGAL::to_double(maxDist)))
            <<std::endl;

/*
 * number to string
 */
template <typename T>
    string NumberToString ( T Number )
    {
     ostringstream ss;
     ss << Number;
     return ss.str();
    }

/*
 * highest and lowest of orderable types
 */
#include <limits>
int max = numeric_limits<int>::max();
int min = numeric_limits<int>::min();

/*
 * enums
 */
enum enumeration_name {
  value1,
  value2,
  value3,
};

/*
 * power
 */
#include <math.h>
pow(7,3) //7^3

/*
 * sqrt
 */
#include <math.h>
double result = sqrt(1024.0);

#include <cstdlib>
double result = std::abs(-1024.0);

/* 
 * sum vector contents
 */
#include<numeric>
std::accumulate(newTopChipPosition.begin(),
                                    newTopChipPosition.end(),0);

/*
 * sort array
 */
#include<algorithm>
std::sort(parasols.begin(), parasols.end());

/*
 * find in vector
 */
std::find(aVector.begin(), aVector.end(), item)!=aVector.end()

/*
 * get index of max/min element in vector
 */
#include <algorithm>
std::vector<int> v(10,0);
int a[] = {1,2,3,2};
int maxIndex = std::max_element(v.begin(), v.end());
int maxIndex2 = std::max_element(a, a+4);

/* 
 * set intersection
 */
std::vector<string> inters;
std::set_intersection(chars[i].begin(), chars[i].end(),
    chars[j].begin(), chars[j].end(),
    std::back_inserter(inters));

/*
 * queue reverse order
 */
priority_queue<int, vector<int>, greater<int>> Q;

/*
 * comparator
 */
struct faceComparator {
    bool operator() (const Face_handle &x, const Face_handle &y) {
        return (x->info() < y->info());
    }
};
std::set<Face_handle, faceComparator> faces;

/*
 * non-static comparator
 */
struct CityComparator{
    P mainCity;
    bool operator()(P x, P y) {
        K::FT d1 = S(mainCity, x).squared_length();
        K::FT d2 = S(mainCity, y).squared_length();
        return(d1 <d2);
    }

};

/*
 * Binary Search
 */
int midpoint(int lowerBound, int upperBound) {
    return lowerBound + (upperBound-lowerBound)/2;
}

int binary_search(int A[], int key, int imin, int imax)
{
  // continue searching while [imin,imax] is not empty
  while (imax >= imin)
    {
      // calculate the midpoint for roughly equal partition
      int imid = midpoint(imin, imax);
      if(A[imid] == key)
        // key found at index imid
        return imid; 
      // determine which subarray to search
      else if (A[imid] < key)
        // change min index to search upper subarray
        imin = imid + 1;
      else         
        // change max index to search lower subarray
        imax = imid - 1;
    }
  // key was not found
  return KEY_NOT_FOUND;
}



/*
 * BOOST
 */
/*
 * print graph
 */
void printGraph(Graph g, WeightMap weight) {
    graph_traits<Graph>::edge_iterator eiter, eiter_end;
    for (tie(eiter, eiter_end) = edges(g); eiter != eiter_end; ++eiter) {
        std::cout << source(*eiter, g) << " <--> " << target(*eiter, g)
          << " with weight of " << weight[*eiter]
          << std::endl;
    }
}

/*
 * shortest path BFS
 */
int shortestPath(const Graph &g, int x, int y) {
        //BFS
        int n=num_vertices(g);
        vector<bool> visited(n, false);
        vector<int> distance(n, -1);
        std::queue<int> bfs_queue;

        vector<int> bfs_num(n);
        bfs_queue.push(x);
        visited[x] = true;
        distance[x] = 0;
        while (!bfs_queue.empty()) {
        int v = bfs_queue.front();
        bfs_queue.pop();
        //cerr << "visit  : " << v << "\n";
        GraphTraits::out_edge_iterator e, e_end;
        for (tie(e, e_end) = out_edges(v, g); e != e_end; ++e) {
            Vertex t = target(*e, g);
            if (!visited[t]) {
            visited[t] = true;
            distance[t] = distance[v] + 1;
            //cerr << "enqueue " << t << "\n";
            if(t==y) return distance[t];
            bfs_queue.push(t);
            }
        }
    }
    return 0;
}

/*
 * non-recursive DFS
 */
int main ()
{
    int n;
    vector<vector<int> > edges;
    read_graph(n, edges);
    /* 8< */
    vector<bool> visited(n, false);
    vector<int> dfs_stack;
    vector<int> dfs_neighbor_pos;
    int next_num = 0;
    vector<int> dfs_num(n);
    for (int u = 0; u < n; u++) {
	if (visited[u])
	    continue;
	dfs_stack.push_back(u);
	dfs_neighbor_pos.push_back(0);
	visited[u] = true;
	dfs_num[u] = next_num++;
	cerr << "visit " << dfs_num[u] << ": " << u << "\n";
	while (!dfs_stack.empty()) {
	    int v = dfs_stack.back();
	    int i = dfs_neighbor_pos.back();
	    dfs_stack.pop_back();
	    dfs_neighbor_pos.pop_back();
	    vector<int>& neighbors = edges[v];
	    for (; i < (int)neighbors.size(); i++) {
		int w = neighbors[i];
		if (!visited[w]) {
		    /* defer looking at v */
		    dfs_stack.push_back(v);
		    dfs_neighbor_pos.push_back(i+1);
		    /* look at w next */
		    visited[w] = true;
		    dfs_num[w] = next_num++;
		    dfs_stack.push_back(w);
		    dfs_neighbor_pos.push_back(0);
		    cerr << "visit " << dfs_num[w] << ": " << w << "\n";
		    break;
		}
		/* if we fall off this loop (as opposed to break), we
		 * are done looking at v */
	    }
	}
    }
    /* >8 */
}

/*
 * Boost named parameters and visitor
 */

vector<int> discoverTimeToVertex;

// Custom visitor used to record DFS order.
struct CustomVisitor : public default_dfs_visitor {
	void discover_vertex(int u, const Graph& G) {
		discoverTimeToVertex.push_back(u);
	}
};

vector<Vertex> low(numVertices);
vector<Vertex> vertexToDiscoverTime(numVertices);
vector<Vertex> predecessor(numVertices);
discoverTimeToVertex.clear();
size_t num_comps 
    = biconnected_components(g,component,
                                lowpoint_map(&low[0])
                                .discover_time_map(&vertexToDiscoverTime[0])
                                .predecessor_map(&predecessor[0])
                                .visitor(CustomVisitor()));

/*
 * Boost: defining a custom property
 */
namespace boost {
    enum edge_info_t { edge_info = 219 }; // A unique ID.
    BOOST_INSTALL_PROPERTY(edge, info);
}

struct EdgeInfo {
...
}

typedef adjacency_list<vecS, vecS, directedS,
    no_property, // vertex property, none this time
    property<edge_info_t, EdgeInfo> //edge property, could also use a predefined property like edge_name_t and define it with integers
    > Graph;
typedef property_map<Graph, edge_info_t>::type InfoMap;
...
InfoMap info_map = get(edge_info, G);
info_map[e] = ...

/*
 * CGAL: add property to a point
 */
template <typename I>
struct MyP : public K::Point_2 {
    MyP(const K::FT& x, const K::FT& y, const I& i_) : K::Point_2(x,y), i(i_) {}
    I i;
};
typedef MyP<std::pair<std::size_t,K::FT> > Participant;
Participant(x,y,make_pair(i,h));

/*
 * CGAL: Voronoi dual
 */
// process all Voronoi vertices
for (Face_iterator f = t.finite_faces_begin(); f != t.finite_faces_end(); ++f) {
    K::Point_2 p = t.dual(f);
    ...
}
// process all Voronoi edges
for(Edge_iterator e = t.finite_edges_begin(); e != t.finite_edges_end(); ++e) {
    CGAL::Object o = t.dual(e);
    // o can be a segment, a ray or a line ...
    ...
}

/*
 * CGAL: Enhancing faces/vertices
 */
//Can use maps for {Face,Vertices,Edges(with some additional work)}-handles
//or:
#include <CGAL/Triangulation_face_base_with_info_2.h>
enum Color { Black = 0, White = 1, Red = 2};
typedef CGAL::Exact_predicates_inexact_constructions_kernel K;
typedef CGAL::Triangulation_vertex_base_2<K> Vb;
//New face class, vertex class stays the same. With info parameter. Here each face gets a color
typedef CGAL::Triangulation_face_base_with_info_2<Color,K> Fb; 
//change the underlying triangulation structure
typedef CGAL::Triangulation_data_structure_2<Vb,Fb> Tds;
typedef CGAL::Delaunay_triangulation_2<K,Tds> Triangulation;

Triangulation t;

// color all infinite faces black
Triangulation::Face_circulator f = t.incident_faces(t.infinite_vertex());
do {
    f->info() = Black;
} while (++f != t.incident_faces(t.infinite_vertex()));


/*
 * QP Debugging
 */
cout<<qp.get_n()<<std::endl;
cout<<qp.get_m()<<std::endl;
CGAL::print_quadratic_program(std::cerr, qp, "qp");

CGAL::Quadratic_program_options options;
options.set_pricing_strategy(CGAL::QP_BLAND);
Solution s = CGAL::solve_linear_program(qp, ET(), options);


