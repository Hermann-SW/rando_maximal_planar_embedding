// cpplint --filter=-legal/copyright draw_planar.cpp
//
#include "draw_planar.hpp"
#include <CGAL/Exact_predicates_inexact_constructions_kernel.h>
#include <CGAL/Polyhedron_3.h>
#include <CGAL/IO/Polyhedron_iostream.h>
#include <fstream>
#include <cassert>

typedef CGAL::Exact_predicates_inexact_constructions_kernel  Kernel;
typedef CGAL::Polyhedron_3<Kernel>                       Polyhedron;
typedef Kernel::Point_3                                     Point_3;
typedef Kernel::Vector_3                                   Vector_3;

int main(int argc, char* argv[]) {
  assert(argc == 2);

  Polyhedron P;
  std::ifstream in1(argv[1]);
  in1 >> P;

  for (auto it = P.points_begin(); it != P.points_end(); ++it)
      if (abs(it->z()) > 1e-6) {
          std::cout << "not all points z-coordinate are 0" << std::endl;
          exit(1);
      }

  *P.points().first -= Vector_3(0, 0, 1);
  CGAL::draw(P);

  return EXIT_SUCCESS;
}
