#pragma once

#include <QDialog>
#include <QMap>

class AirAbstract;
class AirFactory;
class SoNode;

namespace Ui {
class AirDialog;
}


class AirDialog: public QDialog
{
    Q_OBJECT

public:
    AirDialog(QMap<QString, AirFactory*> airMap, QWidget* parent = 0, Qt::WindowFlags f = 0);
    ~AirDialog();

    void setModel(AirAbstract* air);
    AirAbstract* getModel() const {return m_air;}

public slots:
    void setValue(SoNode* node, QString parameter, QString value);

protected slots:
    void changeModel(int index);

private:
    Ui::AirDialog* ui;
    QMap<QString, AirFactory*> m_airMap;
    int m_index;
    AirAbstract* m_airOld;
    AirAbstract* m_air;
};
