#include "Cell.h"
#include <string>
using namespace std;


// Konstruktor
Cell::Cell() {
	this->data = "";
}


void Cell::setData(string data) {
	this->data = data;
}


string Cell::getData() {
	return this->data;
}


void Cell::addVal(double val) {
    // AKO JE CELIJA PRAZNA NE RADIMO NISTA
    if (this->data != "") {
        this->data = showOneDec(to_string(stod(this->data) + val));
    }
}


void Cell::mulVal(double val) {
    if (this->data != "") {
        this->data = showOneDec(to_string(stod(this->data) * val));
    }
}


string Cell::showOneDec(string str) {
    string newStr;
    int i = 0;
    while (1) {
        if (str[i] == '.') {
            newStr.push_back(str[i]);
            newStr.push_back(str[i + 1]);
            break;
        }
        newStr.push_back(str[i]);
        i++;
    }
    return newStr;
}