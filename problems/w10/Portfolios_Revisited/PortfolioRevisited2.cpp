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

int midPoint(int lowerBound, int upperBound) {
    return lowerBound + (upperBound-lowerBound)/2;
}

int main() {
    int numAssets, numPeople;
    while(std::cin >> numAssets >> numPeople) {
        if(numAssets == 0 && numPeople == 0)
            break;

        Program qp (CGAL::SMALLER, true, 0, false, 0); 
        std::vector<int> costs(numAssets);
        std::vector<int> returns(numAssets);
        std::vector<double> returnPerCost(numAssets);
        for(int i=0; i<numAssets; i++) {
            int cost, expectedReturn;
            std::cin >> cost >> expectedReturn;
            qp.set_a(i, 0, expectedReturn);
            qp.set_r(0, CGAL::LARGER);
            qp.set_a(i, 1, cost);
            costs[i] = cost;
            returns[i] = expectedReturn;
            returnPerCost[i] = (double)expectedReturn/(double)cost;
        }
        for(int i=0; i<numAssets; i++) {
            for(int j=0; j<numAssets; j++) {
                int covariance;
                std::cin >> covariance;
                if(j<=i)
                    qp.set_d(i,j,2*covariance);
            }

        }
        for(int i=0; i<numPeople; i++) {
            int maxCost, minReturn, maxVariance;
            std::cin >> maxCost >> maxVariance;
            qp.set_b(1, maxCost); 

            int indexMinCost = std::max_element(returnPerCost.begin(), returnPerCost.end()) - returnPerCost.begin();
            long returnUpperBound = ceil(returns[indexMinCost]*maxCost/costs[indexMinCost])+1;

            // the following lower bound is not correct, since it might not be possible to buy all high cost assets because this might exceed the risk
            // long returnLowerBound = returns[indexMaxCost]*(double)maxCost/costs[indexMaxCost];

            long returnLowerBound = 0;

            // binary search
            while(returnLowerBound <= returnUpperBound) {
                long maxReturn = midPoint(returnLowerBound,returnUpperBound);

                qp.set_b(0, maxReturn); 
                Solution s= CGAL::solve_quadratic_program(qp, ET());
                assert(s.solves_quadratic_program(qp));
                if(s.is_optimal() && s.objective_value() <= maxVariance) {
                    returnLowerBound = maxReturn+1;
                } else {
                    returnUpperBound = maxReturn-1;
                }
            }
            std::cout << returnUpperBound << std::endl;
        }
    }
}
