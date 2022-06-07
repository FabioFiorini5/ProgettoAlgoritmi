//
// Created by Blasko Racu on 19/09/21.
//

#ifndef ALQWARITZMI_MBASESOLVER_V6_H
#define ALQWARITZMI_MBASESOLVER_V6_H


#include "MinimumHittingSetSolver.h"
#include <vector>
#include "Utility.h"

class MBaseSolverV6 {
public:

    using Mhs = struct _mhs {
        _mhs(bool *mhs, int min, int max) : mhs(mhs), min(min), max(max) {}

        bool* mhs;
        int min;
        int max;
    };
    
    MBaseSolverV6(int columnLength):columnSize(columnLength){ //c0,1,2,3 //da riguardare
        truthMap[0]=0;//0,0,0,0 caso non verificabile
        truthMap[1]=-1;//0,0,0,1 caso non verificabile -> mhs + mhs
        truthMap[2]=1;//0,0,1,0 la colonna aggiunta è un mhs
        truthMap[3]-1;//0,0,1,1 la colonna aggiunta è un mhs e l'insieme di partenza non è l'insieme vuoto
        truthMap[4]=-1;//0,1,0,0 l'insieme di partenza è un mhs quindi lo scarto perché aggiungo una colonna vuota e automaticamente non è più un mhs //non dovrebbe poter accadaere per la logica
        truthMap[5]=-1;//0,1,0,1 la colonna aggiunta è inclusa nell'insieme di partenza che è un mhs, quindi non è più un mhs e va scartato
        truthMap[6]=1;//0,1,1,0 la colonna aggiunta è il completamento dell'insieme di partenza
        truthMap[7]=1;//0,1,1,1 la colonna aggiunta è il completamento dell'insieme di partenza più qualche elemento in comune
        truthMap[8]=-1;//1,0,0,0 non ci sono elementi validi, quindi scarto perché qualsiasi insieme contenente questo insieme non può essere mhs
        truthMap[9]=-1;//1,0,0,1 qualsiasi elemento valido viene fornito sia dalla colonna aggiunta sia dall'elemento di partenza quindi scarto perché sono entrambi lo stesso elemento, quindi non mhs
        truthMap[10]=0;//1,0,1,0 ok, insieme di partenza più colonna che fornisce almeno un valore
        truthMap[11]=-1;//1,0,1,1 qualsiasi elemento valido viene fornito o dalla colonna aggiunta o è in comune con l'elemento di partenza quindi scarto
        truthMap[12]=-1;//1,1,0,0 la colonna non fornisce alcun elemento valido -> scarto
        truthMap[13]=-1;//1,1,0,1 la colonna non fornisce alcun elemento valido -> scarto
        truthMap[14]=0;//1,1,1,0 la colonna fornisce almeno un elemento valido -> ok
        truthMap[15]=0;//1,1,1,1 la colonna fornisce almeno un elemento valido -> ok
    }

    virtual ~MBaseSolverV6()=default;

    std::vector<Mhs> solve(InputMatrix& input);

private:

    int check(const bool* pBoolean, int matrix, const InputMatrix& inputMatrix) const;

    [[nodiscard]] int getSuccessor(int val, int other) const;

    [[nodiscard]] bool* getRepresentativeVector(const bool* pBoolean, const InputMatrix &matrix) const;

    [[nodiscard]] bool* getRepresentativeVector(int index, const InputMatrix &matrix) const;

    void generateCandidate(const bool* father, int indexToAdd, bool* candidate) const;

    void printVector(std::ostream &stream, const bool *pBoolean, InputMatrix matrix) const;

    [[nodiscard]] signed char evaluateTruthMap(unsigned char contains0, unsigned char contains1, unsigned char contains2, unsigned char contains3) const;

    signed char truthMap[16];

    [[nodiscard]] bool containsMhs(const bool* vector, const std::vector<Mhs> &mhss) const;

    [[nodiscard]] bool isSubset(const bool* vector, const bool* vector1, int max, int min=0) const;

    int columnSize;

    void printRepVector(const bool *pBoolean, int length) const;


    [[nodiscard]] bool canContinue(const bool *pBoolean, int length) const;


    std::vector<InputMatrix::Label> getLabels(bool *pBoolean, InputMatrix& inputMatrix);


    void printCsv();
};
#endif //ALQWARITZMI_MBaseSolverV6_H
