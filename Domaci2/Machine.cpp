#include "Machine.h"

// destructor
Machine::~Machine() {
	for (auto cmd : commands) {
		delete cmd;
	}
	Command::resetVariables();
}


void Machine::loadProgram(const std::string& filepath) {
	std::ifstream input(filepath); // filepath ima .txt na kraju

	String line;

	int lineNum = 1;
	try {
		if (input.is_open()) {
			// load line by line
			while (std::getline(input, line)) {
				try {
					Command* newCmd = findCommand(lineNum, line);
					commands.push_back(newCmd);
					lineNum++;
				}
				catch (CompilingError& e) {
					std::cout << e.what() << "Line: " << lineNum << std::endl;
				}
			}
		}

		else {
			// trow exception for closed file
			throw IOError();
		}
	}
	catch (IOError& e) {
		std::cout << e.what() << std::endl;
	}
	
}


// finds command from line and returns pointer to a command object
Command* Machine::findCommand(int lineNum, const String& line) {
	String cmd;
	int pos = 0;
	while (line[pos] != ' ' && line[pos] != '\n' && line[pos] != '\0') {
		cmd.push_back(line[pos]);
		pos++;
	}

	if (cmd == "SET") {
		char opr1 = line[++pos];
		pos += 2;
		if (checkIfChar(line, pos)) {
			char opr2 = line[pos];
			return new Set(lineNum, opr1, opr2);
		}
		else {
			int opr2 = loadInt(line, pos);
			return new Set(lineNum, opr1, opr2);
		}
	}

	// MATH
	else if (cmd == "ADD") {
		return getMathObj(lineNum, line, pos, '+');
	}
	else if (cmd == "SUB") {
		return getMathObj(lineNum, line, pos, '-');
	}
	else if (cmd == "MUL") {
		return getMathObj(lineNum, line, pos, '*');
	}
	else if (cmd == "DIV") {
		return getMathObj(lineNum, line, pos, '/');
	}
	
	// GOTO
	else if (cmd == "GOTO") {
		int num = loadInt(line, ++pos);
		if (num == 0) throw CompilingError("ERROR >> GOTO operand equal to zero exception: ");
		return new Goto(lineNum, num);
	}

	// IFEQ / IFGR ELSE ENDIF
	else if (cmd == "IFGR") {
		return getIfObj(lineNum, line, pos, '>');
	}
	else if (cmd == "IFEQ") {
		return getIfObj(lineNum, line, pos, '=');
	}
	else if (cmd == "ELSE") {
		return new If(lineNum, "ELSE");
	}
	else if (cmd == "ENDIF") {
		return new If(lineNum, "ENDIF");
	}

	// LOOP ENDLOOP
	else if (cmd == "LOOP") {
		int num = 0;
		if (pos < line.size()) {
			num = loadInt(line, ++pos);
		}
		if (num < 0) throw CompilingError("ERROR >> LOOP operand equal to 0: ");
		return new Loop(lineNum, num);	
	}
	else if (cmd == "ENDLOOP") {
		return new Loop(lineNum, "ENDLOOP");
	}
}


// returns if object
If* Machine::getIfObj(int lineNum, const String& line, int&pos, char operation) {
	char opr1c = '0';
	char opr2c = '0';
	int opr1i = 0;
	int opr2i = 0;
	pos++;
	if (checkIfChar(line, pos)) {
		opr1c = line[pos];
		pos += 2;
	}
	else {
		opr2i = loadInt(line, pos);
		pos += 1;
	}
	if (checkIfChar(line, pos)) {
		opr2c = line[pos];
		pos += 2;
	}
	else {
		opr2i = loadInt(line, pos);
		pos += 1;
	}
	if (opr1c == '0') {
		if (opr2c == '0') {
			return new If(lineNum, operation, opr1i, opr2i);
		}
		else {
			return new If(lineNum, operation, opr1i, opr2c);
		}
	}
	else if (opr2c == '0') {
		if (opr1c != '0') {
			return new If(lineNum, operation, opr1c, opr2i);
		}
	}
	else {
		return new If(lineNum, operation, opr1c, opr2c);
	}
}


// returns math object
Math* Machine::getMathObj(int lineNum, const String& line, int& pos, char operation) {
	
	char opr2c = '0';
	char opr3c = '0';
	int opr2i = 0;
	int opr3i = 0;

	char opr1 = line[++pos];
	pos += 2;
	if (checkIfChar(line, pos)) {
		opr2c = line[pos];
		pos += 2;
	}
	else {
		opr2i = loadInt(line, pos);
		pos += 1;
	}

	if (checkIfChar(line, pos)) {
		opr3c = line[pos];
	}
	else {
		opr3i = loadInt(line, pos);
	}

	if (opr2c == '0') {
		if (opr3c == '0') {
			return new Math(lineNum, operation, opr1, opr2i, opr3i);
		}
		else {
			return new Math(lineNum, operation, opr1, opr2i, opr3c);
		}
	}
	else if (opr3c == '0') {
		if (opr2c != '0') {
			return new Math(lineNum, operation, opr1, opr2c, opr3i);
		}
	}
	else {
		return new Math(lineNum, operation, opr1, opr2c, opr3c);
	}
}


// Checking if line[pos] is char or int
bool Machine::checkIfChar(const String& str, int pos) const {
	if (str[pos] >= 65 && str[pos] <= 90) {
		return true;
	}
	return false;
}


// returns one int from line
int Machine::loadInt(const String& line, int& pos) {
	String num;
	while (line[pos] != ' ' && line[pos] != '\n' && line[pos] != '\0') {
		num.push_back(line[pos]);
		pos++;
		if (pos >= line.size()) {
			break;
		}
	}
	// converts string to int
	try {
		return stoi(num);
	}
	catch (std::exception& e) {
		std::cout << "ERROR >> Conversion from string to int failed: " << std::endl;
	}
}


// executes program and writes output to file
void Machine::execute(const std::string& filepath) {

	Machine::run();
	// writes to a file
	String out = Command::getVariables();
	std::ofstream output(filepath);
	if (output.is_open()) {
		output << out;
	}
}


void Machine::run() {

	for (int i = 0; i < this->commands.size(); i++) {
		try {
			this->commands[i]->run(i, commands);
		}
		catch (RunTimeError& e) {
			std::cout << e.what() << "Line: " << this->commands[i]->getLine() << std::endl;
		}
	}

}
