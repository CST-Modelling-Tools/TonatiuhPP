#include "ModelTMY.h"

#include "ParameterItem.h"

ModelTMY::ModelTMY(QObject* parent):
    QAbstractTableModel(parent)
{
    m_formatTMY = new sp::ReaderTMY();
}

ModelTMY::~ModelTMY()
{
    delete m_formatTMY;
}

void ModelTMY::setDatesAll()
{
    beginResetModel();
    m_filter.indexA = 0;
    m_filter.indexB = m_formatTMY->data().size();
    endResetModel();
}

void ModelTMY::setDates(QDate dateA, QDate dateB)
{
    int tStep = m_formatTMY->paramEffective()->parameterDouble("Time.Step")->value();
    int tOrigin = m_formatTMY->paramEffective()->parameterDouble("Time.Origin")->value()*tStep;

    beginResetModel();
    m_filter.dateA = dateA;
    m_filter.stampA = m_formatTMY->findStamp(dateA);
    m_filter.indexA = m_formatTMY->findIndexStamp(m_filter.stampA - tOrigin);

    m_filter.dateB = dateB;
//    int stampB = stampA + dateA.daysTo(dateB)*24*3600;
//    m_filter.indexB = m_formatTMY->findIndexStamp(stampB);
    m_filter.indexB =  m_filter.indexA + dateA.daysTo(dateB)*24*3600/tStep;
    m_filter.stampB = m_filter.stampA  + dateA.daysTo(dateB)*24*3600;
    endResetModel();
}

QVariant ModelTMY::headerData(int section, Qt::Orientation orientation, int role) const
{
    if (orientation == Qt::Horizontal) {
        if (role == Qt::DisplayRole)
            return m_formatTMY->recordsInfo().names[section];
        else if (role == Qt::TextAlignmentRole)
            return Qt::AlignRight;
    }
    else if (orientation == Qt::Vertical) {
        if (role == Qt::DisplayRole)
            return section + 1;
        else if (role == Qt::TextAlignmentRole)
            return Qt::AlignRight;
    }
    return QVariant();
}

int ModelTMY::rowCount(const QModelIndex& parent) const
{
    Q_UNUSED(parent);
    return m_filter.indexB - m_filter.indexA;
}

int ModelTMY::columnCount(const QModelIndex& parent) const
{
    Q_UNUSED(parent);
    return m_formatTMY->recordsInfo().names.size();
}

QVariant ModelTMY::data(const QModelIndex& index, int role) const
{
    if (!index.isValid())
        return QVariant();

    if (role == Qt::DisplayRole) {
        int r = index.row();
        int c = index.column();
        int n = m_filter.indexA + r;
        if (n >= m_formatTMY->data().size()) n -= m_formatTMY->data().size();
        return m_formatTMY->data()[n].data.section(',', c, c);
    } else if (role == Qt::TextAlignmentRole) {
        return Qt::AlignRight;
    }

    return QVariant();
}
