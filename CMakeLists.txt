cmake_minimum_required(VERSION 3.22.1)
project(Graphics CXX)

# Find SFML
find_package(SFML 2 COMPONENTS audio graphics window system network)

add_executable(${PROJECT_NAME} main.cpp)
target_link_libraries(${PROJECT_NAME} PRIVATE sfml-graphics sfml-window sfml-system)