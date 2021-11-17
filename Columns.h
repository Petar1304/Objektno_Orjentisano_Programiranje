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
	void subNumOfCols();
	void setColumnHead(Column* head);

	int findColNum(string col_name);
	// check if column has numeric type
	bool isColNumType(string col_name);

	string printColumn();
	Column* getColumn();
	int getNumOfCols();


private:
	Column *head, *last;
	int numOfCols;
};

#endif