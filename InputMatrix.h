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
    using Label = struct _label {
        char letter;
        unsigned short number;
        unsigned short index;
        std::vector<_label> copied;
        std::string toString() const {

            return letter+std::to_string(number)+"("+std::to_string(index)+")";
        }
    };

    explicit InputMatrix(const std::string& path);

    virtual ~InputMatrix();

    [[nodiscard]] int getColumnLength() const;

    InputMatrix();

    [[nodiscard]] int getRowLength() const;

    [[nodiscard]] bool getValueAt(int row, int column) const;

    [[nodiscard]] bool* getRow(int rowNum) const;

    [[nodiscard]] std::vector<bool> getCol(int colNum) const;

    [[nodiscard]] Label *getLabels() const;

    void removeColumn(int colNum);

    void removeRow(int rowNum);

    void print(const std::function<void(const std::string&)>& printer) const;

    void joinColumn(int source, int copy);

    [[nodiscard]] int getColumnLengthOriginal() const;

    void setColumnLengthOriginal(int columnLengthOriginal);

    void printJson(const std::function<void(const std::string&)>& printer);

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

    int columnLengthOriginal;

private:

    Label* labels;

    void loadLabels(std::string& string);

    int getMax(const bool *element, int size) const;
};


#endif //ALQWARITZMI_INPUTMATRIX_H
