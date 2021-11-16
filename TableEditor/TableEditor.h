#ifndef TABLE_EDITOR_H
#define TABLE_EDITOR_H

#include <string>
#include "Cell.h"
#include "Row.h"
#include "Columns.h"
using namespace std;


// Type::INT
enum Type {
	INT, DECIMAL, TEXT
};


//struct Cells {
//	Cell* cell;
//	Cells* next;
//};


struct Rows {
	Row* row;
	Rows* next;
};


class TableEditor {

public:
	TableEditor();
	~TableEditor();

	int importTable(string table);
	string exportTable(); // vraca tabelu u datom formatu

	void addRow(Row* row);

	void selectColumn(string col_name);
	void selectRow(int row_position);
	void selectCell(int row_position, string col_name);
	void deselect();

	void* getSelected();

	void insertRow();
	void insertColumn(string col_name, Type type);



	void printSelected();
	/*
	void deleteContent();
	void editCellValue();
	*/

private:
	Columns* columns;
	Rows* firstRow;
	Rows* lastRow;
	// void pointer (moze da pokazuje i na red i na kolonu i na celiju)
	void* selected;
	char selectedType; // C -> column; R -> row; c -> cell;

	int numRows;

	// private methods
	Columns* readHeader(string table, int &pos, bool& error);
	string readData(string table, int& pos, bool& error);
};

#endif