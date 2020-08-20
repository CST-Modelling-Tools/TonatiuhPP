#include "UndoView.h"
#include "ui_UndoView.h"

#include <QUndoStack>


UndoView::UndoView(QUndoStack* stack, QWidget* parent):
    QWidget(parent),
    ui(new Ui::UndoView)
{
    ui->setupUi(this);
    ui->undoView->setStack(stack);

    setWindowTitle("Undo List");
    setAttribute(Qt::WA_QuitOnClose, false);
    int q = fontMetrics().height();
    resize(24*q, 16*q);
}

UndoView::~UndoView()
{
    delete ui;
}

void UndoView::on_pushButton_clicked()
{
    ui->undoView->stack()->clear();
}
