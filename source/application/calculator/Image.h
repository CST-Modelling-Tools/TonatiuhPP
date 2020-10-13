#pragma once

#include <QImage>
#include <QStringList>


class Image: public QImage
{
public:
    Image();

    QString country(int x, int y);

private:
    void initCountries();

private:
    QStringList countries;
};
