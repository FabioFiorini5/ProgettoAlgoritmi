//
// Created by Blasko Racu on 28/05/22.
//

#include "ResultPrinter.h"
#include "Logger.h"

void ResultPrinter::printResults(std::vector<MBaseSolverV6::_mhs> mhss, InputMatrix& input) {
    if(Configuration::getInstance().isExtractionEnabled()){
        doExtraction(mhss, input);
    }
    else{
        doCartesianMul(mhss, input);
    }
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

    if(Configuration::getInstance().isDebug()){
        calculateExpectedResults(mhss, input);
    }
    std::vector<std::vector<InputMatrix::Label>> vettoreFinale;
    std::vector<InputMatrix::Label> vettoreParziale;
    for(auto& vec:mhss){
        //printVector(std::cout, vec.mhs, input);
        extractMhs(vec.mhs, 0, vettoreFinale, vettoreParziale, input);
        vettoreParziale.clear();
    }
    mhss.clear();
    clearCopied(vettoreFinale);
    //Logger::logDebug("Ended extraction, start sorting");
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

    //Logger::logDebug("Ended sorting");
    return vettoreFinale;
}

void ResultPrinter::calculateExpectedResults(std::vector<MBaseSolverV6::Mhs> &mhss, InputMatrix &inputMatrix) {
    u_int64_t tot=0;
    u_int64_t mhsTot;
    for(const auto& mhs: mhss){
        mhsTot=0;
        for(int i=0; i<inputMatrix.getColumnLength(); i++){
            if (mhs.mhs[i] == 1){
                mhsTot=(mhsTot==0)?inputMatrix.getLabels()[i].copied.size():mhsTot*inputMatrix.getLabels()[i].copied.size();

            }
        }
        tot+=mhsTot;
    }
    //Logger::logDebug("Numero di MHS: "+std::to_string(tot));
}

void ResultPrinter::clearCopied(std::vector<std::vector<InputMatrix::Label>>& mhss) {
    for(auto& mhs:mhss){
        for(auto& el: mhs){
            el.copied.clear();
        }
    }
}

std::vector<std::vector<InputMatrix::Label>>
ResultPrinter::getCartesianMul(std::vector<MBaseSolverV6::Mhs> &mhss, InputMatrix &input) {
    std::vector<std::vector<InputMatrix::Label>> toReturn;
    for(auto& mhs:mhss){
        auto labels=convertMhs(mhs, input);
        toReturn.push_back(labels);
    }
    clearCopied(toReturn);
    mhss.clear();

    return toReturn;
}

std::vector<InputMatrix::Label>
ResultPrinter::convertMhs(MBaseSolverV6::_mhs &mhs, InputMatrix &matrix) {

    std::vector<InputMatrix::Label> labels;
    for(int i=mhs.min; i<std::min(mhs.max+1, matrix.getColumnLength()); i++){
        if(mhs.mhs[i]){
            labels.emplace_back(matrix.getLabels()[i]);
        }
    }
    return labels;
}

void ResultPrinter::doExtraction(std::vector<MBaseSolverV6::Mhs> &mhss, InputMatrix &input) {
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

void ResultPrinter::doCartesianMul(std::vector<MBaseSolverV6::Mhs> &mhss, InputMatrix &input) {
    Logger::logInfo("Start collecting results");
    auto finalVec=getCartesianMul(mhss, input);
    Logger::logInfo("Collected results, start printing");
    for(const auto& vec:finalVec){
        print(vec, input);
    }
    finalVec.clear();
}

void ResultPrinter::print(const std::vector<InputMatrix::Label> &vector, InputMatrix &matrix) {
    std::string mhs;
    mhs.append("{");
    for(int j=0; j<vector.size(); j++){
        const auto& el=vector[j];
        mhs.append("{");
        for(int i=0; i<el.copied.size(); i++){
            mhs.append(el.copied[i].toString());
            if(i<el.copied.size()-1)
                mhs.append(", ");
        }
        mhs.append("}");
        if(j<vector.size()-1)
            mhs.append(" X ");
    }
    mhs.append("}\n");
    Logger::logOut(mhs);
}
