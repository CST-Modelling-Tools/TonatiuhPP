#pragma once

#include <QMainWindow>

namespace Ui {
class ScriptWindow;
}

class ScriptWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit ScriptWindow(QWidget *parent = nullptr);
    ~ScriptWindow();

private:
    Ui::ScriptWindow *ui;
};

