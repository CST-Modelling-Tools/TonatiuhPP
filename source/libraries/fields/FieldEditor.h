#pragma once

#include "libraries/TonatiuhLibraries.h"
#include <QWidget>


class TONATIUH_LIBRARIES FieldEditor: public QWidget
{
    Q_OBJECT

public:
    FieldEditor(QWidget* parent = 0):
        QWidget(parent) {}
    virtual QString GetData() const = 0;
    virtual void SetData(QString data) = 0;
};
