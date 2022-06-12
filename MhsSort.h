//
// Created by Blasko Racu on 11/06/22.
//

#include <vector>
#include "MBaseSolverV6.h"
#include "Configuration.h"

#ifndef ALG_STR_DATI_MHSSORT_H
#define ALG_STR_DATI_MHSSORT_H


#endif //ALG_STR_DATI_MHSSORT_H


/**
 * partial counting sort to order internal vectors
 * @param mhss
 * @param input
 */
static inline void internalSort(std::vector<std::vector<InputMatrix::Label>>& mhss, const InputMatrix& input){
    std::vector<InputMatrix::Label> toOrder(input.getColumnLengthOriginal());
    for(auto& vec:mhss) {
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
}


static bool compareMhs(std::vector<InputMatrix::Label> &a, std::vector<InputMatrix::Label> &b) {
    if(a.size()<b.size()) return true;
    if(a.size()>b.size())  return false;
    for(unsigned long i=0; i<a.size(); i++){
        if(a[i].index<b[i].index) return true;
        if(a[i].index>b[i].index)  return false;
    }
    return false;
}


static int partition(std::vector<std::vector<InputMatrix::Label>> &vettoreFinale, int start, int end){

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


static void quickSort(std::vector<std::vector<InputMatrix::Label>>& vettoreFinale, int start, int end){
    if (start >= end)
        return;

    int p = partition(vettoreFinale, start, end);
    quickSort(vettoreFinale, start, p - 1);

    quickSort(vettoreFinale, p + 1, end);
}


/** in-place partial sorting by vector size**/
static unsigned long partialBucketSort(std::vector<std::vector<InputMatrix::Label>> &vector) {
    unsigned long max=0;
    for(const auto& v: vector)
        max=v.size()>max?v.size():max;
    max++;
    auto indexes=new int[max](); //all 0
    std::vector<std::vector<InputMatrix::Label>> copyArr(vector.size());
    for(auto iter=vector.begin(); iter!=vector.end(); iter++){
        const auto& el=*iter;
        const auto& size=el.size();
        const auto& index=indexes[size];
        Logger::logDebug("Target: "+std::to_string(index));
        copyArr[index]= std::move(el);
        for(int i=size; i<max; i++){
            indexes[i]++;
        }
    }
    vector.clear();
    copy(copyArr.begin(), copyArr.end(), back_inserter(vector));
    if(Configuration::getInstance().isDebug()){
        for(const auto& v:vector)
            Logger::logDebug(std::to_string(v.size()));
    }
    delete[] indexes;

    return max;
}


static void countingSortOnIndex(std::vector<std::vector<InputMatrix::Label>> &vector, int start, int anEnd, int index,
                                std::vector<std::vector<InputMatrix::Label>>& copyArr, int *counter, int maxEl) {
    if(start>=anEnd)
        return;

    for(int i=start; i<=anEnd; i++){
        counter[vector[i][index].index]++;
    }
    counter[0]=start;
    for(int i=1; i<maxEl+1; i++){
        counter[i]+=counter[i-1];
    }
    for(int i=start; i<=anEnd; i++){
        copyArr[i]=vector[i];
    }
    for(int i=anEnd; i>=start; i--){
        /*auto el=copyArr[i];
        auto value=el[index].index;
        auto position= --counter[value];
        vector[position]=el;*/
        vector[--counter[copyArr[i][index].index]]=copyArr[i];
    }
}

static void iterativeRadixSortBucket(std::vector<std::vector<InputMatrix::Label>> &vector, int start, int end,
                                     std::vector<std::vector<InputMatrix::Label>>& copyArr) {

    int maxEl=0;
    for(int i=0; i<vector.size(); i++){
        maxEl=maxEl>vector[i][vector[i].size()-1].index?maxEl:vector[i][vector[i].size()-1].index;
    }
    auto counter= new int[(maxEl+1)]();
    for(int i=vector[start].size()-1; i>=0; i--){
        for(int j=0; j<maxEl+1; j++)
            counter[j]=0;
        countingSortOnIndex(vector, start, end, i, copyArr, counter, maxEl);

    }

    delete[] counter;
}

/**
 *
 * @param vector
 */
static void iterativeRadixSort(std::vector<std::vector<InputMatrix::Label>> &vector, unsigned long max) {
    auto startingIndexes=new int[max]();
    for(int i=vector.size()-1; i>=0; i--) {
        startingIndexes[vector[i].size()]=i;
    }

    std::vector<std::vector<InputMatrix::Label>> copyArr(vector.size());
    for(int i=0; i<max-1; i++){
        int start=startingIndexes[i];
        int end=startingIndexes[i+1]-1;
        iterativeRadixSortBucket(vector, start, end, copyArr);
    }
    int start=startingIndexes[max-1];
    int end=vector.size()-1;
    iterativeRadixSortBucket(vector, start, end, copyArr);
    delete[] startingIndexes;
}



/**
 * three step algorithm with optimization fallback
 * @param mhss
 */
static inline void sort(std::vector<std::vector<InputMatrix::Label>>& mhss, const InputMatrix& input){

    Logger::logInfo("Internal sort");
    internalSort(mhss, input);

    Logger::logInfo("Ended internal sort, start global sorting");
    if(Configuration::getInstance().getOptimization()<4){
        quickSort(mhss, 0, mhss.size()-1);
    }
    else{

        Logger::logInfo("Partial bucket sorting");
        const auto max=partialBucketSort(mhss);

        Logger::logInfo("Iterative radix sorting");
        iterativeRadixSort(mhss, max);
    }

    Logger::logInfo("Ended global sorting");
}
