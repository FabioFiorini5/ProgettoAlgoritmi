// Created by Blasko Racu on 19/09/21.
//

#include <queue>
#include <iostream>
#include "MBaseSolverV6.h"
#include <vector>
#include <iterator>

bool compareMhs(std::vector<InputMatrix::Label> &vector, std::vector<InputMatrix::Label> &vector1);

void MBaseSolverV6::solve(InputMatrix& input) {
    auto emptySet=new bool[columnSize]();
    std::queue<bool*> queue;

    std::vector<Mhs> mhss;
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
    std::cout<<"______________________________________________" <<std::endl;
    std::cout<<"Numero di Iterazioni: "<<counter<<std::endl;
    std::cout<<"Risultati:" <<std::endl;


    std::vector<std::vector<InputMatrix::Label>> vettoreFinale;
    std::vector<InputMatrix::Label> vettoreParziale;
    for(auto& vec:mhss){
        //printVector(std::cout, vec.mhs, input);
        extractMhs(vec.mhs, 0, vettoreFinale, vettoreParziale, input);
        vettoreParziale.clear();
    }
    mhss.clear();

    std::vector<InputMatrix::Label> toOrder(input.getColumnLengthOriginal());
    for(auto& vec:vettoreFinale) {
        for(const auto& lbl: vec){
            toOrder[lbl.index-1]=lbl;
        }
        vec.clear();
        for(auto& lbl: toOrder){
            if(lbl.index!=0){
                vec.push_back(lbl);
                lbl.index=0;
            }
        }
    }

    quickSort(vettoreFinale, 0, vettoreFinale.size()-1);
    /*std::sort(vettoreFinale.begin(), vettoreFinale.end(),[](std::vector<InputMatrix::Label>& a, std::vector<InputMatrix::Label>& b){
        if(a.size()<b.size()) return true;
        if(a.size()>b.size())  return false;
        for(int i=0; i<a.size(); i++){
            if(a[i].index<b[i].index) return true;
            if(a[i].index>b[i].index)  return false;
        }
        return false;
    });*/

    for(auto& vec:vettoreFinale){
        std::cout << "{";
        for(unsigned long i=0; i<vec.size(); i++)
        {

            std::cout<<vec[i].index<<"("<<vec[i].letter<<vec[i].number<<")"<<((i<vec.size()-1)?", ":"");
        }
        std::cout << "}" << std::endl;
    }
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

bool MBaseSolverV6::canContinue(const bool *pBoolean, int length) const{
    int count=0;
    for(int i=0; i<columnSize; i++){
        if(pBoolean[i])
            count++;
    }
    return count<length;
}

void MBaseSolverV6::extractMhs(bool* mhs, int pos_attuale, std::vector<std::vector<InputMatrix::Label>>& vettoreFinale, std::vector<InputMatrix::Label>& vettoreParziale, InputMatrix& inputMatrix) {
    if (pos_attuale > getMax(mhs, columnSize)||pos_attuale==columnSize-1){
        if (mhs[pos_attuale] == 0) {
            vettoreFinale.push_back(vettoreParziale);
        }
        if (mhs[pos_attuale] == 1){
            for (unsigned long j = 0; j < inputMatrix.getLabels()[pos_attuale].copied.size(); j++) {
                vettoreParziale.push_back(inputMatrix.getLabels()[pos_attuale].copied[j]);
                vettoreFinale.push_back(vettoreParziale);
                vettoreParziale.pop_back();
            }
        }
        return;
    }
    if (mhs[pos_attuale] == 0){
        extractMhs(mhs, pos_attuale + 1, vettoreFinale, vettoreParziale, inputMatrix);
    }

    if (mhs[pos_attuale] == 1){
        for (unsigned long j = 0; j < inputMatrix.getLabels()[pos_attuale].copied.size(); j++) {
            vettoreParziale.push_back(inputMatrix.getLabels()[pos_attuale].copied[j]);
            extractMhs(mhs, pos_attuale + 1, vettoreFinale, vettoreParziale, inputMatrix);
            vettoreParziale.pop_back();
        }
    }

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


int MBaseSolverV6::partition(std::vector<std::vector<InputMatrix::Label>> &vettoreFinale, int start, int end){

    auto pivot = vettoreFinale[start];

    int count = 0;
    for (int i = start + 1; i <= end; i++) {
        if (!compareMhs(pivot, vettoreFinale[i]))
            count++;
    }

    // Giving pivot element its correct position
    int pivotIndex = start + count;
    std::swap(vettoreFinale[pivotIndex], vettoreFinale[start]);

    // Sorting left and right parts of the pivot element
    int i = start, j = end;

    while (i < pivotIndex && j > pivotIndex) {
        //se è uguale devo guardare il secondo elemento
        while (compareMhs(vettoreFinale[i],vettoreFinale[pivotIndex])) {
            i++;
        }

        while (compareMhs(vettoreFinale[pivotIndex], vettoreFinale[j])) {
            j--;
        }

        if (i < pivotIndex && j > pivotIndex) {
            std::swap(vettoreFinale[i++], vettoreFinale[j--]);
        }
    }

    return pivotIndex;
}

bool MBaseSolverV6::compareMhs(std::vector<InputMatrix::Label> &a, std::vector<InputMatrix::Label> &b) {
    if(a.size()<b.size()) return true;
    if(a.size()>b.size())  return false;
    for(unsigned long i=0; i<a.size(); i++){
        if(a[i].index<b[i].index) return true;
        if(a[i].index>b[i].index)  return false;
    }
    return false;
}

void MBaseSolverV6::quickSort(std::vector<std::vector<InputMatrix::Label>>& vettoreFinale, int start, int end){
    if (start >= end)
        return;

    int p = partition(vettoreFinale, start, end);
    quickSort(vettoreFinale, start, p - 1);

    quickSort(vettoreFinale, p + 1, end);
}



