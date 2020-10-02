#pragma once

#include <QObject>

class KeyFilter : public QObject
{
    Q_OBJECT
public:
    KeyFilter(QObject* par);

protected:
    bool eventFilter(QObject* object, QEvent* event);
};

