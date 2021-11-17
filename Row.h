#ifndef ROW_H
#define ROW_H

#include "Cell.h"
#include "Columns.h"
#include <string>
using namespace std;

struct Elem {
	Cell *cell;
	Elem *next;
};


class Row {
public:
	Row();
	~Row();

	Elem* getRow();
	Cell *getCell();
	void setRowHead(Elem* head);
	void addCell(string data);
	void deleteCell(int numOfColumn);
	Elem* getLastRow();
	string getColumnValue(int colNum);

private:
	Elem *head, *last;
	int numOfElem;
};

#endif
