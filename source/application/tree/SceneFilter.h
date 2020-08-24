#pragma once

#include <QSortFilterProxyModel>


class SceneFilter: public QSortFilterProxyModel
{
    Q_OBJECT

public:
    SceneFilter(QObject* parent = 0);

    void setShapeTypes(QStringList shapes) {m_shapes = shapes;}
    void addShapeType(QString shape);

protected:
    bool filterAcceptsRow(int sourceRow, const QModelIndex& sourceParent) const;

private:
    QStringList m_shapes;
};
