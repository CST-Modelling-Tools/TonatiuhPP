#include "ContainerEditorMFVec2.h"

#include "ContainerViewerMFVec2.h"


CustomLineEdit::CustomLineEdit(QWidget* parent):
    QLineEdit(parent)
{
}

void CustomLineEdit::focusOutEvent(QFocusEvent* event)
{
    emit editingFinished();
    QLineEdit::focusOutEvent(event);
}



ContainerEditorMFVec2::ContainerEditorMFVec2(QWidget* parent):
    FieldEditor(parent)
{
//     resize(880, 192);

     QSizePolicy sp(QSizePolicy::Minimum, QSizePolicy::MinimumExpanding);
     sp.setHorizontalStretch(0);
     sp.setVerticalStretch(0);
     setSizePolicy(sp);
     setFocusPolicy(Qt::NoFocus);
     setMouseTracking(false);

     m_layout = new QHBoxLayout(this);
     m_layout->setSpacing(0);
     m_layout->setContentsMargins(0, 0, 0, 0);

     m_valuesEdit = new CustomLineEdit;
     m_valuesEdit->setMouseTracking(false);
     m_valuesEdit->setFocusPolicy(Qt::WheelFocus);
     connect( m_valuesEdit, SIGNAL( editingFinished() ), this, SLOT( CloseEditor() ) );
     m_layout->addWidget(m_valuesEdit);

     m_editButton = new QPushButton;
     m_editButton->setFocusPolicy(Qt::StrongFocus);
     m_editButton->setText("...");
     connect( m_editButton, SIGNAL( clicked() ), this, SLOT( OpenContainerViewer() ) );
     m_layout->addWidget(m_editButton);
}

/*!
 * Return current value in the editor.
 */
QString ContainerEditorMFVec2::GetData() const
{
    return m_valuesEdit->text();
}

/*!
 * Set defaults values in the editor.
 */
void ContainerEditorMFVec2::SetData(QString value)
{
    m_valuesEdit->setText(value);
}

/*!
 * Set headers laber to the editor.
 */
void ContainerEditorMFVec2::SetTitles(QStringList titles)
{
    m_titles = titles;
}

/*
 * Closes editor if the elements of the editor are not active.
 */
void ContainerEditorMFVec2::focusOutEvent(QFocusEvent* event)
{
    QWidget::focusOutEvent(event);
    CloseEditor();
}

/*!
 * Emits an editing finished signal if the edition is finished and the
 */
void ContainerEditorMFVec2::CloseEditor()
{
    if (!hasFocus() && !m_editButton->hasFocus() && !m_valuesEdit->hasFocus())
        emit editingFinished();
}

/*!
 * Opens a table viewer width the values in the container.
 */
void ContainerEditorMFVec2::OpenContainerViewer()
{
    ContainerViewerMFVec2 viewer;
    if (!viewer.SetData(m_valuesEdit->text()))
        return;
    if (!m_titles.isEmpty())
        viewer.SetTitles(m_titles);

    if (viewer.exec())
        m_valuesEdit->setText(viewer.GetData());
    m_valuesEdit->setFocus(Qt::ActiveWindowFocusReason);
}
