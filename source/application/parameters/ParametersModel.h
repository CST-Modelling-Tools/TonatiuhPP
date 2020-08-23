#pragma once

#include <QStandardItemModel>

class SoNode;
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

signals:
    void valueModified(SoNode* node, QString field, QString value);

protected:
    MainWindow* m_main;
};
