#pragma once

#include <QTextStream>
#include <QString>
#include <QDateTime>

#include <QStandardItemModel>
#include "ParameterModel.h"
class TreeItem;

namespace sp {


struct RecordTMY
{
    QDateTime time; // local, but saved as UTC0
    int stamp; // timestamp in seconds from year start
    double DNI;
    QString data;
};

struct RecordsInfo
{
    QStringList names; // line 3
    double min;
    double max;
    double total;
    double below;
};

struct RecordsFilter
{
    RecordsFilter():
        indexA{0},
        indexB{0}
    {

    }

    QDate dateA;
    int indexA;
    int stampA;
    QDate dateB;
    int indexB;
    int stampB;
};

class ReaderTMY
{
public:
    ReaderTMY();
    ~ReaderTMY();

    void setModels(ParameterModel* paramsOriginal, ParameterModel* paramsEffective);
    ParameterModel* paramEffective() {return m_paramsEffective;}

    bool read(QString fileName);
    QString message() const {return m_message;}

    RecordsInfo& recordsInfo() {return m_recordsInfo;}
    QVector<RecordTMY>& data() {return m_data;}
    int findIndex(QDate date);
    int findIndexStamp(int stamp);
    int findStamp(QDate date);
    void setDNI(QString name);

protected:
    void readInfo(QTextStream& fin);
    void readData(QTextStream& fin);
    double calculateDNIbelow(double origin);
    bool isTimeSorted();

protected:
    ParameterModel* m_paramsOriginal;
    ParameterModel* m_paramsEffective;

    QString m_message;
    QStringList m_infoNames; // line 1
    QStringList m_info; // line 2

    RecordsInfo m_recordsInfo;
    QVector<RecordTMY> m_data; // lines 4..
};


} // namespace sp
