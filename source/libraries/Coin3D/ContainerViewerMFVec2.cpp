#include "ContainerViewerMFVec2.h"

#include "ui_containerviewermfvec2.h"
#include "DoubleValuesDelegate.h"
#include <QMessageBox>
#include "libraries/math/gcf.h"
#include <QRegularExpression>
/*!
 * Creates an dialog object
 */
ContainerViewerMFVec2::ContainerViewerMFVec2(QWidget* parent):
    QDialog(parent),
    ui(new Ui::ContainerViewerMFVec2)
{
    ui->setupUi(this);
    ui->tableWidget->setItemDelegate(new DoubleValuesDelegate);

    connect(
        ui->addButton, SIGNAL(clicked()),
        this, SLOT(AddNewRow())
    );
    connect(
        ui->deleteButton, SIGNAL(clicked()),
        this, SLOT(DeleteSelectedRow())
    );
    connect(
        ui->helpButton, SIGNAL(clicked()),
        this, SLOT(HelpMenu())
    );
    connect(
        ui->buttonBox, SIGNAL(accepted()),
        this, SLOT(CloseViewer())
    );
}

ContainerViewerMFVec2::~ContainerViewerMFVec2()
{
    delete ui;
}

/*!
 * Return current values in the editor.
 */
QString ContainerViewerMFVec2::GetData() const
{
    int rows = ui->tableWidget->rowCount();
    int columns = ui->tableWidget->columnCount();

    QString data("[");
    for (int r = 0; r < rows; ++r)
    {
        for (int c = 0; c < columns; ++c)
        {
            data += ui->tableWidget->item(r, c)->text();
            if (c < columns - 1) data += " ";
        }
        if (r < rows - 1) data += ",\n";
    }
    data += "]";

    return data;
}

/*!
 * Sets the values in the string into the table.
 */
bool ContainerViewerMFVec2::SetData(QString value)
{
    QStringList values = value.split(QRegularExpression("[\\[\\]]"), Qt::SkipEmptyParts);
    if (values.size() != 1) return false;

    QStringList elements = values[0].split(",", Qt::SkipEmptyParts);
    int rows = elements.count();
    int columns = -1;
    for (int r = 0; r < rows; r++)
    {
        QStringList elementValues = elements[r].split(QRegularExpression("\\s+"), Qt::SkipEmptyParts);
        if (r == 0)
            columns = elementValues.count();
        else if (r > 0 && columns != elementValues.count())
            return false;
    }

    ui->tableWidget->setRowCount(rows);
    ui->tableWidget->setColumnCount(columns);
    for (int r = 0; r < rows; r++)
    {
        QStringList elementValues = elements[r].split(QRegularExpression("\\s+"), Qt::SkipEmptyParts);
        for (int c = 0; c < columns; c++)
        {
            QTableWidgetItem* item = new QTableWidgetItem(elementValues[c]);
            ui->tableWidget->setItem(r, c, item);
        }
    }
    return true;
}

void ContainerViewerMFVec2::SetTitles(QStringList titles)
{
    ui->tableWidget->setHorizontalHeaderLabels(titles);
}

/*!
 * Adds an empty row to the end of the table.
 */
void ContainerViewerMFVec2::AddNewRow()
{
    int rows = ui->tableWidget->rowCount();
    int columns = ui->tableWidget->columnCount();
    ui->tableWidget->insertRow(rows);
    for (int c = 0; c < columns; c++)
    {
        QTableWidgetItem* item = new QTableWidgetItem("0");
        ui->tableWidget->setItem(rows, c, item);
    }
}

/*!
 * Delete selected row.
 */
void ContainerViewerMFVec2::DeleteSelectedRow()
{
    int res = QMessageBox::question(this, "Tonatiuh",
        "Are you sure you want to delete current selected row?");
    if (res != QMessageBox::Yes) return;

    int row = ui->tableWidget->currentRow();
    ui->tableWidget->removeRow(row);
}

void ContainerViewerMFVec2::HelpMenu()
{
    QString message = R"(
Text
)";
    QMessageBox::information(this, "Tonatiuh", message);
}

void ContainerViewerMFVec2::CloseViewer()
{
    if (OkToContinue()) accept();
}

/*!
 * Verifies if the table values are well defined.
 */
bool ContainerViewerMFVec2::OkToContinue()
{
    int rows = ui->tableWidget->rowCount();
    int columns = ui->tableWidget->columnCount();

    for (int r = 0; r < rows; r++)
    {
        for (int c = 0; c < columns; c++)
        {
            QString itemData = ui->tableWidget->item(r, c)->text();
            if (itemData.isEmpty())
            {
                QString message = QString("Empty cell [%1, %2].").arg(QString::number(r), QString::number(c));
                QMessageBox::warning(this, "Tonatiuh", message);
                return false;
            }
        }
    }

    return true;
}
