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
    ParametersView(QWidget* parent);
//    ParametersView(SoNode* fieldContainer, QWidget* parent = 0);
    ~ParametersView();

    void SetContainer(SoNode* node);
    void SetEditable(bool editable);

protected slots:
    void closeEditor(QWidget* editor, QAbstractItemDelegate::EndEditHint hint);

signals:
    void valueModified(SoNode* nodeContainer, QString parameterName, QString newValue);

private:
    ParametersModel* m_model;
    ParametersDelegate* m_delegate;
    SoNode* m_node;
};
