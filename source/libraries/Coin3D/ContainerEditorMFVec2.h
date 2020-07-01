#pragma once

#include <QHBoxLayout>
#include <QLineEdit>
#include <QPushButton>

#include "libraries/Coin3D/FieldEditor.h"



// delete later
class TONATIUH_LIBRARIES CustomLineEdit: public QLineEdit
{
    Q_OBJECT
public:
    CustomLineEdit(QWidget* parent = 0);

signals:
    void editingFinished();

protected:
    void focusOutEvent(QFocusEvent* event);
};



class TONATIUH_LIBRARIES ContainerEditorMFVec2: public FieldEditor
{
    Q_OBJECT

public:
    ContainerEditorMFVec2(QWidget* parent = 0);

    QString GetData() const;
    void SetData(QString value);
    void SetTitles(QStringList titles);

signals:
    void editingFinished();

protected:
    void focusOutEvent(QFocusEvent* event);

protected slots:
    void CloseEditor();
    void OpenContainerViewer();

private:
    QHBoxLayout* m_layout;
    CustomLineEdit* m_valuesEdit;
    QPushButton* m_editButton;
    QStringList m_titles;
};
