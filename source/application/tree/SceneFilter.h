#pragma once

#include <QSortFilterProxyModel>


class SceneFilter: public QSortFilterProxyModel
{
    Q_OBJECT

public:
    SceneFilter(QObject* parent = 0);

    void AddShapeTypeFilter(QString shape);
    void SetShapeTypeFilters(QStringList shapes);

protected:
    bool filterAcceptsRow(int sourceRow, const QModelIndex& sourceParent) const;

private:
    QStringList m_shapes;
};
