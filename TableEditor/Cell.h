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

	// konverzije u INT i FLOAT
	int toInt();
	float toFloat();

private:
	string data;
};

#endif