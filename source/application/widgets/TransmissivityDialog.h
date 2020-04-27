#pragma once

#include <QDialog>
#include <QMap>

#include "ui_transmissivitydialog.h"

class AirAbstract;
class AirFactory;


class TransmissivityDialog: public QDialog, private Ui::TransmissivityDialog
{
    Q_OBJECT

public:
    TransmissivityDialog(QVector<AirFactory*> transmissivityFactoryList, QWidget* parent = 0, Qt::WindowFlags f = 0);
    ~TransmissivityDialog() {}

    AirAbstract* GetTransmissivity() const;
    void SetCurrentTransmissivity(AirAbstract* transmissivity);

public slots:
    void SetValue(SoNode* node, QString paramenterName, QString newValue);

protected slots:
    void ChangeTransmissivityType(int index);

private:
    int m_currentTransmissivityIndex;
    AirAbstract* m_currentTransmissivity;
    AirAbstract* m_newTransmissivity;
    QMap<QString, AirFactory*> m_transmissivityFactoryList;
};
