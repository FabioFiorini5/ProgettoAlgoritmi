//
// Created by Blasko Racu on 19/09/21.
//

#include "InputMatrix.h"

/**
 * Ritorna quanto è lunga una colonna, cioè il numero di righe
 * @return integer
 */

int InputMatrix::getColumnLength() const{
    return columnLength;
}


/**
 * Restituisce la lunghezza di una riga, o il numero di colonne
 * @return
 */
int InputMatrix::getRowLength() const{
    return rowLength;
}

/**
 * restituisce il valore di una cella
 * @param row
 * @param column
 * @return bool
 */
bool InputMatrix::getValueAt(int row, int column) const {
    return matrix[row][column];
}

/**
 * Costruttore base per il caso di test
 { {B3,B4},
 A1,A2,B4},
 {A2,A5,B3,B4} }
 */
InputMatrix::InputMatrix() {
    setColumnLength(5);
    setRowLength(3);
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

/**
 * Costruttore per un caso generico. Richiede il path del file di riferimento del caso.
 * @param path std::string
 */
InputMatrix::InputMatrix(const std::string& path) {
    std::ifstream infile(path);
    std::string line;
    int row=0;
    int column=0;
    while (std::getline(infile, line))
    {
        if(line.starts_with(";;;")){
            continue;
        }

        column=(int)(line.size()-1)/2;
        row++;
    }
    setColumnLength(column);
    setRowLength(row);
    matrix=new bool*[rowLength]();
    std::ifstream infile2(path);
    int rowCount=0;
    while (std::getline(infile2, line))
    {
        if(line.starts_with(";;;")){
            continue;
        }
        if(rowCount>=rowLength)
            break;
        matrix[rowCount]=new bool[columnLength]();
        for(int i=0; i<columnLength; i++){
            char c=line[i*2];
            if(c=='1')
                matrix[rowCount][i]=true;
        }
        rowCount++;
    }
    print();
}

bool* InputMatrix::getRow(int rowNum) const{
    return matrix[rowNum];
}

std::vector<bool> InputMatrix::getCol(int colNum) const{
    std::vector<bool> column(rowLength);
    for(int i=0; i<rowLength; i++)
        column[i]=matrix[i][colNum];
    return column;
}

void InputMatrix::removeColumn(int colNum){
    reduceColumnLength();
    for(int i=0; i<rowLength; i++){
        auto newRow=new bool[columnLength] ();
        for(int j=0; j<colNum; j++){
            newRow[j]=matrix[i][j];
        }
        for(int j=colNum; j<columnLength; j++){
            newRow[j]=matrix[i][j+1];
        }
        delete matrix[i];
        matrix[i]=newRow;
    }
}

void InputMatrix::removeRow(int rowNum){
    reduceRowLength();
    for(int i=rowNum; i<rowLength; i++)
    {
        for(int j=0; j<columnLength; j++){
            matrix[i][j]=matrix[i+1][j];
        }
    }
    delete matrix[rowLength];
}

void InputMatrix::reduceColumnLength(){
    columnLength--;
}

void InputMatrix::reduceRowLength(){
    rowLength--;
}

void InputMatrix::print() const{
    for(int i=0; i<rowLength; i++){
        for(int j=0; j<columnLength; j++){
            std::cout<<"["<<matrix[i][j]<<"]";
        }
        std::cout<<std::endl;
    }
}

