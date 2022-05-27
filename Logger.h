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
    const std::string currentDateTime() {
        time_t     now = time(0);
        struct tm  tstruct;
        char       buf[80];
        tstruct = *localtime(&now);
        // Visit http://en.cppreference.com/w/cpp/chrono/c/strftime
        // for more information about date/time format
        strftime(buf, sizeof(buf), "%Y-%m-%d.%X", &tstruct);

        return buf;
    }
    void newLog(const std::string &basicString, std::ofstream& ofstream, const char *string) {
        if(ofstream.is_open())
            ofstream.close();
        ofstream.open(Configuration::getInstance().getOutputFolderPath()+basicString+string);
    }

    void writeLog(const std::string &basicString){
            if(logFileStream.is_open())
                logFileStream.write(basicString.c_str(), basicString.length());
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
        newLog(instanceName, outputFileStream, ".log");
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
        if(Configuration::getInstance().isInfo()){
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
        if(Configuration::getInstance().isInfo()){
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
        if(Configuration::getInstance().isOut()){
            if(strcmp(string.c_str(), "\n")==0){
                std::cout<<std::endl;
            }
            else{
                std::cout<<string;

            }
            //writeOut
        }
    }
};


#endif //ALG_STR_DATI_LOGGER_H
