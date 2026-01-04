cmake_minimum_required(VERSION 3.10)
project(PongGame)

set(CMAKE_CXX_STANDARD 17)

# Find SFML
find_package(SFML 2.5 COMPONENTS graphics window system REQUIRED)

# Create executable
add_executable(PongGame main.cpp)

# Link SFML libraries
target_link_libraries(PongGame sfml-graphics sfml-window sfml-system)