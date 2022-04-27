//
// Created by Utente on 27/04/2022.
//

#include "PreElaborator.h"

void PreElaborator::clean(InputMatrix &matrix) {
    cleanRows(matrix);
    cleanCols(matrix);

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
    for(int i=0; i<len; i++){
        std::cout<<boolP[i];
    }
    std::cout<<std::endl;
}