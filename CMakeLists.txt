cmake_minimum_required(VERSION 3.24)
project(VLIB)
set(CMAKE_CXX_STANDARD 17)

# Add source files to the project
set(SOURCE_FILES src/main.cpp)

# Create the executable
add_executable(${PROJECT_NAME} ${SOURCE_FILES})
