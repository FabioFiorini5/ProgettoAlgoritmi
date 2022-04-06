//
// Created by Blasko Racu on 19/09/21.
//

#include <string>
#include "InputMatrix.h"
#include <fstream>
#include <iostream>

int InputMatrix::getColumnLength() {
    return columnLength;
}
/*
 *
 *  {B3,B4}, 1
 A1,A2,B4}, 2
 {A2,A5,B3,B4} }
 */
InputMatrix::InputMatrix() {
    columnLength=5;
    rowLength=3;
    matrix=new bool*[rowLength]();

    matrix[0]=new bool[columnLength]();
    matrix[1]=new bool[columnLength]();
    matrix[2]=new bool[columnLength]();

    matrix[0][2]=true;
    matrix[0][3]=true;
    matrix[1][0]=true;
    matrix[1][1]=true;
    matrix[1][3]=true;
    matrix[2][1]=true;
    matrix[2][2]=true;
    matrix[2][3]=true;
    matrix[2][4]=true;
}

InputMatrix::InputMatrix(const std::string& path) {
    std::ifstream infile(path);
    std::string line;
    while (std::getline(infile, line))
    {
        if(line.starts_with(";;;")){
            continue;
        }

        columnLength=(line.size()-1)/2;
        rowLength++;
    }
    rowLength--;
    matrix=new bool*[rowLength]();
    std::ifstream infile2(path);
    int rowCount=0;
    while (std::getline(infile2, line))
    {
        if(line.starts_with(";;;")){
            continue;
        }
        matrix[rowCount]=new bool[columnLength] ();
        for(int i=0; i<columnLength; i++){
            char c=line[i*2];
            if(c=='1')
                matrix[rowCount][i]=true;
        }
        rowCount++;
    }
    for(int i=0; i<rowLength; i++){
        for(int j=0; j<columnLength; j++){
            std::cout<<matrix[i][j];
        }
        std::cout<<std::endl;
    }
}


int InputMatrix::getRowLength() {
    return rowLength;
}

bool InputMatrix::getValueAt(int row, int column) {
    return matrix[row][column];
}
