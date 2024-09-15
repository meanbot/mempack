
cmake_minimum_required(VERSION 3.20.0)


project(mempack 
  HOMEPAGE_URL https://github.com/meanbot/mempack
  DESCRIPTION "mempack - persistent memory allocator" 
  VERSION 0.0.1 
  LANGUAGES CXX)

include(cmake/CPM.cmake)

CPMAddPackage("gh:fmtlib/fmt#10.1.1")

CPMAddPackage(
  NAME mimalloc 
  GITHUB_REPOSITORY microsoft/mimalloc 
  VERSION "2.1.7" 
  DOWNLOAD_ONLY YES
)

CPMAddPackage(
  NAME rpmalloc 
  GITHUB_REPOSITORY mjansson/rpmalloc 
  GIT_TAG "1.4.5" 
  DOWNLOAD_ONLY YES
)

CPMAddPackage( 
  NAME meanbot_result 
  SOURCE_DIR "${CMAKE_CURRENT_SOURCE_DIR}/../result"
)

add_library(${PROJECT_NAME} STATIC 
  src/precomp.cpp 
  src/storage/segment.cpp 
  "src/storage/unused_segments_sorted_array.cpp" 
  src/storage/clusters_array.cpp 
  src/storage/storage.cpp 
  "${mimalloc_SOURCE_DIR}/src/static.c" 
  "${rpmalloc_SOURCE_DIR}/rpmalloc/rpmalloc.c" 
)

add_library(meanbot::${PROJECT_NAME} ALIAS ${PROJECT_NAME})

target_compile_features(${PROJECT_NAME} PUBLIC cxx_std_17)
target_compile_options(${PROJECT_NAME} PUBLIC "$<$<COMPILE_LANG_AND_ID:CXX,MSVC>:/permissive->")

target_include_directories(${PROJECT_NAME} PUBLIC "${CMAKE_CURRENT_SOURCE_DIR}/include")

target_include_directories(${PROJECT_NAME} PRIVATE "${CMAKE_CURRENT_SOURCE_DIR}/src")

target_include_directories(${PROJECT_NAME} PRIVATE "${mimalloc_SOURCE_DIR}/include")
target_include_directories(${PROJECT_NAME} PRIVATE "${rpmalloc_SOURCE_DIR}/rpmalloc")

target_link_libraries(${PROJECT_NAME} fmt::fmt-header-only)
target_link_libraries(${PROJECT_NAME} meanbot::result)

target_precompile_headers(${PROJECT_NAME} PRIVATE "${CMAKE_CURRENT_SOURCE_DIR}/src/precomp.hpp")

set(CPACK_PROJECT_NAME ${PROJECT_NAME})
set(CPACK_PROJECT_VERSION ${PROJECT_VERSION})
include(CPack)
