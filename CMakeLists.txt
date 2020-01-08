cmake_minimum_required (VERSION 3.7)
project (MINBUILD)

add_definitions(-DBOOST_ERROR_CODE_HEADER_ONLY) # https://bit.ly/2ny3GON
# A change
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR})

add_compile_options(
  "-ggdb" "-std=c++11"
)

add_subdirectory (src)
