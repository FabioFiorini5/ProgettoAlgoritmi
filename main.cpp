#include <iostream>
#include "MBaseSolver.h"
#include "MBaseSolverV2.h"
#include "MBaseSolverV3.h"
#include "MBaseSolverV4.h"
#include "MBaseSolverV5.h"
#include "MBaseSolverV6.h"

#ifndef NDEBUG
#define LOG_DEBUG printf /* cose */
#else
#define LOG_DEBUG
#endif

int main() {
    LOG_DEBUG("start\n");

    MBaseSolverV6 solver;
    InputMatrix inputMatrix("/Users/blasko/CLionProjects/alqwaritzmi/benchmarks/c7552.325.matrix");
    solver.solve(inputMatrix);

    LOG_DEBUG("end\n");
    return 0;
}