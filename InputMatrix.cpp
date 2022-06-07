//
// Created by Blasko Racu on 19/09/21.
//

#include <sstream>
#include "InputMatrix.h"
#include "Logger.h"

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
    Logger::logInfo("Loading file");
    std::ifstream infile(path);
    std::string line;
    //std::cout << path << std::endl;
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
    setColumnLengthOriginal(column);
    setColumnLength(column);
    setRowLength(row);
    matrix=new bool*[rowLength]();
    std::ifstream infile2(path);
    int rowCount=0;
    while (std::getline(infile2, line))
    {
        if(line.starts_with(";;;")){
            if(line.starts_with(";;; Map")){
                loadLabels(line);
            }
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
    //printJson(//Logger::logDebug);
    Logger::logInfo("Loaded!");
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
        delete[] matrix[i];

        matrix[i]=newRow;
    }

    auto newLabels=new Label[columnLength]();
    for(int j=0; j<colNum; j++) {
        newLabels[j] = labels[j];
    }
    for(int j=colNum; j<columnLength; j++) {
        newLabels[j] = labels[j + 1];
    }
    delete[] labels;
    labels=newLabels;
}

void InputMatrix::removeRow(int rowNum){
    reduceRowLength();
    for(int i=rowNum; i<rowLength; i++)
    {
        for(int j=0; j<columnLength; j++){
            matrix[i][j]=matrix[i+1][j];
        }
    }
    delete[] matrix[rowLength];
}

void InputMatrix::reduceColumnLength(){
    columnLength--;
}

void InputMatrix::reduceRowLength(){
    rowLength--;
}

void InputMatrix::print(const std::function<void(const std::string&)>& printer) const{
    std::string line;
    for(int j=0; j<columnLength; j++){
        line.append(labels[j].letter+ std::to_string(labels[j].number) +"|");
    }
    printer(line);
    printer("\n");
    line.clear();
    for(int i=0; i<rowLength; i++){
        for(int j=0; j<columnLength; j++){
            line.append("["+std::to_string(matrix[i][j])+"]");
        }
        printer(line);
        printer("\n");
        line.clear();
    }
}

void InputMatrix::loadLabels(std::string& string) {
    //Logger::logDebug(string);
    labels=new Label [columnLength]();
    std::string delimiter = " ";
    string.erase(0, 8);
    size_t pos = 0;
    std::string token;
    int i=0;
    while ((pos = string.find(delimiter)) != std::string::npos) {
        token = string.substr(0, pos);

        int parenthesisPosition= token.find("(") + 1;

        std::string val=token.substr(parenthesisPosition, token.size() - parenthesisPosition - 1);

        labels[i].index = i+1;
        labels[i].letter = val[0];
        labels[i].number = std::stoi(val.substr(1));

        labels[i].copied.push_back(labels[i]);
        i++;
        string.erase(0, pos + delimiter.length());
    }

    std::string line;
    for(int j=0; j<columnLength; j++){
        if(labels[j].index==0){
            labels[j].index = j+1;
            labels[j].letter = 'a';
            labels[j].number = j+1;
            labels[j].copied.push_back(labels[j]);
        }
        line.append(std::to_string(labels[j].letter));
        line.append(std::to_string(labels[j].number));
        line.append(" ");
        line.append(std::to_string(labels[j].index));
        line.append(", ");
    }
    //Logger::logDebug(line);

}

InputMatrix::Label *InputMatrix::getLabels() const {
    return labels;
}

void InputMatrix::joinColumn(int source, int copy) {
    reduceColumnLength();
    for(int i=0; i<rowLength; i++){
        auto newRow=new bool[columnLength] ();
        for(int j=0; j<copy; j++){
            newRow[j]=matrix[i][j];
        }
        for(int j=copy; j<columnLength; j++){
            newRow[j]=matrix[i][j+1];
        }
        delete[] matrix[i];

        matrix[i]=newRow;
    }

    auto newLabels=new Label[columnLength]();
    for(int j=0; j<copy; j++) {
        newLabels[j].number = labels[j].number;
        newLabels[j].letter = labels[j].letter;
        newLabels[j].copied = labels[j].copied;
        newLabels[j].index  = labels[j].index ;

    }
    for(int j=copy; j<columnLength; j++) {
        newLabels[j].number = labels[j+1].number;
        newLabels[j].letter = labels[j+1].letter;
        newLabels[j].copied = labels[j+1].copied;
        newLabels[j].index  = labels[j+1].index ;
    }
    newLabels[source].copied.push_back(labels[copy]);
    delete[] labels;
    labels=newLabels;
}

int InputMatrix::getColumnLengthOriginal() const {
    return columnLengthOriginal;
}

void InputMatrix::setColumnLengthOriginal(int columnLengthOriginal) {
    InputMatrix::columnLengthOriginal = columnLengthOriginal;
}

void InputMatrix::printJson(const std::function<void(const std::string&)>& printer) {
    printer("{");
    printer("\n");
    printer("\t\"sets\": [");
    printer("\n");
    for(int i=0; i<rowLength; i++){
        auto max=getMax(this->matrix[i], columnLength);
        std::string line;
        line.append("\t\t[");
        for(int j=0; j<=max; j++){
            if(this->matrix[i][j]){
                line.append(std::to_string(j+1));
                line.append(j<max-1?",":"");
            }
        }
        line.append("]");
        line.append((i<rowLength-1?",":""));
        printer(line);
        printer("\n");
    }
    printer("\t]");
    printer("\n");
    printer("}");
    printer("\n");
}

int InputMatrix::getMax(const bool* element, int size) const {
    int max=size;
    do{
        max--;
    }
    while(max>-1&&!element[max]);
    return max;
}

InputMatrix::~InputMatrix() {
    for(int i=0; i<rowLength; i++){
        delete[] matrix[i];
    }
    delete[] matrix;

    delete[] labels;
}

void InputMatrix::print() const {
    Logger::logOutCsv(std::to_string(rowLength));
    Logger::logOutCsv(std::to_string(columnLength));
}

