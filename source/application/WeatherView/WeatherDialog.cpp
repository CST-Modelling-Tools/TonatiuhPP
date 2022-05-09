#include "WeatherDialog.h"
#include "ui_WeatherDialog.h"
#include <QFileDialog>

#include "TMY/ReaderTMY.h"
#include "TMY/ModelTMY.h"

#include "TabParameters.h"
#include "TabData.h"
#include "TabWeights.h"

WeatherDialog::WeatherDialog(QWidget* parent):
    QMainWindow(parent),
    ui(new Ui::WeatherDialog)
{
    QPalette p = palette();
    p.setColor(QPalette::HighlightedText, QColor("#000000"));
    p.setColor(QPalette::Highlight, QColor("#cfe3ed"));
    setPalette(p);

    ui->setupUi(this);

    ui->tabWidget->removeTab(0);

    m_modelData = new ModelTMY(this);

    m_tabParameters = new TabParameters(this);
    m_tabParameters->setPalette(p);
    m_tabParameters->setModelTMY(m_modelData);
    ui->tabWidget->addTab(m_tabParameters, "Parameters");

    m_tabData = new TabData;
    m_tabData->setModelTMY(m_modelData);
    ui->tabWidget->addTab(m_tabData, "Data");

    m_tabWeights = new TabWeights;
    m_tabWeights->setModelTMY(m_modelData);
    ui->tabWidget->addTab(m_tabWeights, "Weights");

    connect(
        m_tabParameters, SIGNAL(fileOpened()),
        m_tabData, SLOT(onOpened())
    );

//    setStyleSheet(R"(
//
//    )");

    QList<QWidget*> widgets = findChildren<QWidget*>();
    foreach (QWidget* w, widgets)
        w->setPalette(p);
}

WeatherDialog::~WeatherDialog()
{
    delete ui;
    delete m_modelData;
}
