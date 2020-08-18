#pragma once

#include <QDialog>
#include <QMap>

class SceneModel;
class AirFactory;
class AirKit;
class SoNode;

namespace Ui {
class AirDialog;
}


class AirDialog: public QDialog
{
    Q_OBJECT

public:
    AirDialog(SceneModel* sceneModel, QMap<QString, AirFactory*> airMap, QWidget* parent = 0);
    ~AirDialog();

    AirKit* getAir() {return m_air;}

protected slots:
    void setModel(int index);
    void setValue(SoNode* node, QString field, QString value);

private:
    Ui::AirDialog* ui;
    QMap<QString, AirFactory*> m_airMap;
    AirKit* m_air;
};
