#pragma once

#include "kernel/TonatiuhKernel.h"
#include "kernel/scene/TAbstract.h"

#include <qglobal.h>
#include <vector>

//!  RandomDeviate is the base class for random generators.
/*!
   A random generator class can be written based on this class.
 */
class TONATIUH_KERNEL Random
{
public:
    explicit Random(ulong size)
    {
        m_array.resize(size);
        m_index = size;
        m_total = 0;
    }

    virtual ~Random() {}

    double RandomDouble()
    {
        if (m_index >= m_array.size())
        {
            FillArray();
            m_index = 0;
            m_total += m_array.size();
        }
        return m_array[m_index++];
    }

    virtual void FillArray() = 0;

    ulong NumbersGenerated() const {return m_total;}
    ulong NumbersProvided() const {return m_total - m_array.size() + m_index;}

    NAME_ICON_FUNCTIONS("X", ":/RandomX.png")

protected:
    std::vector<double> m_array;
    ulong m_index;
    ulong m_total;
};
