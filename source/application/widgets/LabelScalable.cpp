#include "LabelScalable.h"


LabelScalable::LabelScalable(QWidget *parent):
    QLabel(parent)
{
    setMinimumSize(1,1);
    setScaledContents(false);
}

int LabelScalable::heightForWidth(int width) const
{
    if (m_pixmap.isNull())
        return height();

    double f = double(width)/m_pixmap.width();
    return f*height();
}

QSize LabelScalable::sizeHint() const
{
    int w = width();
    return QSize(w, heightForWidth(w));
}

void LabelScalable::setPixmap(const QPixmap& p)
{
    m_pixmap = p;
    QLabel::setPixmap(scaledPixmap());
}

QPixmap LabelScalable::scaledPixmap() const
{
    return m_pixmap.scaled(size(), Qt::KeepAspectRatio, Qt::SmoothTransformation);
}

void LabelScalable::resizeEvent(QResizeEvent * e)
{
    if (!m_pixmap.isNull())
        QLabel::setPixmap(scaledPixmap());
}
