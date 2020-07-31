#pragma once

#include <QDialog>
#include <QMap>

class AirTransmission;
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

    void setModel(AirTransmission* air);
    AirTransmission* getModel() const {return m_air;}

public slots:
    void setValue(SoNode* node, QString parameter, QString value);

protected slots:
    void changeModel(int index);

private:
    Ui::AirDialog* ui;
    QMap<QString, AirFactory*> m_airMap;
    int m_index;
    AirTransmission* m_airOld;
    AirTransmission* m_air;
};
