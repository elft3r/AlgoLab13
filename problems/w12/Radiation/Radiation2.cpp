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
typedef CGAL::Quadratic_program<ET> Program;
typedef CGAL::Quadratic_program_solution<ET> Solution;

using namespace std;


vector<vector<double> > powArray(2048,vector<double>(31,-1));

struct Point3 {
    int x,y,z;
};

Point3 P(int x, int y, int z) {
    Point3 p;
    p.x = x;
    p.y = y;
    p.z = z;
    return p; 
}

double getPowerArray(int x, int y) {
    double tmp;
    if(powArray[x+1024][y] == -1) {
        tmp = pow(x,y);
        powArray[x+1024][y] = tmp;
        return tmp;
    } else {
        return powArray[x+1024][y];
    }
}

int midpoint(int lowerBound, int upperBound) {
    return lowerBound + (upperBound-lowerBound)/2;
}

bool testDegree(vector<Point3> &cells, int numHealthy, int degree) {
    Program qp (CGAL::SMALLER, false, 0, false, 0); 
    for(int c=0; c<cells.size(); c++) {
        int row = c;
        int rowIndex = 0;
        for(int i=0; i <= degree; i++) {
            for(int j=0; j <= degree-i; j++) {
                for(int k=0; k <= degree-i-j; k++) {
                    qp.set_a(rowIndex, row, 
                            getPowerArray(cells[c].x, i)
                            * getPowerArray(cells[c].y, j)
                            * getPowerArray(cells[c].z, k)
                            // pow(cells[c].x, i) * pow(cells[c].y, j) * pow(cells[c].z, k)
                            ); 
                    if(row<numHealthy) {
                        //healthy cell
                        qp.set_b(row,-1);
                        qp.set_r(row, CGAL::SMALLER);
                    } else {
                        //tumor cell
                        qp.set_b(row,1);
                        qp.set_r(row, CGAL::LARGER);
                    }
                    ++rowIndex;
                }
            }
        }
        // qp.set_d(row,row,1);
    }
    CGAL::Quadratic_program_options options;
    options.set_pricing_strategy(CGAL::QP_BLAND);
    Solution s = CGAL::solve_linear_program(qp, ET(), options);
    assert(s.solves_linear_program(qp));

    return !s.is_infeasible();
}

int findBestDegree(vector<Point3> &cells, int numHealthy) {
    const int maxLinear = -1;
    for(int degree = 0; degree <= maxLinear; degree++) {
        if(testDegree(cells, numHealthy, degree)) {
            return degree;
        }
    }

    int degreeMin = maxLinear+1;
    int degreeMax = 30;
    while(degreeMax >= degreeMin) {  
        int degree = midpoint(degreeMin, degreeMax);

        if(testDegree(cells, numHealthy, degree)) {
            degreeMax = degree-1;
        } else {
            degreeMin = degree+1;
        }
    }

    return degreeMin;
}

void testCase() {
    int numHealthy, numTumor;
    cin>>numHealthy>>numTumor;

    vector<Point3> cells(numHealthy+numTumor);

    for(int i=0; i<cells.size(); i++) {
        int x,y,z;
        cin>>x>>y>>z;
        cells[i] = P(x,y,z);
    }

    int degree = findBestDegree(cells, numHealthy);
    if(degree <= 30) {
        cout<< degree << std::endl;
    } else {
        cout << "Impossible!" << std::endl;
    }
}

int main() {
    ios_base::sync_with_stdio(false);
    int testCases;
    cin >> testCases;
    while(testCases--) { 
        testCase();
    }
}

