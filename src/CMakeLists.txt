cmake_minimum_required(VERSION 3.0.0)
project(ex4 VERSION 0.1.0)

set(CMAKE_CXX_STANDARD 17)

add_compile_options(-Wall -Wextra -pedantic -Werror -Wnon-virtual-dtor -Wshadow)

set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR}/bin)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR}/bin)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR}/bin)


include_directories(${CMAKE_SOURCE_DIR} 
${CMAKE_SOURCE_DIR}/algorithms 
${CMAKE_SOURCE_DIR}/client_handlers
${CMAKE_SOURCE_DIR}/matrices 
${CMAKE_SOURCE_DIR}/servers
${CMAKE_SOURCE_DIR}/main)

link_directories(${CMAKE_SOURCE_DIR} 
${CMAKE_SOURCE_DIR}/algorithms 
${CMAKE_SOURCE_DIR}/client_handlers
${CMAKE_SOURCE_DIR}/matrices 
${CMAKE_SOURCE_DIR}/servers
${CMAKE_SOURCE_DIR}/main)


add_executable(ex4.out
algorithms/A-star_matrix_searcher.cpp
algorithms/BestFS_matrix_searcher.cpp
algorithms/BFS_matrix_searcher.cpp
algorithms/DFS_matrix_searcher.cpp
algorithms/problem.cpp
client_handlers/graph_client_handler.cpp
matrices/error_code.cpp
matrices/matrix_class.cpp
matrices/matrix.cpp
servers/parallel_server.cpp
servers/serial_server.cpp
main/main.cpp)

target_link_libraries(ex4.out pthread)

set(CPACK_PROJECT_NAME ${PROJECT_NAME})
set(CPACK_PROJECT_VERSION ${PROJECT_VERSION})
include(CPack)