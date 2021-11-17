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
	this->selectedCellType = ' ';
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
	this->selectedCellType = ' ';
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
			this->selectedCellType = t->type;
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
		case Type::INT:
			t = 'i';
			data = "0";
			break;
		case Type::DECIMAL:
			t = 'd';
			data = "0";
			break;
		case Type::TEXT:
			t = 't';
			data = "";
			break;
	}

	// Dodavanje kolone pre selektrovane kolone
	if (this->selectedType == 'C') {

		Column* selectedCol = (Column*)this->selected;
		// find column before selected column
		Column* previousCol = new Column();
		int colNum = 0;

		// ne radi
		// ako nije prva kolona
		if (selectedCol != this->columns->getColumn()) {

			for (Column* tempCol = this->columns->getColumn(); tempCol != nullptr; tempCol = tempCol->next) {
				if (tempCol->next == selectedCol) {
					previousCol = tempCol;
					break;
				}
				// maybe change
				colNum++;
			}

			Column* newCol = new Column();
			newCol->name = col_name;
			newCol->type = t;
			// smestanje u listu kolona
			previousCol->next = newCol;
			newCol->next = selectedCol;
			this->columns->addNumOfCols();

			// popunjavanje svih celija
			for (Rows* temp = this->firstRow; temp != nullptr; temp = temp->next) {
				Elem* current = temp->row->getRow();
				Elem* last = new Elem();
				Elem* newEl = new Elem();
				Cell* newCell = new Cell();
				newCell->setData(data);
				newEl->cell = newCell;

				for (int i = 0; i <= colNum; i++) {
					last = current;
					current = current->next;
				}

				last->next = newEl;
				newEl->next = current;
			}
		}
		// radi
		else {
			// set new first column
			Column* newCol = new Column();
			newCol->name = col_name;
			newCol->type = t;
			newCol->next = this->columns->getColumn();
			this->columns->setColumnHead(newCol);
			this->columns->addNumOfCols();
			
			// set new first rows
			for (Rows* temp = this->firstRow; temp != nullptr; temp = temp->next) {
				Elem* tempEl = temp->row->getRow();
				Elem* newEl = new Elem();
				newEl->cell = new Cell();
				newEl->cell->setData(data);
				newEl->next = tempEl;
				temp->row->setRowHead(newEl);
			}
		}
	}

	// Dodavanje nove celije na kraj svakog reda
	else {
		this->columns->addColumn(col_name, t);
		for (Rows* temp = this->firstRow; temp != nullptr; temp = temp->next) {

			Elem* lastEl = new Elem();
			Elem* newEl = new Elem();
			Cell *newCell = new Cell();
			newCell->setData(data);

			// go to the last element of list
			lastEl = temp->row->getLastRow();
			newEl->cell = newCell;
			newEl->next = nullptr;
			lastEl->next = newEl;
		}
	}
}



// debugging
void TableEditor::printSelected() {
	Cell* cell = (Cell*)this->selected;
	cout << cell->getData() << endl;
}



// Menja sadrzaj selektovane celije
void TableEditor::editCellValue() {
	// radi samo ako je selektovana celija
	string val;
	if (this->selectedType == 'c') {
		Cell* cell = (Cell*)this->selected;
		cout << "Upisati novu vrednost celije: ";
		cin >> val;
		cell->setData(val);
	}
}


// Radi
void TableEditor::deleteContent() {
	// ako je selektovana celija -> brisemo njenu vrednost
	if (this->selectedType == 'c') {
		Cell* el = (Cell*)this->selected;
		el->setData("");
	}



	// Ako je selektovan red -> brisemo taj red 
	else if (this->selectedType == 'R') {
		Rows* row = (Rows*)this->selected;
		Rows* previousRow = new Rows(); // change maybe

		if (row != this->firstRow) {
			for (Rows* temp = this->firstRow; temp != nullptr; temp = temp->next) {
				if (temp == row) {
					break;
				}
				previousRow = temp;
			}
			previousRow->next = row->next;
		}
		// ako brisemo prvi red
		else {
			firstRow = row->next;
		}

		this->numRows--;
		// ciscenje memorije
		delete row->row;
		delete row;
	}

	// Ako je selektovana kolona -> brisemo tu kolonu
	else if (this->selectedType == 'C') {
		int colNum = 0;
		Column* col = (Column*)this->selected;
		Column* previous = new Column();
		// ako nije prva kolona
		if (col != this->columns->getColumn()) {
			for (Column* temp = this->columns->getColumn(); temp != nullptr; temp = temp->next) {
				if (temp == col) {
					break;
				}
				previous = temp;
				colNum++;
			}
			previous->next = col->next;

			// Radi
			// brisanje kolone iz svih redova
			for (Rows* tempRow = this->firstRow; tempRow != nullptr; tempRow = tempRow->next) {
				// doci do kolone koja treba da se obrise
				Elem* tempEl = tempRow->row->getRow();
				Elem* previousEl = tempEl;
				for (int i = 0; i < colNum; i++) {
					previousEl = tempEl;
					tempEl = tempEl->next;
				}
				previousEl->next = tempEl->next;

				delete tempEl->cell;
				delete tempEl;
			}
		}
		// RADI
		// ako brisemo prvu kolonu
		else {
			this->columns->setColumnHead(col->next);
			// brisanje prve celije u svakom redu
			for (Rows* tempRow = this->firstRow; tempRow != nullptr; tempRow = tempRow->next) {
				// menjamo pointer prve celije u redu na drugu
				Elem* firstEl = tempRow->row->getRow();
				Elem* nextEl = firstEl->next;
				tempRow->row->setRowHead(nextEl);
				delete firstEl;
			}
		}
		this->columns->subNumOfCols();
		delete col;
	}
}


// dodavanje vrednosti selektrovane celije ili kolone vrednost val
void TableEditor::add(double value) {
	// za celiju
	if (this->selectedType == 'c') {
		Cell* cell = (Cell*)this->selected;
		if (this->selectedCellType == 'i' || this->selectedCellType == 'd') {
			cell->addVal(value);
		}
		else {
			cout << "Selektovana celija nema numericki sadrzaj..." << endl;
		}
	}
	// za selektrovanu kolonu
	else if (this->selectedType == 'C') {
		// find number of selected column
		if (((Column*)this->selected)->type == 'i' || ((Column*)this->selected)->type == 'd') {
			string col_name = ((Column*)this->selected)->name;
			int colNum = this->columns->findColNum(col_name);

			for (Rows* tempRow = this->firstRow; tempRow != nullptr; tempRow = tempRow->next) {
				Elem* tempCell = tempRow->row->getRow();
				// dolazenje do selektovane kolone u svakom redu
				for (int i = 0; i < colNum; i++) {
					tempCell = tempCell->next;
				}
				tempCell->cell->addVal(value);
			}
		}
		else {
			cout << "Selektrovana kolona nije numerickog tipa..." << endl;
		}
	}
	else {
		cout << "Operacija neuspesne(ili nista nije selektrovano ili je selektrovan red)" << endl;
	}
}


// mnozenje vrednosti selektrovane celije ili kolone vrednost val
void TableEditor::mul(double value) {
	// za celiju
	if (this->selectedType == 'c') {
		Cell* cell = (Cell*)this->selected;
		if (this->selectedCellType == 'i' || this->selectedCellType == 'd') {
			cell->mulVal(value);
		}
		else {
			cout << "Selektovana celija nema numericki sadrzaj..." << endl;
		}
	}
	// za selektrovanu kolonu
	else if (this->selectedType == 'C') {
		// find number of selected column
		if (((Column*)this->selected)->type == 'i' || ((Column*)this->selected)->type == 'd') {
			string col_name = ((Column*)this->selected)->name;
			int colNum = this->columns->findColNum(col_name);

			for (Rows* tempRow = this->firstRow; tempRow != nullptr; tempRow = tempRow->next) {
				Elem* tempCell = tempRow->row->getRow();
				// dolazenje do selektovane kolone u svakom redu
				for (int i = 0; i < colNum; i++) {
					tempCell = tempCell->next;
				}
				tempCell->cell->mulVal(value);
			}
		}
		else {
			cout << "Selektrovana kolona nije numerickog tipa..." << endl;
		}
	}
	else {
		cout << "Operacija neuspesne(ili nista nije selektrovano ili je selektrovan red)" << endl;
	}
}





// sortirati sve kao string, dobice se tacni rezultati
void TableEditor::sortByValue(string col_name, bool asc) {
	// asc = true (>)
	// asc = false (<)

	// bubble sort 
		int colNum = this->columns->findColNum(col_name);
		bool numeric = this->columns->isColNumType(col_name);

		Rows* current = this->firstRow;

		string val1, val2;

		int swapped = 0;
		
		while (current) {
			for (Rows* tempRow = current->next; tempRow != nullptr; tempRow = tempRow->next) {
				// vrednosti po kojima se sortira
				val1 = current->row->getColumnValue(colNum);
				val2 = tempRow->row->getColumnValue(colNum);
				
				if (compareVals(val1, val2, numeric, asc)) {
					Row* temp = current->row;
					current->row = tempRow->row;
					tempRow->row = temp;

					swapped = 1;
				}
			}
			current = current->next;
			if (swapped == 0) break;
		}
}



bool TableEditor::compareVals(string v1, string v2, bool numeric,bool asc) {
	double val1, val2;
	if (numeric) {
		if (v1 == "") {
			val1 = 0;
		}
		else {
			val1 = stod(v1);
		}
		if (v2 == "") {
			val2 = 0;
		}
		else {
			val2 = stod(v2);
		}
		if (asc == true) {
			if (val1 > val2) {
				return true;
			}
			else return false;
		}
		else {
			if (val1 < val2) {
				return true;
			}
			else return false;
		}
	}
	else {
		if (asc == true) {
			if (v1 > v2) {
				return true;
			}
			else return false;
		}
		else {
			if (v1 < v2) {
				return true;
			}
			else return false;
		}
	}
}




// prvo pojavljivanje value u koloni
int TableEditor::findFirstOf(string value, string col_name) {
	int rowNum = 0;
	int colNum = this->columns->findColNum(col_name);
	for (Rows* temp = this->firstRow; temp != nullptr; temp = temp->next) {
		if (temp->row->getColumnValue(colNum) == value) {
			return rowNum;
		}
		rowNum++;
	}
	// trebalo bi da je dovoljno
	if (rowNum) return -2;
}


// Zadnje pojavljivanje value u koloni
int TableEditor::findLastOf(string value, string col_name) {
	int lastRowNum = 0;
	int rowNum = 0;
	int colNum = this->columns->findColNum(col_name);
	for (Rows* temp = this->firstRow; temp != nullptr; temp = temp->next) {
		if (temp->row->getColumnValue(colNum) == value) {
			lastRowNum = rowNum;
		}
		rowNum++;
	}
	// trebalo bi da je dovoljno
	if (lastRowNum) {
		return lastRowNum;
	}
	else return -2;
}



int TableEditor::countValues(string value, string col_name) {
	int num = 0;
	int colNum = this->columns->findColNum(col_name);
	for (Rows* temp = this->firstRow; temp != nullptr; temp = temp->next) {
		if (temp->row->getColumnValue(colNum) == value) {
			num++;
		}
	}
	return num;
}


// ulancana lista za razlicite vrednosti
struct list {
	string data;
	list* next;
};

// vrace broj razlicitih vrednosti u koloni
int TableEditor::countDistinctValues(string col_name) {
	int num = 0;
	list* head = nullptr;
	list* last = head;
	string val = "";
	bool in = false;

	int colNum = this->columns->findColNum(col_name);
	for (Rows* temp = this->firstRow; temp != nullptr; temp = temp->next) {
		in = false;
		val = temp->row->getColumnValue(colNum);

		// proveravamo da li je val u listi
		for (list* t = head; t != nullptr; t = t->next) {
			if (t->data == val) {
				in = true;
			}
		}
		if (in == false) { // dodajemo ga u listu
			num++;

			list* newEl = new list();
			newEl->data = val;
			newEl->next = nullptr;
			if (!head) {
				head = newEl;
			}
			else {
				last->next = newEl;
			}
			last = newEl;
		}
	}
	return num;
}