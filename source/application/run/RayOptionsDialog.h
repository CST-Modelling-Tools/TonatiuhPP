#pragma once

#include <QDialog>

class RandomFactory;


namespace Ui {
class RayOptionsDialog;
}


class RayOptionsDialog: public QDialog
{
    Q_OBJECT

public:
    RayOptionsDialog(
        int raysNumber, int raysScreen,
        QVector<RandomFactory*> randomFactories, int raysRandomFactory = 0,
        int raysGridWidth = 200, int raysGridHeight = 200,
        int photonBufferSize = 1'000'000, bool photonBufferAppend = false,
        QWidget* parent = 0, Qt::WindowFlags f = 0
    );
    ~RayOptionsDialog();

    int raysNumber() const;
    int raysScreen() const;
    int raysRandomFactory() const;
    int raysGridWidth() const;
    int raysGridHeight() const;

    int photonBufferSize() const;
    bool photonBufferAppend() const;

private:
    Ui::RayOptionsDialog* ui;
};
