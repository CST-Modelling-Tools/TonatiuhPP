#pragma once

#include <Inventor/lists/SbStringList.h>

#include <QModelIndex>
#include <QTreeView>

class ParametersDelegate;
class SoField;
class SoNode;
class ParametersModel;


class ParametersView: public QTreeView
{
    Q_OBJECT

public:
    ParametersView(QWidget* parent = 0);
    ParametersView(SoNode* fieldContainer, QString name, QWidget* parent = 0);
    ~ParametersView();

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
