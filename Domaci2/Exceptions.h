#ifndef EXCEPTIONS_H
#define EXCEPTIONS_H

#include <exception>

class CompilingError : public std::exception {
public:
	CompilingError(const char* msg) : std::exception(msg) {}
};


class RunTimeError : public std::exception {
public:
	RunTimeError(const char* msg) : std::exception(msg) {}
};

// input output error
class IOError : public std::exception {
public:
	IOError() : std::exception("ERROR >> Input Output file error: ") {}
};

#endif // !EXCEPTIONS_H