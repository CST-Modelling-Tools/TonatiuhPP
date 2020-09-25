#pragma once

#include <QSplashScreen>

class CustomSplashScreen: public QSplashScreen
{
public:
    CustomSplashScreen(const QPixmap& p);

    void setAlignment(int alignment = Qt::AlignLeft) {m_alignment = alignment;}
    void setMessage(const QString& message, const QColor& color = Qt::black);

    void drawContents(QPainter* painter);

public slots:
    void setFinishTime();
    void setFinishWindow();

private:
    QRect m_rect;
    int m_alignment;
    QString m_message;
    QColor m_color;
    bool m_readyTime;
    bool m_readyWindow;
};

