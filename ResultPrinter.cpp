//
// Created by Blasko Racu on 28/05/22.
//

#include "ResultPrinter.h"
#include "Logger.h"

void ResultPrinter::printResults(std::vector<MBaseSolverV6::Mhs> &mhss, InputMatrix& input) {
    Logger::logInfo("Start extracting results");
    auto vettoreFinale=extractResults(mhss, input);
    Logger::logInfo("Ended extraction");




    for(auto& vec:vettoreFinale){
        Logger::logOut("{");
        for(unsigned long i=0; i<vec.size(); i++)
        {

            Logger::logOut(std::to_string(vec[i].index) + "(" + std::to_string(vec[i].letter) +std::to_string(vec[i].number)+")"+((i<vec.size()-1)?", ":""));
        }
        Logger::logOut("}");
        Logger::logOut("\n");
    }
    Logger::logInfo("Printed results");


}


int ResultPrinter::partition(std::vector<std::vector<InputMatrix::Label>> &vettoreFinale, int start, int end){

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

bool ResultPrinter::compareMhs(std::vector<InputMatrix::Label> &a, std::vector<InputMatrix::Label> &b) {
    if(a.size()<b.size()) return true;
    if(a.size()>b.size())  return false;
    for(unsigned long i=0; i<a.size(); i++){
        if(a[i].index<b[i].index) return true;
        if(a[i].index>b[i].index)  return false;
    }
    return false;
}

void ResultPrinter::quickSort(std::vector<std::vector<InputMatrix::Label>>& vettoreFinale, int start, int end){
    if (start >= end)
        return;

    int p = partition(vettoreFinale, start, end);
    quickSort(vettoreFinale, start, p - 1);

    quickSort(vettoreFinale, p + 1, end);
}

void ResultPrinter::extractMhs(bool* mhs, int pos_attuale, std::vector<std::vector<InputMatrix::Label>>& vettoreFinale, std::vector<InputMatrix::Label>& vettoreParziale, InputMatrix& inputMatrix) {
    if (pos_attuale > getMax(mhs, inputMatrix.getColumnLength())||pos_attuale==inputMatrix.getColumnLength()-1){
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

std::vector<std::vector<InputMatrix::Label>> ResultPrinter::extractResults(std::vector<MBaseSolverV6::Mhs> &mhss, InputMatrix &input) {
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
    return vettoreFinale;
}
