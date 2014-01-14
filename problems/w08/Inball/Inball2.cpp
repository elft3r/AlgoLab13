#include <iostream>
#include <cassert>
#include <CGAL/basic.h>
#include <CGAL/QP_models.h>
#include <CGAL/QP_functions.h>

// choose exact integral type
#ifdef CGAL_USE_GMP
#include <CGAL/Gmpz.h>
typedef CGAL::Gmpz ET;
#else
#include <CGAL/MP_Float.h>
typedef CGAL::MP_Float ET;
#endif

// program and solution types
typedef CGAL::Quadratic_program<int> Program;
typedef CGAL::Quadratic_program_solution<ET> Solution;

using namespace std;

double floor_to_double(const CGAL::Quotient<ET>& x) {
    double a = std::floor(CGAL::to_double(x));
    while(a>x) a-=1;
    while(a+1<=x) a+=1;
    return a;
}

int main() {
    ios_base::sync_with_stdio(false);

    int numInequalities, numDimensions;
    while(true) {
        cin >> numInequalities;
        if(numInequalities==0) break;
        cin >> numDimensions;

        Program lp (CGAL::SMALLER, false, 0, false, 0); 
        for(int i=0; i<numDimensions; i++) {
            lp.set_c(i,0);
        }
        lp.set_c(numDimensions,-1);

        for(int i=0; i<numInequalities; i++) {
            long normA=0;
            for(int j=0; j<numDimensions; j++) {
                int a;
                cin >> a;
                normA = normA + a*a;
                lp.set_a(j,i,a);
            }
            lp.set_a(numDimensions, i, sqrt(normA));
            int b;
            cin >> b;
            lp.set_b(i,b); 
        }
        lp.set_l(numDimensions, true, 0);
        Solution s= CGAL::solve_quadratic_program(lp, ET());
        assert(s.solves_quadratic_program(lp));

        if(s.is_infeasible()) 
            std::cout << "none" << std::endl;
        else if(s.is_unbounded()) 
            std::cout << "inf" << std::endl;
        else {
            CGAL::Quotient<ET> exactValue = s.objective_value();
            cout << floor_to_double(-exactValue) << endl;
        }
    }
}

        

