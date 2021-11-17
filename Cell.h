#ifndef CELL_H
#define CELL_H

#include <string>
using namespace std;


class Cell {
public:
	Cell();
	
	Cell(string data);
	// Dovoljan je obican destruktor

	void setData(string data);
	string getData();
	void addVal(double val);
	void mulVal(double val);

	// konverzije u INT i FLOAT
	int toInt();
	float toFloat();




private:
	string data;

	string showOneDec(string str);
};

#endif