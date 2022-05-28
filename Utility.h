//
// Created by Blasko Racu on 28/05/22.
//

#ifndef ALG_STR_DATI_UTILITY_H
#define ALG_STR_DATI_UTILITY_H

#endif //ALG_STR_DATI_UTILITY_H


inline static int getMin(const bool *pInt, int size) {
    for(int i=0; i<size; i++){
        if(pInt[i])
            return i;
    }
    return 0;
}

inline static int getMax(const bool* element, int size){
    int max=size;
    do{
        max--;
    }
    while(max>-1&&!element[max]);
    return max;
}