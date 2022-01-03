#ifndef TABLE_EDITOR_H
#define TABLE_EDITOR_H

#include <string>
#include "Cell.h"
#include "Row.h"
#include "Columns.h"
using namespace std;


// Type::INT
// Pitati na odbrani zasto je enum class???
enum class Type {
	INT, DECIMAL, TEXT
};


struct Rows {
	Row* row;
	Rows* next;
};


class TableEditor {

public:
	TableEditor();
	~TableEditor();

	// Ucitavanje i cuvanje tabele
	int importTable(string table);
	string exportTable(); // vraca tabelu u datom formatu

	// Selekcije u tabeli
	void selectColumn(string col_name);
	void selectRow(int row_position);
	void selectCell(int row_position, string col_name);
	void deselect();

	// Manipulacija nad strukturom tabele
	void insertRow();
	void insertColumn(string col_name, Type type);
	void editCellValue();
	void deleteContent();

	// Obrada podataka u tabeli
	void add(double value);
	void mul(double value);
	void sortByValue(string col_name, bool asc);

	// Pretraga podataka u tabeli
	int findFirstOf(string value, string col_name);
	int findLastOf(string value, string col_name);

	// Statisticki podaci u tabeli
	int countValues(string value, string col_name);
	int countDistinctValues(string col_name);

private:
	Columns* columns;
	Rows* firstRow;
	Rows* lastRow;
	// void pointer (moze da pokazuje i na red i na kolonu i na celiju)
	void* selected;
	char selectedType; // C -> column; R -> row; c -> cell;
	char selectedCellType;
	int numRows;

	// private methods
	void addRow(Row* row);
	Columns* readHeader(string table, int &pos, bool& error);
	string readData(string table, int& pos, bool& error);
	bool compareVals(string v1, string v2, bool numeric, bool asc);
};

#endif
