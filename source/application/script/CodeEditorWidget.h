#pragma once

#include <QWidget>

class QTextDocument;

namespace Ui {
class CodeEditorWidget;
}


class CodeEditorWidget: public QWidget
{
    Q_OBJECT

public:
    CodeEditorWidget(QWidget* parent = 0, Qt::WindowFlags f = 0);
    ~CodeEditorWidget();

    QTextDocument* document() const;
    bool isReady();

public slots:
    void open(QString fileName = QString());
    void openNew();
    bool save(QString fileName = QString());
    bool saveThis();
    void run();

signals:
    void opened(QString fileName);
    void saved(QString fileName);
    void runned();

private:
    Ui::CodeEditorWidget* ui;
    QString m_fileName;
};
