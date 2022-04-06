//
// Created by Blasko Racu on 19/09/21.
//

#include <queue>
#include <iostream>
#include "MBaseSolverV2.h"

void MBaseSolverV2::solve(InputMatrix& input) {
    int size=input.getColumnLength();
    auto emptySet=new bool[size]();
    std::queue<bool*> queue;
    queue.push(emptySet);
    while(queue.front()!= nullptr){
        bool* current=queue.front();
        std::cerr<<"Current: "<<std::endl;
        print(std::cerr, current, size);
        bool* representativeVector=getRepresentativeVector(current, input);
        for(int i=getSuccessor(getMax(current, size), size); i<size; i++) {
            auto candidate = generateCandidate(size, current, i);

            std::cerr<<"Candidate: "<<std::endl;
            print(std::cerr, candidate, size);

            int result= check(representativeVector, i, input);
            if(result==1&&i!=size-1)
                print(std::cout, candidate, size);
            if(result==0)
                queue.push(candidate);
        }
        delete[] representativeVector;
        delete[] current;
        queue.pop();
    }
}

void MBaseSolverV2::print(std::ostream& stream, const bool *pBoolean, int size) const{
    for(int i=0; i<size; i++){
        stream<<"[";
        stream<<(pBoolean[i]?"1":"0");
        stream<<"]";
    }
    stream<<std::endl;
}

bool* MBaseSolverV2::generateCandidate(int size, const bool* father, int indexToAdd){
    auto bitset= new bool[size];
    for(int i=0; i<size; i++){
        bitset[i]=father[i]||i==indexToAdd;
    }
    return bitset;
}

int MBaseSolverV2::check(const bool *pBoolean, int toAdd, InputMatrix& inputMatrix) {
    int size=inputMatrix.getRowLength();
    bool* other=getRepresentativeVector(toAdd, inputMatrix);
    char unionVector[size];
    for(int i=0; i<size; i++)
        unionVector[i]=pBoolean[i];


    bool contains0=false;
    bool contains1=false;
    bool contains2=false;
    bool contains3=false;
    for(int i=0; i<size; i++){
        if(other[i])
            unionVector[i]+=2;
        if(unionVector[i]==0)
            contains0=true;
        if(unionVector[i]==1)
            contains1=true;
        if(unionVector[i]==2)
            contains2=true;
        if(unionVector[i]==3)
            contains3=true;
    }
    delete[] other;
    return evaluateTruthMap(contains0, contains1, contains2, contains3);
}

int MBaseSolverV2::getSuccessor(int val, int other) const {
    return val+1< other?
        val+1:
        other;
}

int MBaseSolverV2::getMax(const bool* element, int size) const {
    int max=size;
    do{
        max--;
    }
    while(max>-1&&!element[max]);
    return max;
}

bool* MBaseSolverV2::getRepresentativeVector(const bool *pBoolean, InputMatrix &inputMatrix) {
    int size=inputMatrix.getRowLength();
    auto column=new bool[size]();
    for(int i=0; i<size; i++)
        column[i]=false;
    for(int i=0; i<inputMatrix.getColumnLength(); i++){
        if(!pBoolean[i])
            continue;
        for(int j=0; j<size; j++){
            column[j]=column[j]||inputMatrix.getValueAt(j,i);
        }
    }
    return column;
}

bool* MBaseSolverV2::getRepresentativeVector(int index, InputMatrix &inputMatrix) {
    auto bitset=new bool[inputMatrix.getColumnLength()]();
    bitset[index]=true;
    auto toReturn=getRepresentativeVector(bitset, inputMatrix);
    delete[] bitset;
    return toReturn;
}

int MBaseSolverV2::evaluateTruthMap(bool contains0, bool contains1, bool contains2, bool contains3) const {
    if(contains0&&contains1&&contains2)
        return 0;
    if(contains0&&contains1)
        return -1;
    if(contains0&&contains2&&contains3)
        return -1;
    if(contains0&&contains2)
        return 0;
    if(contains0)
        return -1;
    if(contains1&&contains2)
        return 1;
    if(contains1)
        return -1;
    if(contains2&&contains3)
        return -1;
    if(contains2)
        return 1;
    return -1;
}


