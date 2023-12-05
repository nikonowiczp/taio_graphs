# CMakeList.txt : CMake project for taio_lib, include source and define
# project specific logic here.
#
cmake_minimum_required (VERSION 3.8 )

# Add source to this project's executable.
project(taio_lib)
set(PROJECT_SOURCES
    "graph.cpp"
    "graph_runner.cpp"
)
set(PROJECT_HEADERS
    "graph.h"
    "graph_runner.h")
add_library (taio_lib STATIC ${PROJECT_HEADERS} ${PROJECT_SOURCES})
add_library(lib::taio_lib ALIAS taio_lib)

if (CMAKE_VERSION VERSION_GREATER 3.12)
  set_property(TARGET taio_lib PROPERTY CXX_STANDARD 20)
endif()

target_include_directories( taio_lib
    PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}
)