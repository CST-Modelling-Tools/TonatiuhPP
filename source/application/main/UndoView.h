#pragma once

#include <QWidget>
#include <QUndoView>

namespace Ui {
class UndoView;
}

class UndoView : public QWidget
{
    Q_OBJECT

public:
    explicit UndoView(QUndoStack* stack, QWidget *parent = nullptr);
    ~UndoView();

private slots:
    void on_pushButton_clicked();

private:
    Ui::UndoView *ui;
};

