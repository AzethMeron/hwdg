cmake_minimum_required(VERSION 3.4...3.18)
project(graphs)

add_subdirectory(pybind11)
pybind11_add_module(graphs src/pywrapper.cpp src/node.cpp src/edge.cpp src/node_in_graph.cpp src/graph.cpp src/operations.cpp src/tools.cpp src/dijkstra.cpp, src/path.cpp)

# EXAMPLE_VERSION_INFO is defined by setup.py and passed into the C++ code as a
# define (VERSION_INFO) here.
target_compile_definitions(graphs
                           PRIVATE VERSION_INFO=${EXAMPLE_VERSION_INFO})
