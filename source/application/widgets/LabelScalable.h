#pragma once

#include <QLabel>
#include <QPixmap>
#include <QResizeEvent>

class LabelScalable: public QLabel
{
    Q_OBJECT
public:
    explicit LabelScalable(QWidget* parent = 0);
    int heightForWidth(int width) const;
    QSize sizeHint() const;

public slots:
    void setPixmap(const QPixmap &);
    void resizeEvent(QResizeEvent *);

protected:
    QPixmap scaledPixmap() const;

protected:
    QPixmap m_pixmap;
};
