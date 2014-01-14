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

double ceil_to_double(const CGAL::Quotient<ET>& x) {
    double a = std::ceil(CGAL::to_double(x));
    while(a<x) a+=1;
    while(a-1>=x) a-=1;
    return a;
}

double floor_to_double(const CGAL::Quotient<ET>& x) {
    double a = std::floor(CGAL::to_double(x));
    while(a>x) a-=1;
    while(a+1<=x) a+=1;
    return a;
}

int main() {

  int problemType;
  while(std::cin >> problemType) {
        if(problemType==0) break;
        int a,b;
        std::cin >> a >> b;

        const int X = 0;
        const int Y = 1;
        Program lp (CGAL::SMALLER, false, 0, false, 0); 

        if(problemType==1) {
            //minimize the negative problem
            lp.set_c(Y, -b);
            lp.set_d(X,X,2*a);

            lp.set_l(X,true,0);
            lp.set_l(Y,true,0);

            lp.set_a(X,0,1); lp.set_a(Y,0,1); lp.set_b(0, 4);
            lp.set_a(X,1,4); lp.set_a(Y,1,2); lp.set_b(1, a*b);
            lp.set_a(X,2,-1); lp.set_a(Y,2,1); lp.set_b(2, 1);
        } else {
            const int Z = 2;
            lp.set_d(X,X,2*a);
            lp.set_d(Z,Z,2);
            lp.set_c(Y, b);

            lp.set_u(X,true,0);
            lp.set_l(X,false);
            lp.set_u(Y,true,0);
            lp.set_l(Y,false);
            lp.set_u(Z,false);
            lp.set_l(Z,false);

            lp.set_a(X,0,1); lp.set_a(Y,0,1); lp.set_b(0, -4);
            lp.set_r(0, CGAL::LARGER);
            lp.set_a(X,1,4); lp.set_a(Y,1,2); lp.set_a(Z,1,1); 
            lp.set_b(1, -a*b);
            lp.set_r(1, CGAL::LARGER);
            lp.set_a(X,2,-1); lp.set_a(Y,2,1); lp.set_b(2, -1);
            lp.set_r(2, CGAL::LARGER);

        }
        Solution s= CGAL::solve_quadratic_program(lp, ET());
        assert(s.solves_quadratic_program(lp));

        // std::cout << s;

        if(s.is_infeasible()) 
            std::cout << "no" << std::endl;
        else if(s.is_unbounded()) 
            std::cout << "unbounded" << std::endl;
        else {
            CGAL::Quotient<ET> exactValue = s.objective_value();
            int value = problemType==1 ? 
                            floor_to_double(-exactValue) : 
                            ceil_to_double(exactValue);

            std::cout << value << std::endl;
        }
  }
}





