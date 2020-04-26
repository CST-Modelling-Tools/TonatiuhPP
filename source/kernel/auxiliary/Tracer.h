#pragma once
#include <string>
#include <iostream>

class Tracer
{
public:
    Tracer( std::string traceString = "too lazy, eh?");
    ~Tracer(); // destructor
private:
    std::string m_traceString;
};
