#pragma once

#include <QWidget>

QT_BEGIN_NAMESPACE
namespace Ui { class DaysEdit; }
QT_END_NAMESPACE

#include "DaysModel.h"

class DaysEdit: public QWidget
{
    Q_OBJECT

public:
    DaysEdit(QWidget *parent = nullptr);
    ~DaysEdit();

    void updateCombos();
    DaysModel& daysModel() {return m_daysModel;}

signals:
    void pickMonth();
    void pickDay();
    void dateChanged();

private slots:
    void onPickMonth();
    void onPickDay();

private:
    Ui::DaysEdit* ui;
    DaysModel m_daysModel;

public:
    bool eventFilter(QObject* watched, QEvent* event);
};
