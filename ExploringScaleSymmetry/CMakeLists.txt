cmake_minimum_required(VERSION 2.8.9)
project (exploringScaleSymmetry)

find_package(Eigen3 REQUIRED)
  
include_directories(include Chapter6/automata2D Chapter6/automata3D ${EIGEN3_INCLUDE_DIR})
file(GLOB SOURCES "main.cpp" "*/*.cpp" "Chapter6/*/*.cpp")
 
add_compile_options(-Wno-deprecated)
add_executable(exploringScaleSymmetry ${SOURCES})
target_link_libraries(exploringScaleSymmetry GL glut GLU)


