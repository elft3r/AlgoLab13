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

  int numNutrients, numFoods;
  while(std::cin >> numNutrients >> numFoods) {
      if(numNutrients == 0 && numFoods == 0)
          break;
      Program lp (CGAL::LARGER, true, 0, false, 0); 
      for(int nutrient=0;nutrient<numNutrients;nutrient++) {
          int l,u;
          std::cin >> l >> u;
          lp.set_b(2*nutrient, l);
          lp.set_b(2*nutrient+1, u);
          lp.set_r(2*nutrient+1, CGAL::SMALLER);
      }

      for(int food=0;food<numFoods;food++) {
          int price;
          std::cin >> price;
          for(int nutrient=0; nutrient<numNutrients; nutrient++) {
              int price,C;
              std::cin >> C;
              lp.set_a(food, 2*nutrient, C);
              lp.set_a(food, 2*nutrient+1, C);
          }
          lp.set_c(food, price);
      }

      Solution s= CGAL::solve_quadratic_program(lp, ET());
      assert(s.solves_quadratic_program(lp));

      if(!s.is_optimal()) 
          std::cout << "No such diet." << std::endl;
      else
          std::cout << floor_to_double(s.objective_value()) << std::endl;
  }
}

