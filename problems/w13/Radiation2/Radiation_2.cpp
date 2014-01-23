#include <iostream>
#include <cassert>
#include <CGAL/basic.h>
#include <CGAL/QP_models.h>
#include <CGAL/QP_functions.h>
#include <CGAL/Exact_predicates_exact_constructions_kernel.h>
#include <CGAL/Delaunay_triangulation_2.h>


// choose exact integral type
#ifdef CGAL_USE_GMP
#include <CGAL/Gmpz.h>
typedef CGAL::Gmpz ET;
#else
#include <CGAL/MP_Float.h>
typedef CGAL::MP_Float ET;
#endif

// #define DEBUG

#ifdef DEBUG
#define D(x) x
#else 
#define D(x)
#endif

using namespace std;

// program and solution types
typedef CGAL::Quadratic_program<ET> Program;
typedef CGAL::Quadratic_program_solution<ET> Solution;
typedef pair<double,double> Point2;

typedef CGAL::Exact_predicates_exact_constructions_kernel K;
typedef CGAL::Delaunay_triangulation_2<K>  Triangulation;
typedef Triangulation::Edge_iterator  Edge_iterator;
typedef Triangulation::Face_handle  Face_handle;
typedef CGAL::Segment_2<K> Segment;
typedef CGAL::Point_2<K> P;
typedef Triangulation::Finite_faces_iterator Fiter;


//does not work 
void lpApproach(vector<Point2> &cells, int numHealthy) {
    Program program (CGAL::SMALLER, false, 0, false, 0); 
    const int numW = 5;
    for(int row=0; row<cells.size(); row++) {
        program.set_a(0, row, 1); 
        program.set_a(1, row, cells[row].first); 
        program.set_a(2, row, cells[row].second); 
        program.set_a(3, row, cells[row].first * cells[row].first); 
        program.set_a(4, row, cells[row].second * cells[row].second); 

        //slack variables
        for(int i=0; i<cells.size(); i++) {
            if(i==row && row>=numHealthy) {
                program.set_a(numW+i, row, 1); 
            } else {
                program.set_a(numW+i, row, 0); 
            }
        }
        if(row<numHealthy) {
            //healthy cell
            program.set_b(row,-1);
            program.set_r(row, CGAL::SMALLER);
        } else {
            //tumor cell
            program.set_b(row,1);
            program.set_r(row, CGAL::LARGER);
        }

    }
    //slack variables
    for(int i=0; i<numW; i++) {
        int row = cells.size()+i;
        for(int j=0; j<cells.size()+numW; j++) {
            program.set_a(j, row, 0); 
        }
        program.set_r(row, CGAL::EQUAL);
        program.set_b(row,0);
        program.set_c(i, 1);
    }
    for(int i=0; i<cells.size(); i++) {
        program.set_c(numW+i, 1);
        program.set_l(numW+i, true, 0);
    }

    CGAL::Quadratic_program_options options;
    options.set_pricing_strategy(CGAL::QP_BLAND);
    Solution s = CGAL::solve_linear_program(program, ET(), options);
    assert(s.solves_linear_program(program));

    if(s.is_infeasible()) {
        // cout<<program.get_m()<<std::endl;
        // cout<<program.get_n()<<std::endl;

        cout<<"not feasible"<<std::endl;
        return;
    } else if (s.is_unbounded()) {
        cout<<(cells.size() - numHealthy)<<std::endl;
        return;
    }

    int countNotZero=0;
    for(Solution::Variable_value_iterator sol_it = s.variable_values_begin(); sol_it != s.variable_values_end(); sol_it++) {
        int index = sol_it - s.variable_values_begin();
        // cout<<*sol_it<<std::endl;
        if(index > numW-1 && *sol_it != 0) {
            ++countNotZero;
        }
    }
    int countDestroyed = (cells.size() - numHealthy) - countNotZero;

    // CGAL::print_quadratic_program(std::cerr, program, "lp");

    cout<<countDestroyed<<std::endl;
}

void delauneyApproach(vector<Point2> &cells, int numHealthy) {
    vector<P> goodCells;
    vector<P> badCells;

    for(int i=0; i<cells.size(); i++) {
        if(i<numHealthy) 
            goodCells.push_back(P(cells[i].first, cells[i].second));
        else 
            badCells.push_back(P(cells[i].first, cells[i].second));

    }

    Triangulation goodTria;
    goodTria.insert(goodCells.begin(), goodCells.end());

    map<Face_handle, set<int> > conflictMap;

    for(int i=0; i<badCells.size(); i++) {
        vector<Face_handle> conflictFaces;
        goodTria.get_conflicts(badCells[i], back_inserter(conflictFaces));
        for(int j=0; j<conflictFaces.size(); j++) {
            map<Face_handle,set<int> >::iterator it = conflictMap.find(conflictFaces[j]);
            if(it == conflictMap.end()) {
                conflictMap[conflictFaces[j]] = set<int>();
            }
            conflictMap[conflictFaces[j]].insert(i);
            D(cout<<"face " << j << " in conflict with bad cell " << i<<endl;)
        }
    }

    int maxDestroyed=0;
    for(Edge_iterator e = goodTria.finite_edges_begin(); e != goodTria.finite_edges_end(); ++e) {
        Segment eSegment = goodTria.segment(e);
        Face_handle f1, f2;
        f1 = e->first;
        f2 = f1->neighbor(e->second);
        set<int> conflicts1 = conflictMap[f1];
        set<int> conflicts2 = conflictMap[f2];
        vector<int> conflict_intersection;
        set_intersection(conflicts1.begin(), conflicts1.end(),
                            conflicts2.begin(), conflicts2.end(),
                            back_inserter(conflict_intersection));

        vector<int> conflict_difference;
        set_difference(conflicts1.begin(), conflicts1.end(),
                            conflicts2.begin(), conflicts2.end(),
                            back_inserter(conflict_difference));

        int maxPointDestroys=0;
        for(vector<int>::iterator it = conflict_difference.begin(); it!=conflict_difference.end();
                it++) {
            int thisPointDestroys = 0;

            for(vector<int>::iterator it2 = conflict_difference.begin(); it2!=conflict_difference.end();
                    it2++) {
                if(side_of_bounded_circle(eSegment.source(), eSegment.target(), badCells[*it], badCells[*it2]) 
                        == CGAL::ON_BOUNDED_SIDE) {
                    if(*it!=*it2) { 
                        thisPointDestroys++;
                    }
                }
            }
            maxPointDestroys= max(maxPointDestroys, thisPointDestroys+1);
        }

        int cisize = conflict_intersection.size();
        maxDestroyed = max(maxDestroyed, maxPointDestroys+cisize);
      }

    cout<<maxDestroyed<<std::endl;
}



void testCase() {
    int numHealthy, numTumor;
    cin>>numHealthy>>numTumor;

    vector<Point2> cells(numHealthy+numTumor);

    for(int i=0; i<cells.size(); i++) {
        double x,y;
        cin>>x>>y;
        cells[i] = Point2(x,y);
    }

    // lpApproach(cells, numHealthy);
    delauneyApproach(cells, numHealthy);
}

int main() {
    ios_base::sync_with_stdio(false);
    int testCases;
    cin >> testCases;
    while(testCases--) { 
        testCase();
    }
}

