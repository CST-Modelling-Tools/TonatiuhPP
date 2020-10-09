#include "KeyFilter.h"

#include <QKeyEvent>
#include <QDebug>

KeyFilter::KeyFilter(QObject* parent):
    QObject(parent)
{

}

bool KeyFilter::eventFilter(QObject* object, QEvent* event)
{
//    https://bugreports.qt.io/browse/QTBUG-46812?attachmentOrder=asc

    if (event->type() == QEvent::ShortcutOverride)
    {
//        QKeyEvent *kev = static_cast<QKeyEvent*>(event);
//        if ((kev->key() == Qt::Key_Alt || kev->key() == Qt::Key_Meta)
//                && kev->modifiers() == Qt::AltModifier) {
//            return  true;
//        }
//        return QObject::eventFilter(object, event);
//        qDebug() << "consumed";
//        event->accept();
        return true;
    }

//    if (event->type() == QEvent::KeyPress || event->type() == QEvent::KeyRelease)
//    {
//        QKeyEvent* keyEvent = static_cast<QKeyEvent *>(event);
//        if (keyEvent->key() == Qt::Key_Alt)
//            return true;
////        qDebug("Ate key press %d", keyEvent->key());
////        return true;
//    }

    return QObject::eventFilter(object, event);
}
