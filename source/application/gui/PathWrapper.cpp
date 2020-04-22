#include "PathWrapper.h"
#include <Inventor/SoPath.h>

PathWrapper::PathWrapper( SoPath* path )
: m_path( path )
{
}

PathWrapper::PathWrapper( const PathWrapper& rhs )
{
    m_path = rhs.m_path;    
}

PathWrapper::~PathWrapper()
{
}

SoPath* PathWrapper::GetPath() const
{
    return m_path;
}
