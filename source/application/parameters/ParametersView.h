#pragma once

#include <Inventor/lists/SbStringList.h>

#include <QModelIndex>
#include <QTreeView>

class ParametersDelegate;
class SoField;
class SoNode;
class ParametersModel;
class QStandardItem;

class ParametersView: public QTreeView
{
    Q_OBJECT

public:
    ParametersView(QWidget* parent);
    ~ParametersView();

    void SetContainer(SoNode* node);

protected slots:
    void closeEditor(QWidget* editor, QAbstractItemDelegate::EndEditHint hint);

private slots:
    void onBoolChecked(QStandardItem *item);

signals:
    void valueModified(SoNode* nodeContainer, QString parameterName, QString newValue);

private:
    ParametersModel* m_model;
    SoNode* m_node;
};
