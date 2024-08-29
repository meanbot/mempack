
cmake_minimum_required(VERSION 3.20.0)


project(mempack_unit_tests 
  HOMEPAGE_URL https://github.com/meanbot/mempack 
  DESCRIPTION "mempack - persistent memory allocator - unit tests" 
  VERSION 0.0.1 
  LANGUAGES CXX)

include(../../cmake/CPM.cmake)

CPMAddPackage("gh:fmtlib/fmt#10.1.1")
CPMAddPackage("gh:catchorg/Catch2#v3.7.0")

CPMAddPackage( 
  NAME meanbot_mempack 
  SOURCE_DIR "${CMAKE_CURRENT_SOURCE_DIR}/../.."
)

add_executable(${PROJECT_NAME} 
  mempack_unit_tests.cpp 
)

target_compile_features(${PROJECT_NAME} PRIVATE cxx_std_17)

add_compile_definitions(CATCH_CONFIG_PREFIX_ALL)
add_compile_definitions(CATCH_CUSTOM_MAIN)

target_include_directories(${PROJECT_NAME} PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/../../include)

target_link_libraries(${PROJECT_NAME} fmt::fmt-header-only)
target_link_libraries(${PROJECT_NAME} Catch2::Catch2)
target_link_libraries(${PROJECT_NAME} meanbot::mempack)

set(CPACK_PROJECT_NAME ${PROJECT_NAME})
set(CPACK_PROJECT_VERSION ${PROJECT_VERSION})
include(CPack)
