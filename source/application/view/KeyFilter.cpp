#include "KeyFilter.h"

#include <QKeyEvent>

KeyFilter::KeyFilter(QObject* parent):
    QObject(parent)
{

}

bool KeyFilter::eventFilter(QObject* object, QEvent* event)
{
//    https://bugreports.qt.io/browse/QTBUG-46812?attachmentOrder=asc
    if (event->type() == QEvent::ShortcutOverride)
    {
        return true;
    }
//    else if (event->type() == QEvent::KeyPress || event->type() == QEvent::KeyRelease)
//    {
//        QKeyEvent *keyEvent = static_cast<QKeyEvent *>(event);
//        qDebug("Ate key press %d", keyEvent->key());
//        return true;
//    }
    else
    {
        return QObject::eventFilter(object, event);
    }
}
