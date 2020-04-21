#pragma once

template <class T>
class Ptr
{
public:
    Ptr(T* realPointer = 0);
    Ptr(const Ptr& rhs);
    ~Ptr();
    Ptr& operator=(const Ptr<T>& rhs);
    Ptr& operator=(const T*& realPointer);
    T* operator->() const;
    T& operator*() const;
    operator bool() const { return m_realPointer != NULL; }
	void ShowCount() const;

private:
	void Init();
	T* m_realPointer;
};

template <class T>
Ptr<T>::Ptr(T* realPointer)
    : m_realPointer(realPointer)
{
	Init();
}

template <class T>
Ptr<T>::Ptr(const Ptr& rhs)
    : m_realPointer(rhs.m_realPointer)
{
	Init();
}

template <class T>
void Ptr<T>::Init()
{
    if (m_realPointer) m_realPointer->Upcount();
	return;
}

template <class T>
Ptr<T>::~Ptr()
{
    if (m_realPointer) m_realPointer->Downcount();
}

template <class T>
Ptr<T>& Ptr<T>::operator=(const Ptr<T>& rhs)
{
    if (m_realPointer != rhs.m_realPointer)
	{
		T* oldRealPointer = m_realPointer;
		m_realPointer = rhs.m_realPointer;
		Init();
        if (oldRealPointer) oldRealPointer->Downcount();
	}
	return *this;
}

template <class T>
Ptr<T>& Ptr<T>::operator=(const T*& realPointer)
{
    if (m_realPointer != realPointer)
	{
		T* oldRealPointer = m_realPointer;
		m_realPointer = realPointer;
		Init();
        if (oldRealPointer) oldRealPointer->Downcount();
	}
	return *this;
}

template <class T>
T* Ptr<T>::operator->() const
{
	return m_realPointer;
}

template <class T>
T& Ptr<T>::operator*() const
{
	return *m_realPointer;
}

template <class T>
void Ptr<T>::ShowCount() const
{
	m_realPointer->PrintCount();
}

