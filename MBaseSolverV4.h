//
// Created by Blasko Racu on 19/09/21.
//

#ifndef ALQWARITZMI_MBASESOLVER_V4_H
#define ALQWARITZMI_MBASESOLVER_V4_H


#include "MinimumHittingSetSolver.h"

class MBaseSolverV4: public MinimumHittingSetSolver {
public:
    
    MBaseSolverV4(){
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

    ~MBaseSolverV4()=default;

    void solve(InputMatrix& input) override;

private:

    int check(const std::vector<bool>& pBoolean, int matrix, InputMatrix& inputMatrix) const;

    [[nodiscard]] int getSuccessor(int val, int other) const;

    [[nodiscard]] int getMax(const std::vector<bool>& element, int size) const;

    std::vector<bool> getRepresentativeVector(const std::vector<bool>& pBoolean, InputMatrix &matrix) const;

    std::vector<bool> getRepresentativeVector(int index, InputMatrix &matrix) const;

    [[nodiscard]] std::vector<bool> generateCandidate(int size, const std::vector<bool>& father, int indexToAdd) const;

    void print(std::ostream &stream, const std::vector<bool>& pBoolean, int size) const;

    [[nodiscard]] signed char evaluateTruthMap(unsigned char contains0, unsigned char contains1, unsigned char contains2, unsigned char contains3) const;
    
    signed char truthMap[16];
};
#endif //ALQWARITZMI_MBaseSolverV4_H
