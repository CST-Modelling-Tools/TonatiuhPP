#include "CustomSplashScreen.h"

#include <QTimer>
#include <QPainter>
#include <QApplication>
//#include <QDebug>

CustomSplashScreen::CustomSplashScreen(const QPixmap& pLoaded)
{
    m_readyTime = false;
    m_readyWindow = false;

    setWindowFlag(Qt::WindowStaysOnTopHint, true); // show above main window
    QTimer::singleShot(1'000, this, &CustomSplashScreen::setFinishTime); // close in 1000 ms

    int q = fontMetrics().height();
//    qDebug() << "pm " << q;
    qreal r = qApp->devicePixelRatio(); // 2

    int hP = 11*q;
    double s = double(hP)/pLoaded.height();
    int wP = s*pLoaded.width();
    int hT = 1.6*q;

    QPixmap pCustom(wP*r, hP*r + hT*r);
    pCustom.setDevicePixelRatio(r);
//    pCustom.fill(); // with white

    QPainter painter(&pCustom);
    painter.setRenderHint(QPainter::SmoothPixmapTransform, true);

    painter.drawPixmap(0, 0, wP, hP, pLoaded);

    m_rect = QRect(0, hP, wP, hT);
    painter.fillRect(m_rect, QColor("#D2D2D2"));
    m_rect.adjust(hT/3, 0, -hT/3, 0);

    setPixmap(pCustom);
}

void CustomSplashScreen::setMessage(const QString& message, const QColor& color)
{
    m_message = message;
    m_color = color;
    showMessage(m_message);
}

void CustomSplashScreen::drawContents(QPainter* painter)
{
    painter->setPen(m_color);
    painter->drawText(m_rect, m_alignment, m_message);
}

void CustomSplashScreen::setFinishTime()
{
    m_readyTime = true; // set time condition
    if (m_readyWindow) close(); // check other condition
}

void CustomSplashScreen::setFinishWindow()
{
    m_readyWindow = true; // set window condition
    if (m_readyTime) close(); // check other condition
}
