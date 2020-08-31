#include "UndoView.h"
#include "ui_UndoView.h"

#include <QUndoStack>


UndoView::UndoView(QUndoStack* stack, QWidget* parent):
    QDialog(parent),
    ui(new Ui::UndoView)
{
    ui->setupUi(this);
    ui->undoView->setStack(stack);

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

void UndoView::closeEvent(QCloseEvent* event)
{
    m_geometry = geometry();
    QDialog::closeEvent(event);
}

void UndoView::showEvent(QShowEvent* event)
{
    QDialog::showEvent(event);
    if (m_geometry.isValid())
        setGeometry(m_geometry);
}
