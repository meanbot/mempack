
cmake_minimum_required(VERSION 3.20.0)


project(mempack_perf_tests 
  HOMEPAGE_URL https://github.com/meanbot/mempack 
  DESCRIPTION "mempack - persistent memory allocator - perf tests" 
  VERSION 0.0.1 
  LANGUAGES CXX)

include(../../cmake/CPM.cmake)

CPMAddPackage("gh:fmtlib/fmt#10.1.1")
CPMAddPackage(
  NAME benchmark
  GITHUB_REPOSITORY google/benchmark
  VERSION 1.9.0
  OPTIONS "BENCHMARK_ENABLE_TESTING Off"
)

CPMAddPackage( 
  NAME meanbot_mempack 
  SOURCE_DIR "${CMAKE_CURRENT_SOURCE_DIR}/../.."
)

add_executable(${PROJECT_NAME} 
  mempack_perf_tests.cpp 
)

target_compile_features(${PROJECT_NAME} PRIVATE cxx_std_17)

target_include_directories(${PROJECT_NAME} PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/../../include)

target_link_libraries(${PROJECT_NAME} benchmark::benchmark)
target_link_libraries(${PROJECT_NAME} fmt::fmt-header-only)
target_link_libraries(${PROJECT_NAME} meanbot::mempack)

set(CPACK_PROJECT_NAME ${PROJECT_NAME})
set(CPACK_PROJECT_VERSION ${PROJECT_VERSION})
include(CPack)
