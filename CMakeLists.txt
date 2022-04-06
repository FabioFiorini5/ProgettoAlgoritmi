cmake_minimum_required(VERSION 3.17)
project(alqwaritzmi)

set(CMAKE_CXX_STANDARD 20)

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

add_compile_options(-Wall -Wextra -Wvla -v -O3)

add_executable(alqwaritzmi main.cpp
        MinimumHittingSetSolver.h
        InputMatrix.cpp
        InputMatrix.h
        MBaseSolver.cpp
        MBaseSolver.h
        MBaseSolverV2.cpp
        MBaseSolverV2.h
        MBaseSolverV3.cpp
        MBaseSolverV3.h
        MBaseSolverV4.cpp
        MBaseSolverV4.h
        MBaseSolverV5.cpp
        MBaseSolverV5.h
        MBaseSolverV6.cpp
        MBaseSolverV6.h

        )