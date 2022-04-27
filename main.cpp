#include <iostream>
#include "MBaseSolver.h"
#include "MBaseSolverV2.h"
#include "MBaseSolverV3.h"
#include "MBaseSolverV5.h"
#include "MBaseSolverV6.h"
#include "PreElaborator.h"

#ifndef NDEBUG
#define LOG_DEBUG printf /* cose */
#else
#define LOG_DEBUG
#endif

int main(int argc, char *argv[]) {

    LOG_DEBUG("start\n");

    MBaseSolverV6 solver;
    PreElaborator preElab;

    if(argc<=1){//base test
        InputMatrix inputMatrix;
        std::cout<<"Inputmatrix prima: "<<std::endl;
        inputMatrix.print();
        preElab.clean(inputMatrix);
        std::cout<<"Inputmatrix dopo: "<<std::endl;
        inputMatrix.print();
        solver.solve(inputMatrix);
    }
    else{
        InputMatrix inputMatrix(argv[1]);
        preElab.clean(inputMatrix);
        solver.solve(inputMatrix);
    }

    LOG_DEBUG("end\n");
    return 0;
}