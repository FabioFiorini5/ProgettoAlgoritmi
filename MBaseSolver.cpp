//
// Created by Blasko Racu on 19/09/21.
//

#include <queue>
#include <iostream>
#include "MBaseSolver.h"

/**
 * Main func
 * @param input
 */
void MBaseSolver::solve(InputMatrix& input) {
    int size=input.getColumnLength();
    //pointer perché la coda così tiene all'interno i puntatori.
    //ogni volta che viene fatto pop bisogna ricordarsi di eseguire il delete.
    auto emptySet=new bool[size](); //new -> deve essere fatto il delete
    std::queue<bool*> queue;
    queue.push(emptySet);
    // ciclo sulla coda, fino a quando non si svuota
    while(queue.front()!= nullptr){
        bool* current=queue.front();
        std::cerr<<"Current: "<<std::endl;
        //elemento di test
        print(std::cerr, current, size);
        //per ogni combinazione di colonna con l'elemento di test cicla
        for(int i=getSuccessor(getMax(current, size), size); i<size; i++) {
            auto candidate = generateCandidate(size, current, i);

            std::cerr<<"Candidate: "<<std::endl;
            print(std::cerr, candidate, size);

            //la funzione check restituisce 1 se MHS, 0 se candidato
            int result= check(current, i, input);
            if(result==1&&i!=size-1)
                print(std::cout, candidate, size);
            if(result==0)
                queue.push(candidate);
        }
        delete[] current;
        //queue pop non restituisce l'elemento, quindi per ottenerlo si fa .front
        queue.pop();
    }
}

void MBaseSolver::print(std::ostream& stream, const bool *pBoolean, int size) const{
    for(int i=0; i<size; i++){
        stream<<"[";
        stream<<(pBoolean[i]?"1":"0");
        stream<<"]";
    }
    stream<<std::endl;
}

/**
 * genera la combinazione dell'array di booleani in esame con l'indice della colonna da aggiungere
 * @param size
 * @param father
 * @param indexToAdd
 * @return
 */
bool* MBaseSolver::generateCandidate(int size, const bool* father, int indexToAdd){
    auto bitset= new bool[size]; //new -> deve essere fatto delete
    for(int i=0; i<size; i++){
        bitset[i]=father[i]||i==indexToAdd;
    }
    return bitset;
}

int MBaseSolver::check(const bool *pBoolean, int toAdd, InputMatrix& inputMatrix) {
    int size=inputMatrix.getRowLength();
    bool* representativeVector=getRepresentativeVector(pBoolean, inputMatrix);
    bool* other=getRepresentativeVector(toAdd, inputMatrix);
    char unionVector[size];
    for(int i=0; i<size; i++)
        unionVector[i]=representativeVector[i];


    bool contains0=false;
    bool contains1=false;
    bool contains2=false;
    bool contains3=false;
    for(int i=0; i<size; i++){
        if(other[i])
        {
            unionVector[i]+=2;
        }
        if(unionVector[i]==0)
            contains0=true;
        if(unionVector[i]==1)
            contains1=true;
        if(unionVector[i]==2)
            contains2=true;
        if(unionVector[i]==3)
            contains3=true;
    }
    delete[] representativeVector;
    delete[] other;
    return evaluateTruthMap(contains0, contains1, contains2, contains3);
}

int MBaseSolver::getSuccessor(int val, int other) const {
    return val+1< other?
        val+1:
        other;
}

int MBaseSolver::getMax(const bool* element, int size) const {
    int max=size;
    do{
        max--;
    }
    while(max>-1&&!element[max]);
    return max;
}

bool* MBaseSolver::getRepresentativeVector(const bool *pBoolean, InputMatrix &inputMatrix) {
    int size=inputMatrix.getRowLength();
    auto column=new bool[size]();
    for(int i=0; i<size; i++)
        column[i]=false;
    for(int i=0; i<inputMatrix.getColumnLength(); i++){
        if(!pBoolean[i])
            continue;
        for(int j=0; j<size; j++){
            column[j]=column[j]||inputMatrix.getValueAt(j,i);
        }
    }
    return column;
}

bool* MBaseSolver::getRepresentativeVector(int index, InputMatrix &inputMatrix) {
    auto bitset=new bool[inputMatrix.getColumnLength()](); //new -> va fatto il delete
    bitset[index]=true;
    auto toReturn=getRepresentativeVector(bitset, inputMatrix);
    delete[] bitset; //delete
    return toReturn;
}
/**
 * truth map, fa una verifica dei vari casi che  possono verificarsi
 * Al momento non ricordo il perché abbia fatto questa logica, ma va a rendere il caso di test valido.
 * @param contains0
 * @param contains1
 * @param contains2
 * @param contains3
 * @return
 */
int MBaseSolver::evaluateTruthMap(bool contains0, bool contains1, bool contains2, bool contains3) const {
    if(contains0&&contains1&&contains2)
        return 0;
    if(contains0&&contains1)
        return -1;
    if(contains0&&contains2&&contains3)
        return -1;
    if(contains0&&contains2)
        return 0;
    if(contains0)
        return -1;
    if(contains1&&contains2)
        return 1;
    if(contains1)
        return -1;
    if(contains2&&contains3)
        return -1;
    if(contains2)
        return 1;
    return -1;
}


