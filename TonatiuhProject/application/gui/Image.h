#pragma once

#include <QImage>
#include <QString>
#include <QStringList>

class Image: public QImage
{
public:
    Image(QString fileName);

    QString country(int x, int y);
    void addCountries();

private:
    QStringList countries;
};
