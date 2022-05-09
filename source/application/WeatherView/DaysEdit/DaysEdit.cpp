#include "DaysEdit.h"
#include "ui_DaysEdit.h"
#include <QLineEdit>
#include <QKeyEvent>
#include <QHeaderView>

#include "GridPick.h"

// //https://doc.qt.io/archives/qq/qq07-popupcalendar.html

QString sMonths =
        "January,February,March,April,May,June,"
        "July,August,September,October,November,December";

DaysEdit::DaysEdit(QWidget* parent):
    QWidget(parent),
    ui(new Ui::DaysEdit)
{
    ui->setupUi(this);

    QString months = QString(",") + sMonths;
    ui->comboBox_Month->addItems(months.split(','));
    ui->comboBox_Month->installEventFilter(this);

    QStringList days;
    for (int n = 0; n <= 31; ++n) {
        days << QString::number(n);
    }
    ui->comboBox_Day->addItems(days);
    ui->comboBox_Day->installEventFilter(this);

    setStyleSheet(R"(
        QAbstractItemView::item {
            outline:none;
        }
//        QComboBox {outline: 0;}
//        QLineEdit {
//            background: #E1E1E1;
//            border: 1px solid #B4B4B4;
//            padding-top: 1px;
//        }
    )");
    updateCombos();

    connect(this, SIGNAL(pickMonth()), this, SLOT(onPickMonth()));
    connect(this, SIGNAL(pickDay()), this, SLOT(onPickDay()));
}

DaysEdit::~DaysEdit()
{
    delete ui;
}

void DaysEdit::updateCombos()
{
    ui->comboBox_Month->setCurrentIndex(m_daysModel.month());
    ui->comboBox_Day->setCurrentIndex(m_daysModel.day());
    emit dateChanged();
}

void DaysEdit::onPickMonth()
{
    QList<QStandardItem*> list;
    for (const QString& txt : sMonths.split(','))
        list << new QStandardItem(txt);

    QStandardItemModel model;
    model.appendColumn(list.mid(0, 6));
    model.appendColumn(list.mid(6, 6));

    GridPick dialog(&model);
    dialog.setDirection(Qt::Vertical);
    QPoint pos = ui->comboBox_Month->rect().bottomLeft();
    pos = ui->comboBox_Month->mapToGlobal(pos);
    dialog.move(pos);
    dialog.view()->horizontalHeader()->setDefaultSectionSize(75);
    dialog.view()->verticalHeader()->setDefaultSectionSize(20);
    dialog.resize(2*75 + 2, 6*22 + 14);

    int m = m_daysModel.month() - 1;
    dialog.view()->setCurrentIndex(model.index(m%6, m/6));
    dialog.exec();

    QModelIndex index = dialog.view()->currentIndex();
    m_daysModel.setMonth(index.column()*6 + index.row() + 1);
    updateCombos();
}


void DaysEdit::onPickDay()
{
    int dMax = m_daysModel.daysInMonth();
    int cMax = 7;
    int rMax = dMax/cMax;
    if (dMax % cMax) rMax++;

    QStandardItemModel model(rMax, cMax);
    int d = 1;
    for (int r = 0; r < rMax; r++) {
        for (int c = 0; c < cMax; c++) {
            QStandardItem* item = new QStandardItem();
            if (d <= dMax) {
                QString text = QString("%1").arg(d);
                item->setText(text);
                item->setTextAlignment(Qt::AlignHCenter | Qt::AlignVCenter);
            } else {
                item->setEnabled(false);
            }
            model.setItem(r, c, item);
            d++;
        }
    }

    GridPick dialog(&model);
    QPoint pos = ui->comboBox_Day->rect().bottomLeft();
    pos = ui->comboBox_Day->mapToGlobal(pos);
    dialog.move(pos);
    dialog.view()->horizontalHeader()->setDefaultSectionSize(28);
    dialog.view()->verticalHeader()->setDefaultSectionSize(20);
    dialog.resize(cMax*32 + 2, rMax*22 + 12);

    d = m_daysModel.day() - 1;
    dialog.view()->setCurrentIndex(model.index(d/7, d%7));
    dialog.exec();

    QModelIndex index = dialog.view()->currentIndex();
    d = index.row()*7 + index.column() + 1;
    m_daysModel.setDay(d);
    updateCombos();
}

bool DaysEdit::eventFilter(QObject* watched, QEvent* event)
{
    if (watched == ui->comboBox_Day) {
        if (event->type() == QEvent::KeyPress) {
            QKeyEvent* keyEvent = static_cast<QKeyEvent*>(event);
            int d = 0;
            switch (keyEvent->key()) {
                case Qt::Key_Left:
                case Qt::Key_Up:
                    d = -1; break;
                case Qt::Key_Right:
                case Qt::Key_Down:
                    d = 1; break;
                case Qt::Key_Return:
                    emit pickDay(); break;
            }
            if (d != 0) {
                m_daysModel.addDays(d);
                updateCombos();
                return true;
            }
        } else if (event->type() == QEvent::MouseButtonPress) {
            QMouseEvent* mouseEvent = static_cast<QMouseEvent*>(event);
            if (mouseEvent->button() == Qt::LeftButton) {
                emit pickDay();
                return true;
            }
        }
    }
    else if (watched == ui->comboBox_Month) {
        if (event->type() == QEvent::KeyPress) {
            QKeyEvent* keyEvent = static_cast<QKeyEvent*>(event);
            int d = 0;
            switch (keyEvent->key()) {
                case Qt::Key_Left:
                case Qt::Key_Up:
                    d = -1; break;
                case Qt::Key_Right:
                case Qt::Key_Down:
                    d = 1; break;
            }
            if (d != 0) {
                m_daysModel.addMonths(d);
                updateCombos();
                return true;
            }
        } else if (event->type() == QEvent::MouseButtonPress) {
            QMouseEvent* mouseEvent = static_cast<QMouseEvent*>(event);
            if (mouseEvent->button() == Qt::LeftButton) {
                emit pickMonth();
                return true;
            }
        }
    }
    return QWidget::eventFilter(watched, event);
}

