project(sniffer)

cmake_minimum_required(VERSION 3.5)

set(CMAKE_CXX_STANDARD 14)
add_subdirectory(evpp)
add_subdirectory(cpp-bencoding)

message(STATUS, "This is source dir " ${PROJECT_SOURCE_DIR})

include_directories(${PROJECT_SOURCE_DIR}/evpp ${PROJECT_SOURCE_DIR}/cpp-bencoding/include)
link_directories(evpp/lib cpp-bencoding/src)

add_executable(test_server test/tcp_server.cc)
add_executable(test_client test/tcp_client.cc)
add_executable(test_bencode test/bencoding_test.cc)

target_link_libraries(test_server evpp)
target_link_libraries(test_client evpp)
target_link_libraries(test_bencode bencoding)