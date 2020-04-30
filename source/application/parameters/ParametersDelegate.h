#pragma once

#include <QStyledItemDelegate>
#include <QObject>


class ParametersDelegate: public QStyledItemDelegate
{
    Q_OBJECT

public:
    ParametersDelegate(QObject* parent = 0);
    ~ParametersDelegate();
    
    QWidget *createEditor(QWidget *parent, const QStyleOptionViewItem &option,
                           const QModelIndex &index) const;                       
    void setEditorData(QWidget *editor, const QModelIndex &index) const;

public slots:
    void CloseEditor();
};
