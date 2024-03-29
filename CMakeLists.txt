cmake_minimum_required(VERSION 3.10)
project(Project)

set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR}/Release/lib)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR}/Release/lib)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR}/Release/)

set(SOURCES src/expression.cpp src/operation.cpp)

# PROGRAM
find_package(Protobuf REQUIRED)
include_directories(${Protobuf_INCLUDE_DIRS})
include_directories(${CMAKE_CURRENT_BINARY_DIR})
protobuf_generate_cpp(PROTO_SRCS PROTO_HDRS proto/onnx.proto)
add_executable(onnx-executor src/main.cpp ${SOURCES} ${PROTO_SRCS} ${PROTO_HDRS})
target_link_libraries(onnx-executor ${Protobuf_LIBRARIES})

# TESTS
enable_testing()
find_package(GTest REQUIRED)
add_executable(tests tests/tests.cpp ${SOURCES} ${PROTO_SRCS} ${PROTO_HDRS})
target_link_libraries(tests ${Protobuf_LIBRARIES})
target_link_libraries(tests GTest::GTest GTest::Main)
add_test(unit-tests tests)


