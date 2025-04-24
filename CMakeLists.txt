cmake_minimum_required(VERSION 3.10)
project(Cook VERSION 0.1.0 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 14)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Add include directories
include_directories(include)

# Source files
set(SOURCES
    src/main.cpp
    src/lexer.cpp
    src/parser.cpp
    src/ast.cpp
    src/interpreter.cpp
)

# Create executable
add_executable(cook ${SOURCES})

# Install
install(TARGETS cook DESTINATION bin)
