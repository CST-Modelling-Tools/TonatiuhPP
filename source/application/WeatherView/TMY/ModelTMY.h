#pragma once

#include <QAbstractTableModel>

#include "ReaderTMY.h"

class ModelTMY: public QAbstractTableModel
{
    Q_OBJECT

public:
    explicit ModelTMY(QObject* parent = nullptr);
    ~ModelTMY();

    sp::ReaderTMY* formatTMY() {return m_formatTMY;}
    sp::RecordsFilter& filter() {return m_filter;}
    void setDatesAll();
    void setDates(QDate dateA, QDate dateB);

    QVariant headerData(int section, Qt::Orientation orientation, int role) const;

    int rowCount(const QModelIndex& parent) const;
    int columnCount(const QModelIndex& parent) const;
    QVariant data(const QModelIndex& index, int role) const;

private:
    sp::ReaderTMY* m_formatTMY;
    sp::RecordsFilter m_filter;
    int m_indexA;
    int m_indexB;
};

