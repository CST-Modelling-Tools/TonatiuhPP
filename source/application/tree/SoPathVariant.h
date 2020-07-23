#pragma once

#include <QMetaType>

class SoPath;


class SoPathVariant
{
public:
    SoPathVariant(SoPath* path = 0):
        m_path(path) {}

    SoPath* getPath() const {return m_path;}
    
private:
    SoPath* m_path;
};


Q_DECLARE_METATYPE(SoPathVariant)
