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

    void SelectionChanged(SoNode* node);
    void UpdateView();

signals:
    /*!
     * Emmits a signal when a parameters of the view is modified.
     *
     * The modified paramenters is a \a coinNode paramenter named \a parameterName.
     *
     * The new value for the parameter is \a newValue.
     * */
    void valueModified(SoNode* node, QString parameter, QString value);

public slots:
    void SetValue(SoNode* node, QString parameter, QString value);

private:
    void AddTab(SoNode* node, QString partName);
    QStringList ContainerNodeParts(SoBaseKit* kit);

    SoNode* m_node;
};
