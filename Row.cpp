#include "Row.h"
#include "Columns.h"
#include "Cell.h"
#include <iostream>
#include <string>
using namespace std;

// Constructor
Row::Row() {
	this->head = nullptr;
	this->last = nullptr;
	this->numOfElem = 0;
}

// Destructor
Row::~Row() {
	while (this->head != nullptr) {
		Elem* old = this->head;
		this->head = this->head->next;
		delete old->cell;
		delete old;

	}
	this->head = this->last = nullptr;
	this->numOfElem = 0;
}


Elem* Row::getRow() {
	return this->head;
}

Cell* Row::getCell() {
	return this->head->cell;
}

void Row::setRowHead(Elem* head) {
	this->head = head;
}


// Add Cell in Row
void Row::addCell(string data) {

	Cell* cell = new Cell();
	cell->setData(data);

	Elem* element = new Elem();
	element->cell = cell;
	element->next = nullptr;

	if (!this->head) {
		this->head = element;
	}
	else {
		this->last->next = element;	
	}
	this->last = element;
	this->numOfElem++;
}



void Row::deleteCell(int numOfColumn) {
	// Elem* del = new Elem();
	Elem* del = nullptr;
	Elem* last = this->head;
	int counter = 0;

	for (Elem* temp = this->head; temp != nullptr; temp = temp->next) {
		if (counter == numOfColumn) {
			del = temp;
			last->next = temp->next;
			break;
		}
		counter++;
		last = temp;
	}

	// delete del->cell;
	delete del;
	delete last;

	this->numOfElem--;
}

Elem* Row::getLastRow() {
	return this->last;
}


string Row::getColumnValue(int colNum) {
	string val="";

	Elem* temp = this->head;
	for (int i = 0; i < colNum; i++) {
		temp = temp->next;
	}
	val = temp->cell->getData();

	return val;
}