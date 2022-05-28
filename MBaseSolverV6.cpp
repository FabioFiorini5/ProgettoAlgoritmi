// Created by Blasko Racu on 19/09/21.
//

#include <queue>
#include <iostream>
#include "MBaseSolverV6.h"
#include "Logger.h"
#include <vector>
#include <iterator>


std::vector<MBaseSolverV6::Mhs> MBaseSolverV6::solve(InputMatrix& input) {
    Logger::logInfo("Start solving");
    auto emptySet=new bool[columnSize]();
    std::queue<bool*> queue;

    std::vector<Mhs> mhss;
    queue.push(emptySet);
    u_int64_t counter=0;

    while(!queue.empty()){


        counter++;

        auto current=queue.front();

        auto representativeVector=getRepresentativeVector(current, input);

         for(int i=getSuccessor(getMax(current, columnSize), columnSize); i < columnSize; i++) {
            auto candidate= new bool[columnSize]();
            generateCandidate(current, i, candidate);

            if(containsMhs(candidate, mhss)){
                delete[] candidate;
                continue;
            }
            int result= check(representativeVector, i, input);
            if(result==1){
                mhss.emplace_back(candidate, getMin(candidate, columnSize), getMax(candidate, columnSize)+1);
            }
            else if(result == 0 && i != columnSize - 1){// && canContinue(candidate, input.getRowLength())){
                queue.push(candidate);
            }
            else
                delete[] candidate;
        }
        delete[] representativeVector;
        delete[] current;
        queue.pop();


    }
    Logger::logOut("Iterations: "+std::to_string(counter));
    Logger::logOut("\n");
    Logger::logInfo("End solver");

    return mhss;

}




void MBaseSolverV6::printVector(std::ostream &stream, const bool *pBoolean, InputMatrix matrix) const{
    stream<<"{";
    for(int i=0; i < columnSize; i++){
        if(pBoolean[i]){
            stream<<matrix.getLabels()[i].letter<<matrix.getLabels()[i].number;
            if(i!= getMax(pBoolean, columnSize)){
                stream<<", ";
            }
        }
       // stream<<"[";
       // stream<<(pBoolean[i]?"1":"0");
       // stream<<"]";
    }
    stream<<"}"<<std::endl;

    for(int i=0; i  <columnSize; i++){
        stream<<"[";
        stream<<(pBoolean[i]?"1":"0");
        stream<<"]";
    }
    std::cout<<std::endl;

}



void MBaseSolverV6::generateCandidate(const bool* father, int indexToAdd, bool* candidate) const{
    for(int i=0; i<columnSize; i++)
        candidate[i]=father[i];
    candidate[indexToAdd]=true;
}

/**
 *
 * @param pBoolean vettore rappresentativo
 * @param toAdd colonna i-esima
 * @param inputMatrix matrice
 * @return Check restituisce -1 se il vettore è da scartare, 0 se è da aggiungere in coda, 1 se è mhs
 */
int MBaseSolverV6::check(const bool* pBoolean, int toAdd, const InputMatrix& inputMatrix) const{
    int size=inputMatrix.getRowLength();
    auto other=getRepresentativeVector(toAdd, inputMatrix);

    printRepVector(other, inputMatrix.getRowLength());
    auto unionVector= new int[size]();

    for(int i=0; i<size; i++)
        unionVector[i]=pBoolean[i];
    bool contains0=false;
    bool contains1=false;
    bool contains2=false;
    bool contains3=false;

    for(int i=0; i<size; i++){
        unionVector[i]+=other[i]*2;
    }

    delete[] other;

    for(int i=0; i<size; i++)
        if(unionVector[i]==0){
            contains0=true;
            break;
        }
    for(int i=0; i<size; i++)
        if(unionVector[i]==1){
            contains1=true;
            break;
        }

    for(int i=0; i<size; i++)
        if(unionVector[i]==2){
            contains2=true;
            break;
        }

    for(int i=0; i<size; i++)
        if(unionVector[i]==3){
            contains3=true;
            break;
        }

    delete[] unionVector;

    return evaluateTruthMap(contains0, contains1, contains2, contains3);
}

int MBaseSolverV6::getSuccessor(int val, int other) const {
    return val+1< other?
        val+1:
        other;
}



bool* MBaseSolverV6::getRepresentativeVector(const bool* pBoolean, const InputMatrix &inputMatrix) const{
    int size=inputMatrix.getRowLength();
    auto column=new bool[size]();
    auto min = getMin(pBoolean, columnSize);
    auto max = getMax(pBoolean, columnSize);
    for(int i=min; i<=max; i++){
        if(!pBoolean[i])
            continue;

        for(int j=0; j<size; j++){
            column[j]=column[j]||inputMatrix.getValueAt(j,i);

        }
    }
    return column;
}

bool* MBaseSolverV6::getRepresentativeVector(int index, const InputMatrix &inputMatrix) const{
    int size=inputMatrix.getRowLength();
    auto column=new bool[size]();
    for(int j=0; j<size; j++){
        column[j]=column[j]||inputMatrix.getValueAt(j,index);

    }
    return column;
}

signed char MBaseSolverV6::evaluateTruthMap(unsigned char contains0, unsigned char contains1, unsigned char contains2, unsigned char contains3) const {
    return truthMap[(contains0<<3)+(contains1<<2)+(contains2<<1)+contains3];
}

bool MBaseSolverV6::containsMhs(const bool* vector, const std::vector<Mhs> &mhss) const{


    for(const auto& currentMhs:mhss){
        if(isSubset(currentMhs.mhs, vector, currentMhs.max, currentMhs.min))
            return true;
    }
    return false;
}

bool MBaseSolverV6::isSubset(const bool* mhs, const bool* candidate, int max, int min) const {
    for(int i=min; i<max; i++){
        if(mhs[i]&&!candidate[i])
            return false;
    }
    return true;
}

void MBaseSolverV6::printRepVector(const bool *pBoolean, int length) const {
    std::string line;
    for(int i=0; i < length; i++){
        line.append("[");
        line.append((pBoolean[i]?"1":"0"));
        line.append("]");
    }
    Logger::logDebug(line);
}



bool MBaseSolverV6::canContinue(const bool *pBoolean, int length) const{
    int count=0;
    for(int i=0; i<columnSize; i++){
        if(pBoolean[i])
            count++;
    }
    return count<length;
}



std::vector<InputMatrix::Label> MBaseSolverV6::getLabels(bool *pBoolean, InputMatrix& inputMatrix) {
    std::vector<InputMatrix::Label> labels;
    for(int i=0; i<columnSize; i++){
        if(pBoolean[i]){
            labels.push_back(inputMatrix.getLabels()[i]);
        }
    }
    return labels;
}





