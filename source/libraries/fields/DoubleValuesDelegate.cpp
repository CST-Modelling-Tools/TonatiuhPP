#include "DoubleValuesDelegate.h"

#include <QLineEdit>
#include <QDoubleValidator>


DoubleValuesDelegate::DoubleValuesDelegate(QObject* parent):
    QItemDelegate(parent)
{

}

/*!
 * Creates an editor for the item identified by \a index.
 */
QWidget* DoubleValuesDelegate::createEditor(QWidget* parent, const QStyleOptionViewItem& /*option*/, const QModelIndex& /*index*/ ) const
{
    QLineEdit* editor = new QLineEdit(parent);
    QDoubleValidator* validator = new QDoubleValidator(editor);
    validator->setNotation(QDoubleValidator::StandardNotation);
    editor->setValidator(validator);
    return editor;
}

/*!
 * Sets the item value to the editor.
 */
void DoubleValuesDelegate::setEditorData(QWidget* editor, const QModelIndex& index) const
{
    double value = index.model()->data(index, Qt::EditRole).toDouble();
    QLineEdit* lineEditor = static_cast<QLineEdit*>(editor);
    lineEditor->setText(QString::number(value));
}

/*!
 * Sets the editor value to the item.
 */
void DoubleValuesDelegate::setModelData(QWidget* editor, QAbstractItemModel* model, const QModelIndex& index ) const
{
    QLineEdit* lineEditor = static_cast<QLineEdit*>(editor);
    QString value = lineEditor->text();
    model->setData(index, value);
}

void DoubleValuesDelegate::updateEditorGeometry(QWidget* editor, const QStyleOptionViewItem& option, const QModelIndex& /*index*/) const
{
    editor->setGeometry(option.rect);
}
