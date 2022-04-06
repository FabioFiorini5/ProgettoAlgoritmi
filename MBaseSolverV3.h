//
// Created by Blasko Racu on 19/09/21.
//

#ifndef ALQWARITZMI_MBASESOLVER_V3_H
#define ALQWARITZMI_MBASESOLVER_V3_H


#include "MinimumHittingSetSolver.h"

class MBaseSolverV3: public MinimumHittingSetSolver {
public:
    
    MBaseSolverV3(){
        truthMap[0]=0;//0,0,0,0
        truthMap[1]=-1;//0,0,0,1
        truthMap[2]=1;//0,0,1,0
        truthMap[3]=-1;//0,0,1,1
        truthMap[4]=-1;//0,1,0,0
        truthMap[5]=-1;//0,1,0,1
        truthMap[6]=1;//0,1,1,0
        truthMap[7]=1;//0,1,1,1
        truthMap[8]=-1;//1,0,0,0
        truthMap[9]=-1;//1,0,0,1
        truthMap[10]=0;//1,0,1,0
        truthMap[11]=-1;//1,0,1,1
        truthMap[12]=-1;//1,1,0,0
        truthMap[13]=-1;//1,1,0,1
        truthMap[14]=0;//1,1,1,0
        truthMap[15]=0;//1,1,1,1
    }

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
    
    short int truthMap[16];
};
#endif //ALQWARITZMI_MBaseSolverV3_H
