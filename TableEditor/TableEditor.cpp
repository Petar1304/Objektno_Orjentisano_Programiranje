#include <iostream>
#include <string>
#include "Cell.h"
#include "Row.h"
#include "TableEditor.h"
using namespace std;


// Constructor
TableEditor::TableEditor() {
	this->columns = nullptr;
	this->firstRow = nullptr;
	this->lastRow = nullptr;
	this->numRows = 0;
	this->selected = nullptr;
	this->selectedType = ' ';
}


TableEditor::~TableEditor() {
	while (this->firstRow != nullptr) {
		Rows* old = this->firstRow;
		this->firstRow = this->firstRow->next;
		
		old->row->~Row();
	}
	this->columns->~Columns();

	this->columns = nullptr;
	this->firstRow = nullptr;
	this->lastRow = nullptr;
	this->numRows = 0;
	this->selected = nullptr;
	this->selectedType = ' ';
}


string TableEditor::readData(string table, int& pos, bool& error) {
	string data;
	bool zarez = false;
	
	// obraditi moguce greske
	while (1) {
		if ((table[pos] == ',' && zarez == false) || table[pos] == '\n') {
			if (table[pos] != '\n') {
				pos++;
			}
			break;
		}
		if (table[pos] == '\"') {
			zarez = !zarez;
		}
		data.push_back(table[pos]);
		pos++;
	}

	return data;
}



Columns* TableEditor::readHeader(string table, int& pos, bool& error) {
	Columns* cols = new Columns();
	char t; // type
	string d; //data
	
	// read first row
	while (table[pos] != '\n') {
		t = table[pos];
		pos += 2;
		d = readData(table, pos, error);
		cols->addColumn(d, t);
	}
	pos++;
	return cols;
}


// Dodavanje reda u tabelu
void TableEditor::addRow(Row* row) {

	Rows* newRow = new Rows();
	newRow->row = row;
	newRow->next = nullptr;

	if (this->firstRow == nullptr) {
		this->firstRow = newRow;
	}
	else {
		this->lastRow->next = newRow;
	}
	this->lastRow = newRow;
	this->numRows++;
}



int TableEditor::importTable(string table) {
	
	int pos = 0;
	string data;
	bool error = false;

	this->columns = readHeader(table, pos, error);
	if (error == true) return -1;

	// read all rows
	while (pos < table.length()) {

		Row* row = new Row();

	// ulancana lista za celije u redovima
	while (table[pos] != '\n') {		
			data = readData(table, pos, error);
			if (error == true) return this->numRows;
			row->addCell(data);
		}
	// ulancana lista za redova
		this->addRow(row);	
		pos++;
	}
	return -2;
}


string TableEditor::exportTable() {
	string table;

	// add heading
	table.append(this->columns->printColumn());

	for (Rows* temp = this->firstRow; temp != nullptr; temp = temp->next) {
		int i = 0;
		for (Elem* t = temp->row->getRow(); t != nullptr; t = t->next) {
			table.append(t->cell->getData());
			if (i < this->columns->getNumOfCols() - 1) {
				table.push_back(',');
			}
			i++;
		}
		table.push_back('\n');
	}	
	table.push_back('\n');

	return table;
}



void TableEditor::selectColumn(string col_name) {
	
	for (Column* temp = this->columns->getColumn(); temp != nullptr; temp = temp->next) {
		if (temp->name == col_name) {
			this->selected = temp;
			this->selectedType = 'C'; 
			break;
		}
	}
}


void TableEditor::selectRow(int row_position) {
	int rowNum = 0;
	for (Rows* temp = this->firstRow; temp != nullptr; temp = temp->next) {
		if (rowNum == row_position) {
			this->selected = temp;
			this->selectedType = 'R';
			break;
		}
		rowNum++;
	}
}


void TableEditor::selectCell(int row_position, string col_name) {
	int num_col = 0;
	for (Column* t = this->columns->getColumn(); t != nullptr; t = t->next) {
		if (t->name == col_name) {
			break;
		}
		num_col++;
	}

	int row_num = 0;
	Elem* row = new Elem();
	for (Rows* temp = this->firstRow; temp != nullptr; temp = temp->next) {
		if (row_num == row_position) {
			row = temp->row->getRow();
			break;
		}
		row_num++;
	}

	for (int i = 0; i < num_col; i++) {
		row = row->next;
	}
	this->selected = row->cell;
	this->selectedType = 'c';
}


void TableEditor::deselect() {
	this->selected = nullptr;
	this->selectedType = ' ';
}


void* TableEditor::getSelected() {
	return this->selected;
}


void TableEditor::insertRow() {
	// creating a new row
	Rows* newRows = new Rows();
	Row* newRow = new Row();
	string val;
	Column* col = this->columns->getColumn();

	for (int i = 0; i < this->columns->getNumOfCols(); i++) {
		cout << "Upisati vrednost celije " << col->name << ": ";
		cin >> val;
		newRow->addCell(val);
		col = col->next;
	}
	newRows->row = newRow;

	// adding new row to table
	if (this->selectedType == 'R') {
		Rows* selectedRow = (Rows*)this->selected;
		// find row before selected row
		Rows* previousRow = new Rows();

		for (Rows* t = this->firstRow; t != nullptr; t = t->next) {
			if (t->next == selectedRow) {
				previousRow = t;
				break;
			}
		}
		// ne bi trebalo da bude greska
		previousRow->next = newRows;
		newRows->next = selectedRow;
	}
	// ako nemamo selekciju
	else {
		this->lastRow->next = newRows;
		newRows->next = nullptr;
		this->lastRow = newRows;
	}
	this->numRows++;
}



void TableEditor::insertColumn(string col_name, Type type) {
	char t = ' ';
	string data;

	switch (type) {
	case 0:
		t = 'i';
		data = '0';
		break;
	case 1:
		t = 'd';
		data = '0';
		break;
	case 2:
		t = 't';
		data = "";
		break;
	}


	if (this->selectedType == 'C') {
		cout << "Hello" << endl;
	}
	else {
		Elem* lastEl = new Elem();

		this->columns->addColumn(col_name, t);
		for (Rows* temp = this->firstRow; temp != nullptr; temp = temp->next) {
			Cell *newCell = new Cell();
			for (Elem* t = temp->row->getRow(); t != nullptr; t = t->next) {
				lastEl = t;
			}
			newCell->setData(data);
			lastEl->cell = newCell;
			lastEl->next = nullptr;
		}
	}

}





// debugging
void TableEditor::printSelected() {
	string data;

	Cell* cell = (Cell*)this->selected;
	data = cell->getData();

	cout << data << endl;

}