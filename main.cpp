#include <iostream>
#include "MBaseSolverV6.h"
#include "PreElaborator.h"
#include "Configuration.h"
#include "Logger.h"
#include "ResultPrinter.h"
#include <future>
#include <vector>

#ifndef NDEBUG
#define LOG_DEBUG printf /* cose */
#else
#define LOG_DEBUG
#endif

std::vector<MBaseSolverV6::Mhs> solverino(InputMatrix& input){
    MBaseSolverV6 solver(input.getColumnLength());
    auto results = solver.solve(input);
    return results;
}

void ExcecutorInstances(){
    PreElaborator preElab;
    for (const auto & entry : std::filesystem::directory_iterator(Configuration::getInstance().getInputFolderPath())){
        std::cout<<"Currently evaluating "<< entry.path().filename() << std::endl;
        Logger::getInstance().newInstance(entry.path().filename());
        Logger::logInfo(entry.path().filename());
        InputMatrix inputMatrix(entry.path());
        preElab.clean(inputMatrix);
        //MBaseSolverV6 solver(inputMatrix.getColumnLength());
        auto results = std::async(&solverino, inputMatrix );


        ResultPrinter printer;
        printer.printResults(results, inputMatrix);
        Configuration::getInstance().setStopThreadSolver(false);

        if(Configuration::getInstance().getStopThreadInstances()){
            break;
        }
    }

}

int main(int argc, char *argv[]) {

    Configuration::getInstance().load(argv[2]);
    Configuration::getInstance().setStopThreadInstances(false);
    Configuration::getInstance().setStopThreadSolver(false);
    std::thread(ExecutorInstances);

    int c;

    std::cout<<"Premi 1 per terminare l'esecuzione, 2 per saltare l'istanza"<<std::endl;

    std::cin >> c;
    if (c == 1){
        Configuration::getInstance().setStopThreadInstances(true);
    }
    if(c == 2){
        Configuration::getInstance().setStopThreadSolver(true);
    }
    /*
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

*/
    return 0;
}
