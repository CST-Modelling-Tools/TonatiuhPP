#pragma once

#include <QMetaType>

class SoPath;


class PathWrapper
{
public:
    PathWrapper(SoPath* path = 0);
    SoPath* GetPath() const {return m_path;}
    
private:
    SoPath* m_path;
};


Q_DECLARE_METATYPE(PathWrapper)
