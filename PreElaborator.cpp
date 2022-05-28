//
// Created by Utente on 27/04/2022.
//

#include "PreElaborator.h"
#include "Configuration.h"
#include "Logger.h"

void PreElaborator::clean(InputMatrix &matrix) {
    auto optimization=Configuration::getInstance().getOptimization();
    Logger::logDebug("Optimization level: "+std::to_string(optimization));
    if(optimization==0)
        return;
    Logger::logOut("%time%");
    Logger::logOut("Before pre-elaboration\n");
    matrix.print(Logger::logOut);
    Logger::logOut("\n");
    cleanRows(matrix);
    Logger::logOut("After same rows deletion\n");
    matrix.print(Logger::logOut);
    Logger::logOut("%time%");
    if(optimization==1)
        return;
    cleanCols(matrix);
    Logger::logOut("After empty columns deletion\n");
    matrix.print(Logger::logOut);
    Logger::logOut("%time%");
    if(optimization==2)
        return;
    cleanDuplicates(matrix);
    Logger::logOut("After duplicates union\n");
    matrix.print(Logger::logOut);
    Logger::logOut("%time%");

}

void PreElaborator::cleanRows(InputMatrix &matrix) {
    for(int i=0; i< matrix.getRowLength()-1; i++){ //controlla sia giusto il -1
        for(int j=i+1; j<matrix.getRowLength(); j++){ //metto il +1 per evitare il controllo con se stesso
            //se la riga i esima è sottoinsieme della colonna j esima
            if (isIncluded(matrix.getRow(i), matrix.getRow(j), matrix.getColumnLength()))
                matrix.removeRow(j);
        }
    }

}

void PreElaborator::cleanCols(InputMatrix &matrix) {
//se in una colonna ci sono solo zeri, allora tolgo la colonna
    for(int i=0; i<matrix.getColumnLength(); i++){
        auto actualColumn = matrix.getCol(i);
        bool toDelete=true;
        for(int j=0; j<matrix.getRowLength(); j++){
            if(actualColumn[j]) {
                toDelete = false;
                break;
            }
        }
        if(toDelete){
            matrix.removeColumn(i);
            i--;
        }

    }
}

bool PreElaborator::isIncluded(const bool* mat_i, const bool* mat_j, int columnLength) {
    for(int c=0; c<columnLength; c++){
        //Se uno è diverso return false, altrimenti return true
        if(mat_i[c] && !mat_j[c]) {
            return false;
        }
    }
    return true;
}

//4 casi
// matricei ha 0 e matricej ha 0 -> non ci interessa
//matricei ha 0 e matricej ha 1 -> non ci interessa
//matricei ha 1 e matricej ha 0 -> non è un sottoinsieme
//matricei ha 1 e matricej ha 1 -> questo lo è ma potrebbero mancargli altri 1, continua il ciclo


void PreElaborator::print(bool* boolP, int len) const{
    std::string line;
    for(int i=0; i<len; i++){
        line.append(std::to_string(boolP[i]));
    }
    Logger::logDebug(line);
}

void PreElaborator::cleanDuplicates(InputMatrix &matrix) {

    std::string line;
    for(int i=0; i<matrix.getColumnLength()-1; i++) {
        auto actualColumn = matrix.getCol(i);
        for(int j=i+1; j<matrix.getColumnLength(); j++){
            auto cloneCandidate=matrix.getCol(j);
            if(equals(actualColumn, cloneCandidate)){
                matrix.joinColumn(i,j);
                j--;
            }

        }
        line.append("Column ");//<<matrix.getLabels()[i].letter<<matrix.getLabels()[i].number;
        for(const auto& val: matrix.getLabels()[i].copied){
            line.append(std::to_string(val.letter)+std::to_string(val.number)+(" |"));
        }
        Logger::logDebug(line);
        line.clear();
    }

}

bool PreElaborator::equals(std::vector<bool> vector1, std::vector<bool> vector2) {
    for(int i=0; i<vector1.size(); i++){
        if(vector1[i]!=vector2[i])
            return false;
    }
    return true;
}
