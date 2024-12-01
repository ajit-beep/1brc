cmake_minimum_required(VERSION 3.10)

# set the project name
project (TemperatureAnalyzer)

# set the c++ standards
set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED True)


# Add the executable
add_executable(TemperatureAnalyzer src/main.cpp)

# Optimize for speed
if (CMAKE_BUILD_TYPE STREQUAL "Release")
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -O3")
endif()