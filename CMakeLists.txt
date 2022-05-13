cmake_minimum_required(VERSION 3.0.0)
project(json_parser VERSION 0.1.0)

# build库
SET(JSON_SRC_DIR ${PROJECT_SOURCE_DIR}/src)
include_directories(${PROJECT_SOURCE_DIR}/include)
add_library(json_parser ${JSON_SRC_DIR}/json_parser.cpp ${JSON_SRC_DIR}/token.cpp)

# build测试用例
add_executable(test1 ${PROJECT_SOURCE_DIR}/test/test1.cpp)
target_link_libraries(test1 json_parser)