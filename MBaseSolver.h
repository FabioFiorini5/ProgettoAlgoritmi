//
// Created by Blasko Racu on 19/09/21.
//

#ifndef ALQWARITZMI_MBASESOLVER_H
#define ALQWARITZMI_MBASESOLVER_H


#include "MinimumHittingSetSolver.h"

class MBaseSolver: public MinimumHittingSetSolver {
public:

    void solve(InputMatrix& input) override;

private:

    int check(const bool *pBoolean, int matrix, InputMatrix& inputMatrix);

    [[nodiscard]] int getSuccessor(int val, int other) const;

    int getMax(const bool *element, int size) const;

    bool* getRepresentativeVector(const bool *pBoolean, InputMatrix &matrix);

    bool* getRepresentativeVector(int index, InputMatrix &matrix);

    bool *generateCandidate(int size, const bool *father, int indexToAdd);

    void print(std::ostream &stream, const bool *pBoolean, int size) const;

    [[nodiscard]] int evaluateTruthMap(bool contains0, bool contains1, bool contains2, bool contains3) const;
};
#endif //ALQWARITZMI_MBASESOLVER_H
