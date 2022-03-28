#ifndef BUS_EXCEPTIONS_H
#define BUS_EXCEPTIONS_H

#include <exception>

class Error : public std::exception {
public:
    Error(const char* msg) : std::exception() {}
};

#endif //BUS_EXCEPTIONS_H
