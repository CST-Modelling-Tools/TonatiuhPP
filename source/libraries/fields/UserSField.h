#pragma once

#include "libraries/TonatiuhLibraries.h"
#include <QStringList>
#include <Inventor/fields/SoSubField.h>
#include <Inventor/fields/SoSField.h>


class FieldEditor;

class TONATIUH_LIBRARIES UserSField: public SoSField
{
    typedef SoSField inherited;

public:
    virtual ~UserSField() {}
    static void initClass(void);
    static SoType getClassTypeId(void);
    static void atexit_cleanup(void);

    virtual FieldEditor* GetEditor() const = 0;

    QStringList GetNames() const
    {
        return m_dimensionsNames;
    }

    void SetNames(QStringList dimensionsNames)
    {
        m_dimensionsNames = dimensionsNames;
    }

private:
    QStringList m_dimensionsNames;
    FieldEditor* m_pEditor;
    static SoType classTypeId;
};
