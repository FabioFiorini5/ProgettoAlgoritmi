//
// Created by Blasko Racu on 27/05/22.
//
#include <iostream>


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
    Logger() {}                    // Constructor? (the {} brackets) are needed here.

    // C++ 11
    // =======
    // We can use the better technique of deleting the methods
    // we don't want.
public:
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
    void info(const std::string& string, const std::string& parameters...){

    }

    /**
     * Used to print debug informations if the channel is enabled
     * @param string
     */
    void debug(const std::string& string, const std::string& parameters...){

    }

    /**
     * Used to print errors informations if the channel is enabled
     * @param string
     */
    void error(const std::string& string, const std::string& parameters...){

    }

    /**
     * Used to print output if the channel is enabled
     * @param string
     */
    void out(const std::string& string, const std::string& parameters...){

    }
};


#endif //ALG_STR_DATI_LOGGER_H
