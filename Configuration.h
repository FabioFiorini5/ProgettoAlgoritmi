//
// Created by Blasko Racu on 27/05/22.
//
#include <iostream>
#include <fstream>
#include <unordered_map>

#ifndef ALG_ConfigurationTR_DATI_CONFIGURATION_H
#define ALG_ConfigurationTR_DATI_CONFIGURATION_H


class Configuration
{
public:
    static Configuration& getInstance()
    {
        static Configuration    instance; // Guaranteed to be destroyed.
        // Instantiated on first use.
        return instance;
    }
private:
    inline void loadLogLevels() {
        loadLogLevel(info, "info");
        loadLogLevel(out, "out");
        loadLogLevel(debug, "debug");
        loadLogLevel(error, "error");
    }

    inline void loadLogLevel(bool& pointer, const std::string& string){
        auto it=properties.find(string);
        if(it!=properties.end()){
            pointer= std::strcmp("true", it->second.c_str()) == 0;
        }
    }

    inline void loadPath(std::string& pointer, const std::string& string){
        auto it=properties.find(string);
        if(it!=properties.end()){
            pointer= it->second;
        }
    }

    inline void loadPaths() {
        loadPath(inputFolderPath, "inputFolderPath");
        loadPath(outputFolderPath, "outputFolderPath");
    }

    Configuration() {
        info=true;
        debug=true;
        error=true;
        out=true;
    }                    // Constructor? (the {} brackets) are needed here.

    bool info, debug, error, out;

    std::string inputFolderPath;
    std::string outputFolderPath;


    std::unordered_map<std::string, std::string> properties;


    // C++ 11
    // =======
    // We can use the better technique of deleting the methods
    // we don't want.
public:
    Configuration(Configuration const&)               = delete;
    void operator=(Configuration const&)  = delete;
    // Note: Configurationcott Meyers mentions in his Effective Modern
    //       C++ book, that deleted functions should generally
    //       be public as it results in better error messages
    //       due to the compilers behavior to check accessibility
    //       before deleted status



    void load(const std::string& configurationPath){
        std::ifstream infile(configurationPath);
        std::string line;
        std::cout << configurationPath << std::endl;
        while (std::getline(infile, line)) {
            auto pos = line.find('=');

            auto property = line.substr(0, pos);
            auto propertyValue = line.substr(pos + 1, line.length() - pos - 1);
            properties.emplace(std::make_pair(property, propertyValue));
        }
        loadLogLevels();
        loadPaths();
    }

    [[nodiscard]] bool isInfo() const {
        return info;
    }

    [[nodiscard]] bool isDebug() const {
        return debug;
    }

    [[nodiscard]] bool isError() const {
        return error;
    }

    [[nodiscard]] bool isOut() const {
        return out;
    }

    [[nodiscard]] const std::string &getInputFolderPath() const {
        return inputFolderPath;
    }

    [[nodiscard]] const std::string &getOutputFolderPath() const {
        return outputFolderPath;
    }
};

#endif //ALG_ConfigurationTR_DATI_CONFIGURATION_H
