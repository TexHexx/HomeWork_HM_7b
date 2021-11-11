# CMakeList.txt : CMake project for HomeWork_HM_7b, include source and define
# project specific logic here.
#
cmake_minimum_required (VERSION 3.8)

project ("HomeWork_HM_7b")
find_package(Protobuf CONFIG REQUIRED)
include_directories(${Protobuf_INCLUDE_DIRS})
include_directories(${CMAKE_CURRENT_BINARY_DIR})
add_executable(${PROJECT_NAME} "HomeWork_HM_7b.cpp" "HomeWork_HM_7b.h" ${PROTO_SRCS} ${PROTO_HDRS})
target_link_libraries(HomeWork_HM_7b PRIVATE protobuf::libprotoc protobuf::libprotobuf protobuf::libprotobuf-lite)

# TODO: Add tests and install targets if needed.
