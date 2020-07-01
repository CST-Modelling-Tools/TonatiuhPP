#pragma once

#include "libraries/TonatiuhLibraries.h"

#include <QStringList>

#include <Inventor/fields/SoMField.h>
#include <Inventor/fields/SoSubField.h>


class FieldEditor;

class TONATIUH_LIBRARIES UserMField: public SoMField
{
    typedef SoMField inherited;

public:
    static void initClass(void);
    virtual ~UserMField() {}

    static SoType getClassTypeId(void) {return classTypeId;}
    static void atexit_cleanup(void);
    virtual FieldEditor* getEditor() const = 0;

    QStringList getNames() const {return m_names;}
    void setNames(QStringList names) {m_names = names;}

private:
    QStringList m_names;
    FieldEditor* m_editor;
    static SoType classTypeId;
};
