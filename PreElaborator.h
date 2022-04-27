//
// Created by Utente on 27/04/2022.
//

#ifndef ALG_STR_DATI_PREELABORATOR_H
#define ALG_STR_DATI_PREELABORATOR_H


#include "InputMatrix.h"

class PreElaborator {

public:
    void clean(InputMatrix& matrix);
private:
    void cleanRows(InputMatrix& matrix);
    void cleanCols(InputMatrix& matrix);
    bool isIncluded(const bool* mat_i, const bool* mat_j, int columnLength);

    void print(bool* boolP, int len) const;
};


#endif //ALG_STR_DATI_PREELABORATOR_H
