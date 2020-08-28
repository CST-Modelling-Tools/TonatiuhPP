#include "ScriptWindow.h"
#include "ui_ScriptWindow.h"

ScriptWindow::ScriptWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::ScriptWindow)
{
    ui->setupUi(this);
}

ScriptWindow::~ScriptWindow()
{
    delete ui;
}
