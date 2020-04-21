#include "RefCount.h"

RefCount::RefCount()
    : m_refCount(0)
{
}

RefCount::RefCount(const RefCount&)
    : m_refCount(0)
{
}

RefCount& RefCount::operator=(const RefCount&)
{
    return *this;
}

RefCount::~RefCount()
{
}

void RefCount::Upcount()
{
    ++m_refCount;
}

void RefCount::Downcount()
{
    if (--m_refCount == 0) delete this;
}

unsigned long int RefCount::GetCount() const
{
    return m_refCount;
}
