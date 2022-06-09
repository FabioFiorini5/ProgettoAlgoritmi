#include <iostream>
#include "MBaseSolverV6.h"
#include "PreElaborator.h"
#include "Configuration.h"
#include "Logger.h"
#include "ResultPrinter.h"
#include <vector>
#include <chrono>
#include <thread>
#include "Memory.h"

using namespace std::chrono_literals;


void timeout(){
    int timeout=Configuration::getInstance().getTimeout();
    int i=0;
    while(!Configuration::getInstance().getStopThreadSolver()&&i<timeout*10){
        std::this_thread::sleep_for(std::chrono::milliseconds(100));
        i++;
    }
    if(i>=timeout*10)
    {
        Logger::logInfo("Timeout reached");
        Logger::logOut("Timeout reached");
        Configuration::getInstance().setStopThreadSolver(true);
    }

}



void run(){
    Logger::getInstance().initCsvLog();
    PreElaborator preElab;
    for (const auto & entry : std::filesystem::directory_iterator(Configuration::getInstance().getInputFolderPath())){
        if(std::strcmp(".matrix", entry.path().extension().string().c_str())!=0)
            continue;
        Logger::getInstance().newInstance(entry.path().filename());
        Logger::logInfo(entry.path().filename());
        Logger::logOutCsv(entry.path().filename());
        Logger::logOutCsv(Memory::getMemoryUsage());

        std::thread timeoutThread(&timeout);
        InputMatrix inputMatrix(entry.path());
        inputMatrix.print();
        Logger::logOutCsv("%time%");
        preElab.clean(inputMatrix);
        Logger::logOutCsv("%time%");

        inputMatrix.print();

        MBaseSolverV6 solver(inputMatrix.getColumnLength());
        Logger::logOutCsv("%time%");
        auto results = solver.solve(inputMatrix);
        Logger::logOutCsv("%time%");

        ResultPrinter printer;
        printer.printResults(results, inputMatrix);

        Logger::logOutCsv(Memory::getMemoryUsage());
        Logger::logOutCsv(std::to_string(Configuration::getInstance().getStopThreadSolver()));
        Logger::newRowCsv();

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
            Configuration::getInstance().setRunning(false);
        }
        if(c == 2){
            Configuration::getInstance().setStopThreadSolver(true);
        }
    }
    executor.join();

    return 0;
}
