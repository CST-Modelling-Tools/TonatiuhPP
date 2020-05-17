#pragma once

template <class T>
class Ptr
{
public:
    Ptr(T* realPointer = 0): m_pointer(realPointer)
    {Init();}

    Ptr(const Ptr& rhs): m_pointer(rhs.m_pointer)
    {Init();}

    ~Ptr() {
        if (m_pointer) m_pointer->Downcount();
    }

    Ptr& operator=(const Ptr<T>& rhs);
    Ptr& operator=(const T*& realPointer);

    T* operator->() const {return m_pointer;}
    T& operator*() const {return *m_pointer;}

    operator bool() const {return m_pointer;}
//    void ShowCount() const {m_realPointer->PrintCount();}

private:
    void Init() {
        if (m_pointer) m_pointer->Upcount();
    }

    T* m_pointer;
};


template <class T>
Ptr<T>& Ptr<T>::operator=(const Ptr<T>& rhs)
{
    if (m_pointer != rhs.m_pointer)
    {
        T* old = m_pointer;
        m_pointer = rhs.m_pointer;
        Init();
        if (old) old->Downcount();
    }
    return *this;
}

template <class T>
Ptr<T>& Ptr<T>::operator=(const T*& realPointer)
{
    if (m_pointer != realPointer)
    {
        T* old = m_pointer;
        m_pointer = realPointer;
        Init();
        if (old) old->Downcount();
    }
    return *this;
}
