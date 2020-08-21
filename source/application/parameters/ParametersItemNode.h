#pragma once

#include <QStandardItem>

class SoNode;


class ParametersItemNode: public QStandardItem
{
public:
    ParametersItemNode(QString part, SoNode* node);

    SoNode* node() const {return m_node;}
    void setNode(SoNode* node);

private:
    SoNode* m_node;
};
