//
// Created by Blasko Racu on 19/09/21.
//

#ifndef ALQWARITZMI_INPUTMATRIX_H
#define ALQWARITZMI_INPUTMATRIX_H


class InputMatrix {

public:
    explicit InputMatrix(const std::string& path);

    int getColumnLength();

    InputMatrix();

    int getRowLength();

    bool getValueAt(int row, int column);

private:
    bool** matrix;
    int columnLength;
    int rowLength;
};


#endif //ALQWARITZMI_INPUTMATRIX_H
