#pragma once

#include "libraries/TonatiuhLibraries.h"


class TONATIUH_LIBRARIES RefCount
{
public:
    RefCount(): m_count(0) {}
    RefCount(const RefCount&): m_count(0) {}
    RefCount& operator=(const RefCount&) {return *this;}
    virtual ~RefCount() {}

    void Upcount() {++m_count;}
    void Downcount() {if (--m_count == 0) delete this;}
    ulong GetCount() const {return m_count;}

private:
    ulong m_count;
};
