#include "Tracer.h"

Tracer::Tracer( std::string traceString )
: m_traceString(traceString)
{
    std::cout << "+ " << m_traceString << std::endl;
}

Tracer::~Tracer()
{
    std::cout << "- " << m_traceString << std::endl;
}
