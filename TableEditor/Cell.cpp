#include "Cell.h"
#include <string>
using namespace std;


Cell::Cell() {
	this->data = "";
}

Cell::Cell(string data) {
	this->data = data;
}


void Cell::setData(string data) {
	this->data = data;
}

string Cell::getData() {
	return this->data;
}

int Cell::toInt() {
	// stoi() converts string to int
	return stoi(this->data);
}

float Cell::toFloat() {
	// stof() converts string to float
	return stof(this->data);
}
