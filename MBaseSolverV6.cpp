// Created by Blasko Racu on 19/09/21.
//

#include <queue>
#include <iostream>
#include "MBaseSolverV6.h"
#include <vector>
#include <iterator>

void MBaseSolverV6::solve(InputMatrix& input) {
    auto emptySet=new bool[columnSize]();
    std::queue<bool*> queue;

    std::vector<bool*> mhss;
    queue.push(emptySet);
    clock_t startTime = clock();
    long counter=0;

    while(!queue.empty()){
        if(counter%1000000==0){
            clock_t endTime = clock();
            clock_t clockTicksTaken = endTime - startTime;
            double timeInSeconds = clockTicksTaken / (double) CLOCKS_PER_SEC;
            std::cout<<timeInSeconds<<std::endl;
            startTime=clock();
        }

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
                mhss.push_back(candidate);
            }
            else if(result==0&& i != columnSize - 1){
                queue.push(candidate);
            }
            else
                delete[] candidate;
        }
        delete[] representativeVector;
        delete[] current;
        queue.pop();
    }
    std::cout<<"______________________________________________" <<std::endl;
    std::cout<<"Numero di Iterazioni: "<<counter<<std::endl;
    std::cout<<"Risultati:" <<std::endl;
    for(auto vec:mhss){
        printVector(std::cout, vec);
    }
}

void MBaseSolverV6::printVector(std::ostream& stream, const bool* pBoolean) const{
    for(int i=0; i < columnSize; i++){
        stream<<"[";
        stream<<(pBoolean[i]?"1":"0");
        stream<<"]";
    }
    stream<<std::endl;
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

    //printRepVector(std::cout, other, inputMatrix.getRowLength());
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

int MBaseSolverV6::getMax(const bool* element, int size) const {
    int max=size;
    do{
        max--;
    }
    while(max>-1&&!element[max]);
    return max;
}

bool* MBaseSolverV6::getRepresentativeVector(const bool* pBoolean, const InputMatrix &inputMatrix) const{
    int size=inputMatrix.getRowLength();
    auto column=new bool[columnSize]();
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
    auto column=new bool[columnSize]();
    for(int j=0; j<size; j++){
        column[j]=column[j]||inputMatrix.getValueAt(j,index);

    }
    return column;
}

signed char MBaseSolverV6::evaluateTruthMap(unsigned char contains0, unsigned char contains1, unsigned char contains2, unsigned char contains3) const {
    return truthMap[(contains0<<3)+(contains1<<2)+(contains2<<1)+contains3];
}

bool MBaseSolverV6::containsMhs(const bool* candidate, const std::vector<bool*>& mhss) const{
    int max= getMax(candidate, columnSize);
    int min= getMin(candidate, columnSize);
    for(auto currentMhs:mhss){
        if(isSubset(currentMhs, candidate, max, min))
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

void MBaseSolverV6::printRepVector(std::ostream& stream, const bool *pBoolean, int length) const {
    for(int i=0; i < length; i++){
        stream<<"[";
        stream<<(pBoolean[i]?"1":"0");
        stream<<"]";
    }
    stream<<std::endl;
}

int MBaseSolverV6::getMin(const bool *pInt, int size) const {
    for(int i=0; i<size; i++){
        if(pInt[i])
            return i;
    }
    return 0;
}



