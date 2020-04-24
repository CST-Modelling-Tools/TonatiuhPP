#include "DialogAbout.h"
#include "ui_DialogAbout.h"

#include <QDate>

DialogAbout::DialogAbout(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::DialogAbout)
{
    ui->setupUi(this);

    QString textVersion = qApp->applicationVersion();

    QString textDate = QString(__DATE__).simplified(); // "Dec  5 2018"
    QDate date = QDate::fromString(textDate, "MMM d yyyy");
    textDate = date.toString("d MMM yyyy");

    QString text = QString("Version: %1 (%2)").arg(textVersion).arg(textDate);

    ui->label_version->setText(text);

    Qt::WindowFlags flags = windowFlags();
    flags ^= Qt::WindowContextHelpButtonHint;
    flags |= Qt::MSWindowsFixedSizeDialogHint;
    setWindowFlags(flags);
}

DialogAbout::~DialogAbout()
{
    delete ui;
}
