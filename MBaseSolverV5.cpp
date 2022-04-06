//
// Created by Blasko Racu on 19/09/21.
//

#include <queue>
#include <iostream>
#include <algorithm>
#include "MBaseSolverV5.h"

void MBaseSolverV5::solve(InputMatrix& input) {
    int size=input.getColumnLength();
    auto emptySet=std::vector<bool>(size);
    std::queue<std::vector<bool>> queue;

    std::vector<std::vector<bool>> mhss;
    queue.push(emptySet);
    while(!queue.empty()){
        std::vector<bool> current=queue.front();
        std::vector<bool> representativeVector=getRepresentativeVector(current, input);
        for(int i=getSuccessor(getMax(current, size), size); i<size; i++) {
            auto candidate = generateCandidate(size, current, i);
            if(containsMhs(candidate, mhss)){
                continue;
            }
            int result= check(representativeVector, i, input);
            if(result==1&&i!=size-1){
                mhss.push_back(std::move(candidate));
            }
            if(result==0){
                queue.push(std::move(candidate));
            }
        }
        queue.pop();
    }
    for(const std::vector<bool>& vec:mhss){
        print(std::cout, vec);
    }
}

void MBaseSolverV5::print(std::ostream& stream, const std::vector<bool>& pBoolean) const{
    for(const bool b:pBoolean){
        stream<<"[";
        stream<<(b?"1":"0");
        stream<<"]";
    }
    stream<<std::endl;
}

std::vector<bool> MBaseSolverV5::generateCandidate(int size, const std::vector<bool>& father, int indexToAdd) const{
    std::vector<bool> bitset(size);
    for(int i=0; i<size; i++){
        bitset[i]=father[i]||i==indexToAdd;
    }
    return bitset;
}

int MBaseSolverV5::check(const std::vector<bool>& pBoolean, int toAdd, InputMatrix& inputMatrix) const{
    int size=inputMatrix.getRowLength();
    std::vector<bool> other=getRepresentativeVector(toAdd, inputMatrix);
    std::vector<char> unionVector(size);
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
    return evaluateTruthMap(contains0, contains1, contains2, contains3);
}

int MBaseSolverV5::getSuccessor(int val, int other) const {
    return val+1< other?
        val+1:
        other;
}

int MBaseSolverV5::getMax(const std::vector<bool>& element, int size) const {
    int max=size;
    do{
        max--;
    }
    while(max>-1&&!element[max]);
    return max;
}

std::vector<bool> MBaseSolverV5::getRepresentativeVector(const std::vector<bool>& pBoolean, InputMatrix &inputMatrix) const{
    int size=inputMatrix.getRowLength();
    std::vector<bool> column(size);
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

std::vector<bool> MBaseSolverV5::getRepresentativeVector(int index, InputMatrix &inputMatrix) const{
    std::vector<bool> bitset(inputMatrix.getColumnLength());
    bitset[index]=true;
    auto toReturn=getRepresentativeVector(bitset, inputMatrix);
    return toReturn;
}

signed char MBaseSolverV5::evaluateTruthMap(unsigned char contains0, unsigned char contains1, unsigned char contains2, unsigned char contains3) const {
    return truthMap[(contains0<<3)+(contains1<<2)+(contains2<<1)+contains3];
}

bool MBaseSolverV5::containsMhs(const std::vector<bool>& candidate, const std::vector<std::vector<bool>>& mhss) const{
    //std::ranges::any_of
    for(const std::vector<bool>& currentMhs:mhss){
        if(isSubset(currentMhs, candidate))
            return true;
    }
    return false;
}

bool MBaseSolverV5::isSubset(const std::vector<bool> &mhs, const std::vector<bool> &candidate) const {
    for(int i=0; i<mhs.size(); i++){
        if(mhs[i]&&!candidate[i])
            return false;
    }
    return true;
}


