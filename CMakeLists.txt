cmake_minimum_required(VERSION 3.5)
project(Lab2)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED True)

include_directories(${CMAKE_CURRENT_SOURCE_DIR}/include)
include_directories(${CMAKE_CURRENT_SOURCE_DIR}/utilite)
include_directories(${CMAKE_CURRENT_SOURCE_DIR}/Components)
set(SOURCES
    Source.cpp
    Components/Solution.cpp
    utilite/FileReader.cpp
    utilite/FileWriter.cpp
    Components/ComplexNumber.cpp
    EquationProcessor.cpp
    Components/StudentRating.cpp
    utilite/Sorting.cpp
    utilite/GenerateHTML.cpp
    
)

add_executable(${PROJECT_NAME} ${SOURCES})

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -g")