#include "Trace.h"
#include <iostream>

int Trace::level = -1;
bool Trace::TraceEnabled = true;

Trace::Trace( std::string functionName, bool showTrace )
: m_functionName( functionName ), m_functionTrace( showTrace )
{
    if(!TraceEnabled || !m_functionTrace)
        return;

    level++;
    for( int i = 0; i < level; ++i ) std::cout << "      ";
    std::cout << "( " << level << " )" << "<ENTERING " << m_functionName << ">" << std::endl;
}

Trace::~Trace( )
{
    if(!TraceEnabled || !m_functionTrace)
        return;

    for( int i = 0; i < level; ++i ) std::cout << "      ";
    std::cout << "( " << level << " )" << "<EXITING  " << m_functionName << ">" << std::endl;
    level--;
}
