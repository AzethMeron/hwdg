cmake_minimum_required(VERSION 3.0.0)
project(pybind_graphs)

# specify the C++ standard
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED True)

#compiler path
#set( CMAKE_CXX_COMPILER C:/MinGW/bin/g++.exe)
#set( CMAKE_C_COMPILER C:/MinGW/bin/gcc.exe)


#-O2 optimizations
set(CMAKE_CXX_FLAGS_RELEASE "-DNDEBUG -Wall -O2")

add_library(bellmanford STATIC bellmanford.cpp)
add_library(dijkstra STATIC dijkstra.cpp)
add_library(edge STATIC edge.cpp)
add_library(graph STATIC graph.cpp)
add_library(node_in_graph STATIC node_in_graph.cpp)
add_library(node STATIC node.cpp)
add_library(operations STATIC operations.cpp)
add_library(path STATIC path.cpp)
add_library(tools STATIC tools.cpp)


install(FILES custom_map.hpp DESTINATION include)
install(FILES hwdg.hpp DESTINATION include)
install(FILES legend.hpp DESTINATION include)
