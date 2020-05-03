#include <QString>

#include <Inventor/nodekits/SoBaseKit.h>

#include "FieldContainerWidget.h"
#include "ParametersTabs.h"

/**
 * Creates a new ParametersView with parent \a parent.
 */
ParametersTabs::ParametersTabs(QWidget* parent):
    QTabWidget(parent),
    m_actualCoinNode(0),
    m_isPart(false)
{
    addTab( new FieldContainerWidget(0, "" ), tr("Transform") );
    addTab( new FieldContainerWidget(0, "" ), tr("Shape") );

    setStyleSheet(R"(
QTabWidget::pane {
border:none;
}
    )");

}
/*!
 * Destroys the parameters view widget.
 */
ParametersTabs::~ParametersTabs()
{

}

/*!
 * Changes the parameters view to show \a coinNode \a parts parameters.
 */
void ParametersTabs::SelectionChangedToPart(SoNode* coinPart)
{
    clear();

    if (coinPart->getTypeId().isDerivedFrom(SoNodeKitListPart::getClassTypeId()))
    {
        /*SoNodeKitListPart* parentGroup = static_cast< SoNodeKitListPart* >( coinPart );
        if( parentGroup )
        {
            int child=0;
            while( child < parentGroup->getNumChildren() && child <10)
            {
                SoNode* element = (SoNode*)  parentGroup->getChild(child);
                if( element )    AddTab( element, "" );
                child++;
            }
        }*/
    }
    else
    {
        m_actualCoinNode = coinPart;
        m_isPart = true;
        AddTab(coinPart, "");
    }
}

/*!
 * Changes the parameters view to show \a coinNode \a parts parameters.
 */
void ParametersTabs::SelectionChangedToKit( SoBaseKit* coinNode/*, QStringList parts*/ )
{
    clear();

    QStringList parts = ContainerNodeParts(coinNode);

    m_actualCoinNode = coinNode;
    m_isPart = false;
    for (int i = 0; i < parts.size(); ++i)
    {
        QString partName = parts[i];

        if (partName == QString( "" )) AddTab( coinNode, "" );
        else if (partName[partName.length()-1]=='*')
        {
            QString partName2 = partName.left(partName.length()-1);
            SoNodeKitListPart* parentGroup = static_cast< SoNodeKitListPart* >( coinNode->getPart(partName2.toStdString().c_str(), false ) );
            if( parentGroup )
            {
                int child=0;
                while( child < parentGroup->getNumChildren() && child <10)
                {
                    SoNode* element = (SoNode*)  parentGroup->getChild(child);
                    if( element )    AddTab( element, "" );
                    child++;
                }
            }
        }
        else
        {
            SoNode* coinPart = coinNode->getPart(partName.toStdString().c_str(), false );
            if( coinPart != 0 ) AddTab( coinPart, partName );
        }
    }

}

/*!
 * Updates selected node parametes views.
 */
void ParametersTabs::UpdateView()
{
    if (m_isPart) //?
        SelectionChangedToPart (m_actualCoinNode);
    else
        SelectionChangedToKit(  (SoBaseKit*)m_actualCoinNode );
}

/*!
 * Emits a valueModified signal with \a node as the actual node, \a paramenterName and \a newValue.
 */
void ParametersTabs::SetValue(SoNode* node, QString parameterName, QString newValue)
{
    emit valueModified(node, parameterName, newValue);
}

/*!
 * Adds a new tab to the view with \a coinNode \a partName parameters.
 */
void ParametersTabs::AddTab(SoNode* coinNode, QString partName)
{
    QString type = coinNode->getName().getString();
    if (type.length() <= 0) type = coinNode->getTypeId().getName().getString();

    FieldContainerWidget* nodeContainer = new FieldContainerWidget(coinNode, partName, this);
    addTab(nodeContainer, type);
    connect(
        nodeContainer, SIGNAL(valueModified(SoNode*, QString, QString)),
        this, SLOT(SetValue(SoNode*, QString, QString))
    );
}

/*!
 * Returns the names of the parts of the \a coinNode that the view shows.
 *
 * If the \a coinNode is not a container node, return a empty list.
 */
QStringList ParametersTabs::ContainerNodeParts(SoBaseKit* kit)
{
    if (!kit) return {};
    if (!dynamic_cast<SoBaseKit*>(kit)) return {}; //?

    QString type = kit->getTypeId().getName().getString();

    if (type == "TLightKit")
        return {"transform", "icon", "tsunshape"};
    else if (type == "TShapeKit")
        return {"shape", "appearance.material"};
    else if (type == "TAnalyzerKit")
        return {"parameter", "result", "levelList*", "transform"};
    else if (type == "TAnalyzerResultKit")
        return {"result"};
    else
        return {"transform"};
}
