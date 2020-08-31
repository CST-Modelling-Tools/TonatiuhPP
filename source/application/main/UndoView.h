#pragma once

#include <QWidget>
#include <QDialog>
#include <QUndoView>

namespace Ui {
class UndoView;
}

class UndoView : public QDialog
{
    Q_OBJECT

public:
    explicit UndoView(QUndoStack* stack, QWidget* parent = nullptr);
    ~UndoView();

private slots:
    void on_pushButton_clicked();

protected:
    void closeEvent(QCloseEvent* event);
    void showEvent(QShowEvent* event);

private:
    Ui::UndoView* ui;
    QRect m_geometry;
};

