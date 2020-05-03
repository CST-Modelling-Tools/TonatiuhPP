#pragma once

#include <Inventor/lists/SbStringList.h>

#include <QModelIndex>
#include <QTreeView>

class ParametersDelegate;
class SoField;
class SoNode;
class ParametersModel;


class FieldContainerWidget: public QTreeView
{
    Q_OBJECT

public:
    FieldContainerWidget(QWidget* parent = 0);
    FieldContainerWidget(SoNode* fieldContainer, QString name, QWidget* parent = 0);
    ~FieldContainerWidget();

    void SetContainer(SoNode* node, QString name);
    void SetEditable(bool editable);

protected slots:
    void currentChanged(const QModelIndex& current, const QModelIndex& previous);
    void closeEditor(QWidget* editor, QAbstractItemDelegate::EndEditHint hint);

signals:
    void valueModified(SoNode* nodeContainer, QString parameterName, QString newValue);

private:
    void ReadFields();

    SoNode* m_node;
    QString m_name;
    ParametersModel* m_model;
    QModelIndex m_index;
    ParametersDelegate* m_delegate;
};
