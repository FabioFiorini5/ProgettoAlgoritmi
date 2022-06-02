#include <iostream>
#include "MBaseSolverV6.h"
#include "PreElaborator.h"
#include "Configuration.h"
#include "Logger.h"
#include "ResultPrinter.h"
#include <vector>
#include <chrono>
#include <thread>

using namespace std::chrono_literals;

#ifndef NDEBUG
#define LOG_DEBUG printf /* cose */
#else
#define LOG_DEBUG
#endif


void timeout(){
    int timeout=Configuration::getInstance().getTimeout();
    int i=0;
    while(!Configuration::getInstance().getStopThreadSolver()&&i<timeout){
        std::this_thread::sleep_for(std::chrono::milliseconds(1000));
        i++;
    }
    if(i>=timeout)
    {
        Logger::logInfo("Timeout reached");
        Logger::logOut("Timeout reached");
        Configuration::getInstance().setStopThreadSolver(true);
    }

}

void run(){
    PreElaborator preElab;
    for (const auto & entry : std::filesystem::directory_iterator(Configuration::getInstance().getInputFolderPath())){
        if(std::strcmp(".matrix", entry.path().extension().string().c_str())!=0)
            continue;
        Logger::getInstance().newInstance(entry.path().filename());
        Logger::logInfo(entry.path().filename());
        std::thread timeoutThread(&timeout);
        InputMatrix inputMatrix(entry.path());
        preElab.clean(inputMatrix);
        MBaseSolverV6 solver(inputMatrix.getColumnLength());
        auto results = solver.solve(inputMatrix);

        ResultPrinter printer;
        printer.printResults(results, inputMatrix);
        Configuration::getInstance().setStopThreadSolver(true);
        timeoutThread.join();
        Configuration::getInstance().setStopThreadSolver(false);

        if(Configuration::getInstance().getStopThreadInstances()){
            break;
        }
    }
    Configuration::getInstance().setRunning(false);

}

int main(int argc, char *argv[]) {

    if(argc>1)
        Configuration::getInstance().load(argv[1]);
    Configuration::getInstance().setStopThreadInstances(false);
    Configuration::getInstance().setStopThreadSolver(false);

    Configuration::getInstance().setRunning(true);
    std::thread executor(&run);

    int c=0;
    while(Configuration::getInstance().isRunning()){
        std::cout<<"Premi 1 per terminare l'esecuzione, 2 per saltare l'istanza"<<std::endl;

        std::cin >> c;
        if (c == 1){
            Configuration::getInstance().setStopThreadInstances(true);
        }
        if(c == 2){
            Configuration::getInstance().setStopThreadSolver(true);
        }
    }
    executor.join();
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
