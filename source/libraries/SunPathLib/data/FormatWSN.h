#pragma once

#include "SunPathLib/samplers/SunSpatial.h"

namespace sp {

// Weighted Sun Nodes


struct SUNPATHLIB ParamsWSN
{
    bool symmetryEW = false; // East-West symmetry
    int precision = 6;
    bool positiveAzimuth = false;
    bool withAmplitudes = false;
};


class SUNPATHLIB FormatWSN
{
public:
    FormatWSN(SunSpatial* sunSpatial);

    bool read(QString fileName, const ParamsWSN& params = ParamsWSN());
    bool write(QString fileName, const ParamsWSN& params = ParamsWSN());
    QString message() const {return m_message;}

protected:
    void readInfo(QTextStream& fin, const ParamsWSN& params);
    void readData(QTextStream& fin, const ParamsWSN& params);
    QString writeInfo(const ParamsWSN& params);
    QString writeData(const ParamsWSN& params);

protected:
    SunSpatial* m_sunSpatial;
    QString m_message;
};


} // namespace sp
