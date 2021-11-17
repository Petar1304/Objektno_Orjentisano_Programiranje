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

	// dodavnje/brisanje kolone
	void addColumn(string name, char type);
	void deleteColumn(string name);

	// dodaje/oduzima 1 od broja kolona
	void addNumOfCols();
	void subNumOfCols();

	// menja prvu kolonu
	void setColumnHead(Column* head);

	// vraca broj kolone sa imenom col_name
	int findColNum(string col_name);

	// check if column has numeric type
	bool isColNumType(string col_name);

	// vraca kolonu u formatu za ispis
	string printColumn();

	// vraca pointer na prvu kolonu
	Column* getColumn();

	// vraca broj kolona
	int getNumOfCols();


private:
	Column *head, *last;
	int numOfCols;
};

#endif