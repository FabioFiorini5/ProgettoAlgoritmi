//
// Created by Blasko Racu on 19/09/21.
//

#ifndef ALQWARITZMI_INPUTMATRIX_H
#define ALQWARITZMI_INPUTMATRIX_H

#include <string>
#include <fstream>
#include <iostream>
#include <vector>

class InputMatrix {

public:
    explicit InputMatrix(const std::string& path);

    [[nodiscard]] int getColumnLength() const;

    InputMatrix();

    [[nodiscard]] int getRowLength() const;

    [[nodiscard]] bool getValueAt(int row, int column) const;

    [[nodiscard]] bool* getRow(int rowNum) const;

    [[nodiscard]] std::vector<bool> getCol(int colNum) const;

    void removeColumn(int colNum);

    void removeRow(int rowNum);

    void print() const;

private:
    void reduceColumnLength();

    void reduceRowLength();

    void setColumnLength(int val){
        this->columnLength=val;
    }
    void setRowLength(int val){
        this->rowLength=val;
    }

    bool** matrix;
    int columnLength=0;
    int rowLength=0;
};


#endif //ALQWARITZMI_INPUTMATRIX_H
