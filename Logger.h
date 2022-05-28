//
// Created by Blasko Racu on 27/05/22.
//
#include <iostream>
#include "Configuration.h"


#ifndef ALG_STR_DATI_LOGGER_H
#define ALG_STR_DATI_LOGGER_H


class Logger
{
public:
        static Logger& getInstance()
        {
            static Logger instance; // Guaranteed to be destroyed.
            // Instantiated on first use.
            return instance;
        }
    private:
    inline const std::string currentDateTime() {
        using namespace std::chrono;
        auto timepoint = system_clock::now();
        auto coarse = system_clock::to_time_t(timepoint);
        auto fine = time_point_cast<std::chrono::milliseconds>(timepoint);

        char buffer[sizeof "9999-12-31 23:59:59.999"];
        std::snprintf(buffer + std::strftime(buffer, sizeof buffer - 3,
                                             "%F %T.", std::localtime(&coarse)),
                      4, "%03lu", fine.time_since_epoch().count() % 1000);
        return buffer;
    }
    inline void newLog(const std::string &basicString, std::ofstream& ofstream, const char *string) {
        if(ofstream.is_open())
            ofstream.close();
        ofstream.open(Configuration::getInstance().getOutputFolderPath()+basicString+string);
    }

    inline void writeLog(const std::string &basicString){
        if(logFileStream.is_open())
            logFileStream.write(basicString.c_str(), basicString.length());
    }


    inline void writeOut(const std::string &basicString){
        if(outputFileStream.is_open())
            outputFileStream.write(basicString.c_str(), basicString.length());
    }

    Logger() {}                    // Constructor? (the {} brackets) are needed here.

        std::ofstream outputFileStream;
        std::ofstream logFileStream;

public:
    virtual ~Logger() {
        if(outputFileStream.is_open())
            this->logFileStream.close();
        if(outputFileStream.is_open())
            this->outputFileStream.close();
    }



// C++ 11
        // =======
        // We can use the better technique of deleting the methods
        // we don't want.
    void newInstance(const std::string& instanceName){
        newLog(instanceName, logFileStream, ".log");
        newLog(instanceName, outputFileStream, ".out");
    }
        Logger(Logger const&) = delete;
        void operator=(Logger const&)  = delete;

        // Note: Scott Meyers mentions in his Effective Modern
        //       C++ book, that deleted functions should generally
        //       be public as it results in better error messages
        //       due to the compilers behavior to check accessibility
        //       before deleted status
        //Copiato da stack, non ho scritto io questo commento ahah

    /**
     * Used to print informations if the channel is enabled
     * @param string
     */
    inline void info(const std::string& string){
        if(Configuration::getInstance().isInfo()){
            auto time=currentDateTime();
            auto log="INFO \t["+time+"] \t"+string;
            std::cout<<log<<std::endl;
            writeLog(log+"\n");
        }
    }

    /**
     * Used to print debug informations if the channel is enabled
     * @param string
     */
    inline void debug(const std::string& string){
        if(Configuration::getInstance().isDebug()){
            auto time=currentDateTime();
            auto log="DEBUG \t["+time+"] \t"+string;
            std::cout<<log<<std::endl;
            writeLog(log+"\n");
        }
    }

    /**
     * Used to print errors informations if the channel is enabled
     * @param string
     */
    inline void error(const std::string& string){
        if(Configuration::getInstance().isError()){
            auto time=currentDateTime();
            auto log="ERROR \t["+time+"] \t"+string;
            std::cout<<log<<std::endl;
            writeLog(log+"\n");
        }
    }

/**
     * Used to print output if the channel is enabled
     * @param string
     */
    inline void out(const std::string& string){
        if(std::strcmp("%time%", string.c_str())==0){
            out("Current time: [ "+currentDateTime()+ " ]\n");
            return;
        }
        writeOut(string);
        if(Configuration::getInstance().isOut()){
            if(strcmp(string.c_str(), "\n")==0){
                std::cout<<std::endl;
            }
            else{
                std::cout<<string;
            }
        }
    }
    /**
     * Used to print informations if the channel is enabled
     * @param string
     */
    inline static void logInfo(const std::string& string){
        getInstance().info(string);
    }

    /**
     * Used to print debug informations if the channel is enabled
     * @param string
     */
    inline static void logDebug(const std::string& string){
        getInstance().debug(string);
    }

    /**
     * Used to print errors informations if the channel is enabled
     * @param string
     */
    inline static void logError(const std::string& string){
        getInstance().error(string);
    }

    /**
     * Used to print output if the channel is enabled
     * @param string
     */
    inline static void logOut(const std::string& string){
        getInstance().out(string);
    }


};


#endif //ALG_STR_DATI_LOGGER_H
