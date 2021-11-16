#ifndef COLUMNS_H
#define COLUMNS_H

#include "Cell.h"
#include <string>
using namespace std;


struct Column {
	char type;
	string name;
	Column* next;
};


class Columns {
public:
	Columns();
	~Columns();

	void addColumn(string name, char type);
	void deleteColumn(string name);
	void addNumOfCols();

	string printColumn();
	Column* getColumn();
	int getNumOfCols();


private:
	Column *head, *last;
	int numOfCols;
};

#endif