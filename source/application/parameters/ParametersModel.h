#pragma once

#include <QStandardItemModel>

class SoNode;
class TNode;
class MainWindow;


class ParametersModel: public QStandardItemModel
{
    Q_OBJECT

public:
    ParametersModel(QObject* parent = 0);

    void setNode(SoNode* node);

    void setMain(MainWindow* main) {m_main = main;}
    MainWindow* getMain() const {return m_main;}

    bool setData(const QModelIndex& index, const QVariant& value, int role = Qt::EditRole);
    bool setData(SoNode* node, QString field, TNode* value);

signals:
    void fieldTextModified(SoNode* node, QString field, QString value);
    void fieldNodeModified(SoNode* node, QString field, TNode* value);

protected:
    MainWindow* m_main;
    SoNode* m_node;
};
