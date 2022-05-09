#include "ParameterModel.h"

#include "ParameterItem.h"

ParameterModel::ParameterModel(QObject* parent):
    QStandardItemModel{parent}
{
    setHorizontalHeaderLabels({"Name", "Value"});
}

void ParameterModel::cleanRows()
{
    removeRows(0, rowCount());
}

QStandardItem* ParameterModel::findParameter(QString name) const
{
    QStringList names = name.split('.');
    QStandardItem* parent = invisibleRootItem();
    for (int n = 0; n < names.size(); n++) {
        int rX = -1;
        for (int r = 0; r < parent->rowCount(); r++) {
            QStandardItem* item = parent->child(r);
            if (item->text() != names[n]) continue;
            rX = r;
            break;
        }

        if (rX < 0) return 0;
        if (n == names.size() - 1)
            return parent->child(rX, 1);
        else
            parent = parent->child(rX);
    }
    return 0;
}

ParameterAbstract* ParameterModel::parameterAbstract(QString name) const
{
    return dynamic_cast<ParameterAbstract*>(findParameter(name));

}

ParameterDouble* ParameterModel::parameterDouble(QString name) const
{
    return dynamic_cast<ParameterDouble*>(findParameter(name));
}

QString ParameterModel::parameter(QString name) const
{
    QStandardItem* item = findParameter(name);
    if (!item) {
        qDebug() << "Parameter not found: " << name;
        return QString();
    }

    QStandardItem* itemV = item->parent()->child(item->row(), 1);
    return itemV->text();
}

void ParameterModel::setParameter(QString name, QString value)
{
    QStandardItem* item = findParameter(name);
    if (!item) {
        qDebug() << "Parameter not found: " << name;
        return;
    }

    QStandardItem* itemV = item->parent()->child(item->row(), 1);
    return itemV->setText(value);
}

QStandardItem* ParameterModel::appendParameter(QStandardItem* parent, ParameterAbstract* item)
{
    QStandardItem* label = new QStandardItem(item->name());
    label->setEditable(false);

    if (!item->isEditable())
        item->setData(QBrush("#A0A0A0"), Qt::ForegroundRole);

    parent->appendRow({label, item});
    return label;
}

#include "ParameterItem.h"

