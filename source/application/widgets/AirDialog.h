#pragma once

#include <QDialog>
#include <QMap>

class Air;
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

    void setModel(Air* air);
    Air* getModel() const {return m_air;}

public slots:
    void setValue(SoNode* node, QString parameter, QString value);

protected slots:
    void changeModel(int index);

private:
    Ui::AirDialog* ui;
    QMap<QString, AirFactory*> m_airMap;
    int m_index;
    Air* m_airOld;
    Air* m_air;
};
