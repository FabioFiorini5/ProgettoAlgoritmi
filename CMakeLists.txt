cmake_minimum_required(VERSION 3.17)
project(alg_str_dati)

set(CMAKE_CXX_STANDARD 20)

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

set (CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} -fno-omit-frame-pointer -fsanitize=address")
set (CMAKE_LINKER_FLAGS_DEBUG "${CMAKE_LINKER_FLAGS_DEBUG} -fno-omit-frame-pointer -fsanitize=address")

add_compile_options(-Wall -Wextra -Wvla -v -O3 -fsanitize=address -static-libsan -g)

add_executable(alg_str_dati main.cpp
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
        PreElaborator.cpp
        PreElaborator.h
        Logger.h Configuration.h)