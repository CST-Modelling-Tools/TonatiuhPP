#pragma once

#include <QSplashScreen>

class CustomSplashScreen: public QSplashScreen
{
public:
    CustomSplashScreen(const QPixmap& pixmap):
        QSplashScreen(pixmap) {}
};

