#pragma once

#include <QDialog>
#include <QMap>

#include "ui_transmissivitydialog.h"

class TTransmissivity;
class TTransmissivityFactory;


class TransmissivityDialog: public QDialog, private Ui::TransmissivityDialog
{
    Q_OBJECT

public:
    TransmissivityDialog(QVector<TTransmissivityFactory*> transmissivityFactoryList, QWidget* parent = 0, Qt::WindowFlags f = 0);
    ~TransmissivityDialog() {}

    TTransmissivity* GetTransmissivity() const;
    void SetCurrentTransmissivity(TTransmissivity* transmissivity);

public slots:
    void SetValue(SoNode* node, QString paramenterName, QString newValue);

protected slots:
    void ChangeTransmissivityType(int index);

private:
    int m_currentTransmissivityIndex;
    TTransmissivity* m_currentTransmissivity;
    TTransmissivity* m_newTransmissivity;
    QMap<QString, TTransmissivityFactory*> m_transmissivityFactoryList;
};
