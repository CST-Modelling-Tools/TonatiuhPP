#pragma once

#include <QTabWidget>

class SoBaseKit;
class SoNode;

//! The ParametersView class provides a tabbed view for nodes parameters.
/*!
 * ParametersView provides a page for each of the parts of a node. The tab bar shows the node part name and the tab area shows the part parameters list.
 *
 * The tab area containes a ParametersView.
 *
 * \sa ParametersView
 *
 */

class ParametersTabs: public QTabWidget
{
    Q_OBJECT

public:
    ParametersTabs(QWidget* parent = 0);
    ~ParametersTabs();

    void SelectionChangedToPart(SoNode* coinPart);
    void SelectionChangedToKit(SoBaseKit* coinNode);
    void UpdateView();

signals:
    /*!
     * Emmits a signal when a parameters of the view is modified.
     *
     * The modified paramenters is a \a coinNode paramenter named \a parameterName.
     *
     * The new value for the parameter is \a newValue.
     * */
    void valueModified(SoNode* coinNode, QString parameterName, QString newValue);

public slots:
    void SetValue(SoNode* node, QString parameterName, QString newValue);

private:
    void AddTab(SoNode* coinNode, QString partName);
    QStringList ContainerNodeParts(SoBaseKit* kit);

    SoNode* m_actualCoinNode;
    bool m_isPart;
};
