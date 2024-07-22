cmake_minimum_required(VERSION 3.1...3.23)
project(random_maximal_planar_embedding_CGAL_demo)

add_compile_options(-O3 -Wall -Wextra -pedantic)

find_package(CGAL REQUIRED OPTIONAL_COMPONENTS Qt5)

create_single_source_cgal_program("demo.cpp")

if(CGAL_Qt5_FOUND)
  target_link_libraries(demo PUBLIC CGAL::CGAL_Basic_viewer)
endif()
