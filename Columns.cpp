#include <iostream>
#include <string>
#include "Cell.h"
#include "Columns.h"
using namespace std;

// Constructor
Columns::Columns() {
	this->head = nullptr;
	this->last = nullptr;
	this->numOfCols = 0;
}

// Destructor
Columns::~Columns() {
	while (this->head != nullptr) {
		Column* old = this->head;
		this->head = this->head->next;
		delete old;
	}
	this->head = this->last = nullptr;
	this->numOfCols= 0;
}

// Add new cell
void Columns::addColumn(string name, char type) {
	Column* element = new Column();
	element->type = type;
	element->name = name;
	element->next = nullptr;

	if (!this->head) {
		this->head = element;
	}
	else {
		this->last->next = element;
	}
	this->last = element;
	this->numOfCols++;
}


// Delete Column
void Columns::deleteColumn(string name) {
	Column* del = nullptr;
	Column* last = this->head;

	for (Column* temp = this->head; temp->next != nullptr; temp = temp->next) {
		if (temp->name == name) {
			del = temp;
			last->next = temp->next;
			break;
		}
		last = temp;
	}
	delete del;
	delete last;
	this->numOfCols--;
}

void Columns::addNumOfCols() {
	this->numOfCols++;
}

void Columns::subNumOfCols() {
	this->numOfCols--;
}

void Columns::setColumnHead(Column* head) {
	this->head = head;
}


string Columns::printColumn() {
	string data;
	int i = 0;

	for (Column* temp = this->head; temp != nullptr; temp = temp->next) {
		data.push_back(temp->type);// convert char to string
		data.push_back(':');
		data.append(temp->name);
		if (i < this->numOfCols - 1) {
			data.push_back(',');
		}
		i++;
	}
	data.push_back('\n');
	return data;
}


Column* Columns::getColumn() {
	return this->head;
}


int Columns::getNumOfCols() {
	return this->numOfCols;
}


int Columns::findColNum(string col_name) {
	int i = 0;
	for (Column* temp = this->head; temp != nullptr; temp = temp->next) {
		if (temp->name == col_name) {
			break;
		}
		i++;
	}

	return i;
}


bool Columns::isColNumType(string col_name) {
	char t = ' ';
	for (Column* temp = this->head; temp != nullptr; temp = temp->next) {
		if (temp->name == col_name) {
			t = temp->type;
			break;
		}
	}
	if (t == 'i' || t == 'd') {
		return true;
	}
	else return false;
}