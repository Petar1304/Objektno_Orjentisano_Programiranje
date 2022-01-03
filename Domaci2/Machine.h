#ifndef MACHINE_H
#define MACHINE_H

#include "Commands.h"
#include "Exceptions.h"
#include <fstream>
#include <iostream>
#include <map>
#include <string>
#include <vector>

typedef std::string String;

class Machine {
public:
	Machine() = default;
	~Machine();
	// loads Program from filepath
	void  loadProgram(const String& filepath);
	// excecutes the program and saves variables to filepath
	void execute(const String& filepath);

private:
	std::vector<Command*> commands;

	// private methods
	Command* findCommand(int lineNum, const String& cmd);
	bool checkIfChar(const String& str, int pos) const;
	int loadInt(const String& str, int& pos);
	int findOperand(const String& line, int& pos);
	Math* getMathObj(int lineNum, const String& line, int& pos, char operation);
	If* getIfObj(int lineNum, const String& line, int& pos, char operation);
	void run();
};


#endif // MACHINE_H