#include "CustomSplashScreen.h"

#include <QPainter>
//#include <QDebug>


CustomSplashScreen::CustomSplashScreen(const QPixmap& p)
{
    int q = fontMetrics().height(); // 25
//    qDebug() << q;
    QPixmap pScaled = p.scaledToHeight(12*q, Qt::SmoothTransformation);
    int h = 1.8*q;

    QPixmap pCustom(pScaled.width(), pScaled.height() + h);
    pCustom.fill();
    QPainter painter(&pCustom);
    painter.drawPixmap(0, 0, pScaled);

    m_rect = QRect(0, pScaled.height(), pScaled.width(), h);
    painter.fillRect(m_rect, QColor("#D2D2D2"));
    m_rect.adjust(h/3, 0, -h/3, 0);

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
