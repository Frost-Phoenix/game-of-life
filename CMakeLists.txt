cmake_minimum_required(VERSION 3.10)
project(game-of-life)

# Set the C++ standard
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED True)

set(CMAKE_BUILD_TYPE Release)

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

# Define source files
file(GLOB_RECURSE SRC_FILES src/*.cpp)

# Set include directories
include_directories(include)

# Output directory for the binary
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)

# Create the executable
add_executable(game-of-life ${SRC_FILES})

# Debug and Release flags
set(CMAKE_CXX_FLAGS_DEBUG "-g -fsanitize=address,undefined")
set(CMAKE_CXX_FLAGS_RELEASE "-O2")

# Enable debug and release configurations
if(CMAKE_BUILD_TYPE MATCHES Debug)
    target_compile_options(game-of-life PRIVATE ${CMAKE_CXX_FLAGS_DEBUG})
elseif(CMAKE_BUILD_TYPE MATCHES Release)
    target_compile_options(game-of-life PRIVATE ${CMAKE_CXX_FLAGS_RELEASE})
endif()
