cmake_minimum_required(VERSION 3.10)

project(taio_graphs)

# Set C++ standard
set(CMAKE_CXX_STANDARD 17)

# Add executable
add_executable(taio_graphs main.cpp graph.cpp graph_runner.cpp)

# Optionally, you can set the output directory for the executable
# set(EXECUTABLE_OUTPUT_PATH ${CMAKE_BINARY_DIR}/bin)
