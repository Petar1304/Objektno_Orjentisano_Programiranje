#ifndef CELL_H
#define CELL_H

#include <string>
using namespace std;


class Cell {
public:

	Cell();
	// Dovoljan je obican destruktor
	
	//Cell(string data);
	
	void setData(string data);
	string getData();
	void addVal(double val);
	void mulVal(double val);

private:
	string data;

	// private methods
	string showOneDec(string str);
};

#endif
