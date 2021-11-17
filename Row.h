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
	Elem* getLastRow();

	// vraca vrednost kolone broj colNum u datom redu
	string getColumnValue(int colNum);

	// menja prvu celiju u redu
	void setRowHead(Elem* head);

	// dodaje celiju u red
	void addCell(string data);

	// brise celiju
	void deleteCell(int numOfColumn);
	
private:
	Elem *head, *last;
	int numOfElem;

};

#endif