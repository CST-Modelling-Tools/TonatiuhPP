#pragma once

#include <Inventor/lists/SbStringList.h>

#include <QModelIndex>
#include <QTreeView>

class ParametersDelegate;
class SoField;
class SoNode;
class ParametersModel;

class FieldContainerWidget : public QTreeView
{
    Q_OBJECT

public:
    FieldContainerWidget( QWidget* parent = 0 );
    FieldContainerWidget( SoNode* fieldContainer, QString containerName, QWidget* parent = 0 );
    ~FieldContainerWidget();

    void SetContainer( SoNode* fieldContainer, QString containerName );
    void SetEditable( bool editable );

protected slots:
    void currentChanged ( const QModelIndex & current, const QModelIndex & previous );
    void closeEditor ( QWidget * editor, QAbstractItemDelegate::EndEditHint hint );

signals:
    void valueModificated( SoNode* nodeContainer, QString parameterName, QString newValue );

private:
    void ReadFields( );

       QString m_containerName;
       QModelIndex m_currentIndex;
    ParametersDelegate* m_pDelegate;
    SoNode* m_pFieldContainer;
       ParametersModel* m_pModel;

};
