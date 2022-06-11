//
// Created by Blasko Racu on 28/05/22.
//

#ifndef ALG_STR_DATI_RESULTPRINTER_H
#define ALG_STR_DATI_RESULTPRINTER_H


#include <vector>
#include "MBaseSolverV6.h"

class ResultPrinter {


public:
    void printResults(std::vector<MBaseSolverV6::_mhs> mhss, InputMatrix& input);

    int partition(std::vector<std::vector<InputMatrix::Label>> &vettoreFinale, int start, int end);

    bool compareMhs(std::vector<InputMatrix::Label> &a, std::vector<InputMatrix::Label> &b);

    void quickSort(std::vector<std::vector<InputMatrix::Label>> &vettoreFinale, int start, int end);

    void extractMhs(bool *mhs, int pos_attuale, std::vector<std::vector<InputMatrix::Label>> &vettoreFinale,
                    std::vector<InputMatrix::Label> &vettoreParziale, InputMatrix &inputMatrix);

    std::vector<std::vector<InputMatrix::Label>> extractResults(std::vector<MBaseSolverV6::Mhs> &vector, InputMatrix &matrix);

    void calculateExpectedResults(const std::vector<MBaseSolverV6::Mhs> &vector, InputMatrix &matrix);

    void clearCopied(std::vector<std::vector<InputMatrix::Label>>& mhss);

    std::vector<std::vector<InputMatrix::Label>>
    getCartesianMul(std::vector<MBaseSolverV6::Mhs> &vector, InputMatrix &matrix);

    std::vector<InputMatrix::Label> convertMhs(MBaseSolverV6::_mhs &mhs, InputMatrix &matrix);

    void doExtraction(std::vector<MBaseSolverV6::Mhs> &vector, InputMatrix &matrix);

    void doCartesianMul(std::vector<MBaseSolverV6::Mhs> &mhss, InputMatrix &input);

    void print(const std::vector<InputMatrix::Label> &vector);

    void analyze(const std::vector<MBaseSolverV6::_mhs>& vector1, InputMatrix &matrix);
};


#endif //ALG_STR_DATI_RESULTPRINTER_H
