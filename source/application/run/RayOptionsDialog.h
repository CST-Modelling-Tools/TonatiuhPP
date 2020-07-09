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
        int raysNumber,
        QVector<RandomFactory*> randomFactories, int raysRandomFactory = 0,
        int raysPlaneWidth = 200, int raysPlaneHeight = 200,
        bool drawRays = true, bool drawPhotons = false,
        int photonBufferSize = 1'000'000, bool photonBufferAppend = false,
        QWidget* parent = 0, Qt::WindowFlags f = 0
    );
    ~RayOptionsDialog();

    int raysNumber() const;
    int raysRandomFactory() const;
    int rayGridWidth() const;
    int rayGridHeight() const;

    bool drawRays() const;
    bool drawPhotons() const;

    int photonBufferSize() const;
    bool photonBufferAppend() const;

private:
    Ui::RayOptionsDialog* ui;
};
