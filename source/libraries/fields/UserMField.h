#pragma once

#include <QStringList>

#include <Inventor/fields/SoSubField.h>
#include <Inventor/fields/SoMField.h>

class FieldEditor;

class UserMField: public SoMField
{
    typedef SoMField inherited;

public:
    virtual ~UserMField() {}
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
