#ifndef COMMANDS_H
#define COMMANDS_H

#include "Exceptions.h"
#include <iostream>
#include <map>
#include <string>
#include <vector>

typedef std::string String;

// enum class CMD {
//	SET, ADD, SUB, MUL, DIV, IFEQ, IFGR, ELSE, ENDIF, LOOP, ENDLOOP
// };


class Command {
public:
	// constructor
	Command(int line);
	// default destructor
	~Command() = default;
	// returns line number
	int getLine() const;
	// returns variables for printing
	static String getVariables();
	// returns variable value
	static int findValue(char var);
	// reset static field variables
	static void resetVariables();
	// returns command name
	virtual String getName();
	// returns index of command from commands vector
	int findCommandIndex(int start, std::vector<Command*>& commands, const String commandName);
	// runs command
	virtual void run(int& i, std::vector<Command*>& commands) = 0;

protected:
	static std::map<char, int> variables;

private:
	int line; // command line number
	String name;
};


// SET
class Set : public Command {
public:
	Set(int line, char var, int val);
	Set(int line, char var1, char var2);
	~Set() = default;
	// copy constructor
	Set(const Set& set);
	String getName() override;
	void run(int& i, std::vector<Command*>& commands) override;

private:
	char var;
	int val1;
	char var1;

};


// MATH OPERATIONS
class Math : public Command {
public:
	// constructors
	Math(int line, char operation, char var, int val1, int val2);
	Math(int line, char operation, char var, char val1, int val2);
	Math(int line, char operation, char var, int val1, char val2);
	Math(int line, char operation, char var, char val1, char val2);
	~Math() = default;
	String getName() override;
	void run(int& i, std::vector<Command*>& commands) override;

private:
	char operation;
	char var;
	int val1;
	int val2;
	char var1;
	char var2;

};
 

// GOTO
class Goto : public Command {
public:
	Goto(int line, int num);
	Goto(const Goto& g);
	~Goto() = default;
	String getName() override;
	int getNum();
	void run(int& i, std::vector<Command*>& commands) override;

private:
	int num;
};



// IFEQ IFGR ELSE ENDIF
class If : public Command {
public:
	If(int line, char opr, int val1, int val2);
	If(int line, char opr, int val1, char var2);
	If(int line, char opr, char var1, int val2);
	If(int line, char opr, char var1, char var2);
	// constuctor for else and endif
	If(int line, String name);
	// copy constructor
	If(const If& i);
	~If() = default;
	bool isTrue();
	String getName() override;
	void run(int& i, std::vector<Command*>& commands) override;

private:
	String name;
	char opr;
	int val1;
	int val2;
	char var1;
	char var2;
};



// LOOP
class Loop : public Command {
public:
	Loop(int line,  int num);
	// constructor for endloop
	Loop(int line, String name);
	Loop(const Loop& l);
	~Loop() = default;
	String getName() override;
	int getNum();
	void run(int& i, std::vector<Command*>& commands) override;

private:
	String name;
	int num;

};

#endif // COMMANDS_H