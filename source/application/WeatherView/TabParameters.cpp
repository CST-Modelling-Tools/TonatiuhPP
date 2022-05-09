#include "TabParameters.h"
#include "ui_TabParameters.h"

#include "TMY/ReaderTMY.h"
#include "TMY/ModelTMY.h"
#include "TMY/ParameterModel.h"
#include <QFileDialog>

TabParameters::TabParameters(QWidget* parent):
    QWidget(parent),
    ui(new Ui::TabParameters)
{
    ui->setupUi(this);

    ui->splitter->setStretchFactor(0, 1);
    ui->splitter->setStretchFactor(1, 1);
    ui->splitter->setSizes({200, 200});

//    ui->lineEdit_File->setText("../data/Seville_GTER_TMY.csv");

    m_paramsOriginal = new ParameterModel(this);
    ui->treeView_Original->setModel(m_paramsOriginal);
    ui->treeView_Original->header()->resizeSection(0, 150);

    m_paramsEffective = new ParameterModel(this);
    ui->treeView_Effective->setModel(m_paramsEffective);
    ui->treeView_Effective->header()->resizeSection(0, 150);

    connect(m_paramsEffective, SIGNAL(itemChanged(QStandardItem*)),
            this, SLOT(on_active(QStandardItem*)));
}

TabParameters::~TabParameters()
{
    delete ui;
}

void TabParameters::setModelTMY(ModelTMY* model)
{
    m_modelData = model;
}

#include <QSettings>
void TabParameters::on_pushButton_Open_clicked()
{
    QSettings settings("Tonatiuh", "Cyprus");
    QString dirName = settings.value("dirProjects", "").toString();

    QString fileName = QFileDialog::getOpenFileName(
        this, "Open File", dirName, "file (*.*)"
    );
    if (fileName.isEmpty()) return;

//    QDir dir(dirName);
    fileName = QDir::current().absoluteFilePath(fileName);
    ui->lineEdit_File->setText(fileName);
    on_lineEdit_File_editingFinished();
}

void TabParameters::on_lineEdit_File_editingFinished()
{
    sp::ReaderTMY* format = m_modelData->formatTMY();
    format->setModels(m_paramsOriginal, m_paramsEffective);
    QString fileName = ui->lineEdit_File->text();
    format->read(fileName);
    ui->treeView_Effective->expandAll();
//    m_modelData->setDatesAll();
    emit fileOpened();
}

void TabParameters::on_active(QStandardItem* item)
{
    if (!item->isEditable()) return;

    m_modelData->formatTMY()->setDNI("DNI");
//    qDebug() << "onActive " << item->text();
}

