#pragma once

#include <QSortFilterProxyModel>
#include <QVector>


class SceneFilter: public QSortFilterProxyModel
{
    Q_OBJECT

public:
    SceneFilter(QObject* parent = 0);

    void AddShapeTypeFilter(QString shapeType);
    void SetShapeTypeFilters(QVector<QString> shapeTypeFilters);

protected:
    bool filterAcceptsRow(int sourceRow, const QModelIndex& sourceParent) const;

private:
    QVector<QString> m_shapeTypeList;
};
