#include "Commands.h"

// STATIC VARIABLE DEFINITION
 std::map <char, int> Command::variables;


// COMMAND
Command::Command(int line) : line(line) {}

// resets static field
void Command::resetVariables() {
	Command::variables.clear();
}

// returns command name
String Command::getName() {
	return this->name;
}

// returns line number
int Command::getLine() const {
	return this->line;
}

// returns variables for printing
String Command::getVariables() {
	String out;
	for (auto [key, val] : Command::variables) {
		out.push_back(key);
		out.push_back('=');
		out.append(std::to_string(val));
		out.push_back('\n');
	}
	// removing new line at the end
	out.pop_back();
	return out;
}

// returns variable value
int Command::findValue(char var) {
	return Command::variables[var];
}

// returns command index in commands vector
int Command::findCommandIndex(int start, std::vector<Command*>& commands, const String commandName) {
	int i = start;
	while (commands[i]->getName() != commandName) {
		i++;
	}
	return i;
}



// SET
Set::Set(int line, char var, int val) : Command(line), var(var), val1(val) {
	this->var1 = '0'; // null
}

Set::Set(int line, char var1, char var2) : Command(line), var(var1), var1(var2) {
	this->val1 = 0;
}

Set::Set(const Set& set) : Command(set.getLine()) {
	this->val1 = set.val1;
	this->var = set.var;
	this->var1 = set.var1;
}


String Set::getName() {
	return "SET";
}

void Set::run(int& i, std::vector<Command*>& commands) {
	if (this->var1 == '0') {
		Command::variables[this->var] = this->val1;
	}
	else {
		Command::variables[this->var] = Command::variables[this->var1];
	}
}




// MATH
Math::Math(int line, char operation, char var, int val1, int val2) : Command(line), operation(operation), var(var), val1(val1), val2(val2) {
	this->var1 = '0';
	this->var2 = '0';
}

Math::Math(int line, char operation, char var, char val1, int val2) : Command(line), operation(operation), var(var), var1(val1), val2(val2) {
	this->val1 = 0;
	this->var2 = '0';
}

Math::Math(int line, char operation, char var, int val1, char val2) : Command(line), operation(operation), var(var), val1(val1), var2(val2) {
	this->var1 = '0';
	this->val2 = 0;
}

Math::Math(int line, char operation, char var, char val1, char val2) : Command(line), operation(operation), var(var), var1(val1), var2(val2) {
	this->val1 = 0;
	this->val2 = 0;
}


String Math::getName() {
	return "MATH";
}


// running math operations
void Math::run(int& i, std::vector<Command*>& commands) {
	if (this->var1 != '0') {
		this->val1 = variables[this->var1];
	}
	if (this->var2 != '0') {
		this->val2 = variables[this->var2];
	}

	switch (this->operation) {
	case '+':
		Command::variables[this->var] = this->val1 + this->val2;
		break;
	case '-':
		Command::variables[this->var] = this->val1 - this->val2;
		break;
	case '*':
		Command::variables[this->var] = this->val1 * this->val2;
		break;
	case '/':
		if (this->val2 == 0) {
			// throw Dividing by zero exception
			throw RunTimeError("ERROR >> Dividing by zero: ");
		}
		Command::variables[this->var] = this->val1 / this->val2;
		break;
	}
}



// GOTO
Goto::Goto(int line, int num) : Command(line), num(num) {}

Goto::Goto(const Goto& g) : Command(g.getLine()) {
	this->num = g.num;
}

int Goto::getNum() {
	return this->num;
}

String Goto::getName() {
	return "GOTO";
}

void Goto::run(int& i, std::vector<Command*>& commands) {
	i += this->num;
}



// IFEQ IFGR ELSE ENDIF
If::If(int line, char opr, int val1, int val2) : Command(line), opr(opr), val1(val1), val2(val2) {
	this->name = "IF";
	this->var1 = '0';
	this->var2 = '0';
}
If::If(int line, char opr, int val1, char var2) : Command(line), opr(opr), val1(val1), var2(var2) {
	this->name = "IF";
	this->var1 = '0';
	this->val2 = 0;
}
If::If(int line, char opr, char var1, int val2) : Command(line), opr(opr), var1(var1), val2(val2) {
	this->name = "IF";
	this->val1 = 0;
	this->var2 = '0';
}
If::If(int line, char opr, char var1, char var2) : Command(line), opr(opr), var1(var1), var2(var2) {
	this->name = "IF";
	this->val1 = 0;
	this->val2 = 0;
}
If::If(int line, String name) : Command(line), name(name) {
	this->opr = '0';
	this->val1 = this->val2 = this->var1 = this->var2 = 0;
}


// copy constructor
If::If(const If& i) : Command(i.getLine()) {
	this->name = i.name;
	this->opr = i.opr;
	this->val1 = i.val1;
	this->val2 = i.val2;
	this->var1 = i.var1;
	this->var2 = i.var2;
}


String If::getName() {
	return this->name;
}


// checks if ifgr/igeq command is true or false
bool If::isTrue() {
	if (this->var1 != '0') {
		this->val1 = variables[this->var1];
	}
	if (this->var2 != '0') {
		this->val2 = variables[this->var2];
	}
	if (this->opr == '=') {
		if (this->val1 == this->val2) return true;
		else return false;
	}
	else if (this->opr == '>') {
		if (this->val1 > this->val2) return true;
		else return false;
	}
}



void If::run(int& i, std::vector<Command*>& commands) {
	if (commands[i]->getName() == "ELSE" || commands[i]->getName() == "ENDIF") return;

	If* cmd = (If*)commands[i];

	int ifPos = i;
	int elsePos = findCommandIndex(ifPos, commands, "ELSE");
	int endifPos = findCommandIndex(elsePos, commands, "ENDIF");
	
	i++; // skippig if
	if (cmd->isTrue()) {
		while (i < elsePos) {
			commands[i]->run(i, commands);
			if (i > endifPos || i < ifPos) { return; }
			i++;
		}
	}
	else {
		i = elsePos + 1; // move to else
		while (i < endifPos) {
			commands[i]->run(i, commands);
			if (i > endifPos || i < elsePos) { return; }
			i++;
		}
	}
	// i = endifPos + 1; // skipping endif
	i = endifPos;
}



// LOOP ENDLOOP
Loop::Loop(int line, int num) : Command(line), num(num) {
	this->name = "LOOP";
}

Loop::Loop(int line, String name) : Command(line), name(name) {
	this->num = 0;
}

// copy constructor
Loop::Loop(const Loop& l) : Command(l.getLine()) {
	this->name = l.name;
	this->num = l.num;
}

String Loop::getName() {
	return this->name;
}

int Loop::getNum() {
	return this->num;
}


void Loop::run(int& i, std::vector<Command*>& commands) {
	if (commands[i]->getName() == "ENDLOOP") { return; }

	int start = i + 1;
	int end = findCommandIndex(start, commands, "ENDLOOP");

	Loop* cmd = (Loop*)commands[i];
	int n = cmd->getNum();

	if (n == 0) { // infinite loop
		while (1) {
			i = start;
			while (i < end) {
				commands[i]->run(i, commands);
				// jumping out of loop
				if (i > end) { i--;  return; } // because it will be incremented again in another loop	
				i++;
			}
		}
	}
	else { // loop n-times
		for (int j = 0; j < n; j++) {
			i = start;
			while (i < end) {
				commands[i]->run(i, commands);
				// jumping out of loop
				if (i > end) { i--; return; }
				i++;
			}
		}
	}
	// skippig the loop and endloop command
	i = end;
}
