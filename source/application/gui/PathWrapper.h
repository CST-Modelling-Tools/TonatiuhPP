#pragma once

#include <QMetaType>

class SoPath;

class PathWrapper
{
public:
    PathWrapper( SoPath* path = 0 );
    PathWrapper( const PathWrapper& rhs );
    ~PathWrapper();
    SoPath* GetPath() const;
    
private:
    SoPath* m_path;
};

Q_DECLARE_METATYPE(PathWrapper)
