#include <iostream>
#include <vector>
#include <sstream>
#include <string.h>
#include <stdio.h>

using namespace std;

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
 * QP Debugging
 */
cout<<qp.get_n()<<std::endl;
cout<<qp.get_m()<<std::endl;
CGAL::print_quadratic_program(std::cerr, qp, "qp");

CGAL::Quadratic_program_options options;
options.set_pricing_strategy(CGAL::QP_BLAND);
Solution s = CGAL::solve_linear_program(qp, ET(), options);


