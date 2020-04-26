#pragma once

#include "libraries/TonatiuhLibraries.h"

class TONATIUH_LIBRARIES RefCount
{
public:
    RefCount();
    RefCount(const RefCount& rhs);
    RefCount& operator=(const RefCount& rhs);
    virtual ~RefCount() = 0;
    void Upcount();
    void Downcount();
    unsigned long int GetCount() const;

private:
    unsigned long int m_refCount;
};




