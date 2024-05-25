cmake_minimum_required(VERSION 3.5)
project(Lab2)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED True)

set(SOURCES
    Source.cpp
    Solution.cpp
    FileReader.cpp
    FileWriter.cpp
    ComplexNumber.cpp
    EquationProcessor.cpp
    StudentRating.cpp
    Sorting.cpp
    GenerateHTML.cpp
    
)

add_executable(${PROJECT_NAME} ${SOURCES})

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -g")