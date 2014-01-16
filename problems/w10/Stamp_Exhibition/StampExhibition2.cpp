#include <iostream>
#include <utility>
#include <cmath>
#include <cassert>
#include <CGAL/basic.h>
#include <CGAL/QP_models.h>
#include <CGAL/QP_functions.h>
#include <CGAL/Exact_predicates_inexact_constructions_kernel.h>

// choose exact integral type
#ifdef CGAL_USE_GMP
#include <CGAL/Gmpzf.h>
typedef CGAL::Gmpzf ET;
#else
#include <CGAL/MP_Float.h>
typedef CGAL::MP_Float ET;
#endif

using namespace std;

// program and solution types
typedef pair<int,int> Pos;
typedef CGAL::Exact_predicates_inexact_constructions_kernel K;
typedef CGAL::Quadratic_program<ET> Program;
typedef CGAL::Quadratic_program_solution<ET> Solution;
typedef K::Point_2 P;
typedef K::Segment_2 S;


int main() {
    ios_base::sync_with_stdio(false);

    int testCases;
    cin>>testCases;
    for(int tcase=0; tcase<testCases; tcase++)  {
        Program lp (CGAL::SMALLER, true, 1, true, 4096); 

	int numLights, numStamps, numWalls;
	cin >> numLights >> numStamps >> numWalls;
	vector<P> lights(numLights);
	vector<P> stamps(numStamps);
	vector<S> walls(numWalls);
	vector<long> stampIntensity(numStamps);

	for(int l=0; l<numLights; l++) {
	    int x,y;
	    cin>>x>>y;
	    lights[l]=P(x,y);
	    lp.set_c(l,1);
	}

	for(int s=0; s<numStamps; s++) {
	    int x,y,intensity;
	    cin>>x>>y>>intensity;
	    stamps[s] = P(x,y);
	    stampIntensity[s] = intensity;

	}

	for(int w=0; w<numWalls; w++) {
	    int x1,y1,x2,y2;
	    cin>>x1>>y1>>x2>>y2;
        walls[w] = S(P(x1,y1),P(x2,y2));
	}


	for(int s=0; s<numStamps; s++) {
	    for(int l=0; l<numLights; l++) {
            S segment =S(stamps[s],lights[l]);
            K::FT r2 = segment.squared_length();
            double quotient2 = 1.0/r2;
            lp.set_a(l,s,quotient2);
            lp.set_a(l,numStamps+s,quotient2);
            for(int w=0; w<numWalls; w++) {
                if(CGAL::do_intersect(walls[w], segment)) {
                // cout<<"stamp"<<s<<" lamp"<<l<<" " <<quotient2;
                    lp.set_a(l,s,0);
                    lp.set_a(l,numStamps+s,0);
                    break;
                }

            }
	    }
        lp.set_b(s, stampIntensity[s]);
        lp.set_r(numStamps+s, CGAL::LARGER); 
        lp.set_b(numStamps+s, 1);
    }

	Solution s=CGAL::solve_linear_program(lp,ET());
	assert(s.solves_quadratic_program(lp));
	// CGAL::print_linear_program(std::cerr, lp, "lp");

	if(s.is_optimal()) 
	    cout<< "yes" << endl;
	else
	    cout << "no" << endl;
    }
}

