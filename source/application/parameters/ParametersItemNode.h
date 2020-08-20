#pragma once

#include <QStandardItem>

class SoNode;


class ParametersItemNode: public QStandardItem
{
public:
    ParametersItemNode(SoNode* node);

    SoNode* node() const {return m_node;}

private:
    SoNode* m_node;
};

