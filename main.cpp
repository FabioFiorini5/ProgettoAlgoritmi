#include <iostream>
#include "MBaseSolverV6.h"
#include "PreElaborator.h"
#include "Configuration.h"
#include "Logger.h"
#include "ResultPrinter.h"

#ifndef NDEBUG
#define LOG_DEBUG printf /* cose */
#else
#define LOG_DEBUG
#endif

int main(int argc, char *argv[]) {



    PreElaborator preElab;

    if(argc<=1){//base test
        InputMatrix inputMatrix;
        std::cout<<"Inputmatrix prima: "<<std::endl;
        inputMatrix.print(Logger::logDebug);
        preElab.clean(inputMatrix);
        std::cout<<"Inputmatrix dopo: "<<std::endl;
        inputMatrix.print(Logger::logDebug);
        MBaseSolverV6 solver(inputMatrix.getColumnLength());
        auto results=solver.solve(inputMatrix);
        ResultPrinter printer;
        printer.printResults(results, inputMatrix);
    }
    else{
        if(std::strcmp(argv[1], "path")==0){
            Configuration::getInstance().load(argv[2]);
            for (const auto & entry : std::filesystem::directory_iterator(Configuration::getInstance().getInputFolderPath())){
                Logger::getInstance().newInstance(entry.path().filename());
                Logger::logInfo(entry.path().filename());

                InputMatrix inputMatrix(entry.path());
                preElab.clean(inputMatrix);
                MBaseSolverV6 solver(inputMatrix.getColumnLength());
                auto results=solver.solve(inputMatrix);
                ResultPrinter printer;
                printer.printResults(results, inputMatrix);
            }
        }
        else{
            clock_t startTime = clock();
            InputMatrix inputMatrix(argv[1]);
            preElab.clean(inputMatrix);
            inputMatrix.print([](const std::string& x) -> void { Logger::getInstance().info(x);});
            MBaseSolverV6 solver(inputMatrix.getColumnLength());
            auto results=solver.solve(inputMatrix);
            ResultPrinter printer;
            printer.printResults(results, inputMatrix);
            clock_t endTime = clock();

            clock_t clockTicksTaken = endTime - startTime;
            double timeInSeconds = clockTicksTaken / (double) CLOCKS_PER_SEC;
            std::cout<<"Elapsed time: "<<timeInSeconds<<std::endl;
        }

    }


    return 0;
}

