#pragma once

template <class T>
class TPointer
{
	T* pointer;
public:
	TPointer(const TPointer& C) { T = C.pointer; }
	TPointer(T* p) : pointer(p) {}
	void operator=(const TPointer& C) { T = C.pointer; }

	~TPointer(void)
	{
		if (T) delete T;
	}
	T* operator->() const
	{
		return T;
	}
};