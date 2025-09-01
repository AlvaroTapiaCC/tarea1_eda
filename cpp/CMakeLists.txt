cmake_minimum_required(VERSION 3.10)
project(clustering
VERSION 1.0
LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED True)

add_executable(main main.cpp src/utils.cpp src/matrix.cpp src/cluster.cpp src/simsearch.cpp)
target_include_directories(main PUBLIC ${PROJECT_SOURCE_DIR}/include)