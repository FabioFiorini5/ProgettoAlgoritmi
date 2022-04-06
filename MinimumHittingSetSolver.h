//
// Created by Blasko Racu on 19/09/21.
//

#ifndef ALQWARITZMI_MINIMUMHITTINGSETSOLVER_H
#define ALQWARITZMI_MINIMUMHITTINGSETSOLVER_H

#include "InputMatrix.h"

class MinimumHittingSetSolver
{

    public:
        // pure virtual function
        virtual void solve(InputMatrix& input) = 0;


};

#endif //ALQWARITZMI_MINIMUMHITTINGSETSOLVER_H
