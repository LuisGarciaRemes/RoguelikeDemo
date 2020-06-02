#pragma once
#include <cstdint>
#include <assert.h> 

struct RefCounter
{
	uint32_t smartCount;
	uint32_t weakCount;

	RefCounter(uint32_t i_Smartcount, uint32_t i_WeakCount) :
		smartCount(i_Smartcount), 
		weakCount(i_WeakCount)
	{}
};

template <class T>
class WeakPointer;

template<class T>
class SmartPointer
{
	template<class U>
	friend class WeakPointer;

	template<class U>
	friend class SmartPointer;

	T* pointer;
	RefCounter* refCount;

public:


	void reduceCount()
	{
		if (refCount != nullptr)
		{
			if (--refCount->smartCount == 0)
			{
				delete pointer;
				if (refCount->weakCount == 0)
					delete refCount;
			}
		}
	}

	SmartPointer()
	{
		pointer = nullptr;
		refCount = nullptr;
	}

	~SmartPointer()
	{
		reduceCount();
		pointer = nullptr;
		refCount = nullptr;
	}

	SmartPointer(T * ptr) :
		pointer(ptr),
		refCount(new RefCounter(1, 0))
	{}

	SmartPointer(const SmartPointer& other) :
		pointer(other.pointer),
		refCount(other.refCount)
	{
		assert(refCount);
		refCount->smartCount++;
	}

	template<class U>
	SmartPointer(const SmartPointer<U>& other):
		pointer(other.pointer),
		refCount(other.refCount)
	{
		assert(refCount);
		refCount->smartCount++;
	}

	SmartPointer(const WeakPointer<T> & other)
	{
		if (other.refCount->smartCount > 0)
		{
			pointer = other.pointer;
			refCount = other.refCount;
			refCount->smartCount++;
		}
		else
		{
			pointer = nullptr;
			refCount = nullptr;
		}
	}

	template<class U>
	SmartPointer(const WeakPointer<U> & other)
	{
		if (other.refCount->smartCount > 0)
		{
			pointer = other.pointer;
			refCount = other.refCount;
			refCount->smartCount++;
		}
		else
		{
			pointer = nullptr;
			refCount = nullptr;
		}
	}

	SmartPointer & operator=(std::nullptr_t nullp) const
	{
		reduceCount();
		pointer = nullptr;
		refCount = nullptr;
	}

	SmartPointer & operator=(const SmartPointer & other)
	{
		if (other.pointer == pointer)
			return *this;
		if (other.pointer == nullptr)
		{
			reduceCount();
			pointer = nullptr;
			pointer = nullptr;
			return *this;
		}
		reduceCount();
		pointer = other.pointer;
		refCount = other.refCount;
		refCount->smartCount++;
		return *this;
	}

	template<class U>
	SmartPointer & operator=(const SmartPointer<U> & other)
	{
		if (other.pointer == pointer)
			return *this;
		if (other.pointer == nullptr)
		{
			reduceCount();
			pointer = nullptr;
			refCount = nullptr;
			return *this;
		}
		reduceCount();
		pointer = other.pointer;
		refCount = other.refCount;
		refCount->smartCount++;
		return *this;
	}

	SmartPointer & operator=(const WeakPointer<T> & other)
	{
		if (other.pointer == pointer)
			return *this;
		if (other.pointer == nullptr)
		{
			reduceCount();
			pointer = nullptr;
			pointer = nullptr;
			return *this;
		}
		reduceCount();
		pointer = other.pointer;
		refCount = other.refCount;
		refCount->smartCount++;
		return *this;
	}

	template<class U>
	SmartPointer & operator=(const WeakPointer<U> & other)
	{
		if (other.pointer == pointer)
			return *this;
		if (other.pointer == nullptr)
		{
			reduceCount();
			pointer = nullptr;
			pointer = nullptr;
			return *this;
		}
		reduceCount();
		pointer = other.pointer;
		refCount = other.refCount;
		refCount->smartCount++;
		return *this;
	}

SmartPointer & operator=(T * ptr)
	{
		if (pointer == ptr)
			return *this;

		reduceCount();
		pointer = ptr;

		if (ptr == nullptr)
			refCount = nullptr;
		else
			refCount = new RefCounter(1, 0);
		return *this;
	}

	inline bool operator==(const SmartPointer<T> & other) const
	{
		return pointer == other.pointer;
	}

	template<class U>
	inline bool operator==(const SmartPointer<U> & other) const;

	inline bool operator==(const WeakPointer<T> & other) const
	{
		return pointer == other.pointer;
	}

	template<class U>
	inline bool operator==(const WeakPointer<U> & other) const
	{
		return pointer == other.pointer;
	}

	template<class U>
	inline bool operator!=(const SmartPointer<U> & other) const
	{
		return pointer != other.pointer;
	}

	inline bool operator!=(const WeakPointer<T> & other) const
	{
		return pointer != other.pointer;
	}

	template<class U>
	inline bool operator!=(const WeakPointer<U> & other) const;

	inline bool operator==(T * ptr) const
	{
		return pointer == ptr;
	}

	template<class U>
	inline bool operator==(U * ptr) const
	{
		return pointer == ptr;
	}

	inline bool operator==(std::nullptr_t nullp) const
	{
		return pointer == nullptr;
	}

	inline bool operator!=(T * ptr) const
	{
		return pointer != ptr;
	}

	template<class U>
	inline bool operator!=(U * ptr) const
	{
		return pointer != ptr;
	}

	inline operator bool() const
	{
		return pointer != nullptr;
	}

	T * operator->()
	{
		return pointer;
	}

	T & operator*()
	{
		if (pointer)
			return *pointer;
	}

};

template<class T>
class WeakPointer
{

	template<class U>
	friend class SmartPointer;

	template<class U>
	friend class WeakPointer;

	T* pointer;
	RefCounter* refCount;

public:
	WeakPointer():
		pointer(nullptr),
		refCount(nullptr)
	{}

	~WeakPointer()
	{
		if (refCount != nullptr)
		{
			if (--refCount->weakCount == 0 && refCount->smartCount == 0)
			{
				delete refCount;
				refCount = nullptr;
			}
		}
	}

	WeakPointer(const WeakPointer & owner):
		pointer(owner.pointer),
		refCount(owner.refCount)
	{
		assert(refCount);
		refCount->weakCount++;
	}

	template<class U>
	WeakPointer(const SmartPointer<U> & owner):
		pointer(owner.pointer),
			refCount(owner.refCount)
		{
			assert(refCount);
			refCount->weakCount++;
		}

	template<class U>
	WeakPointer(const WeakPointer<U> & owner) :
		pointer(owner.pointer),
		refCount(owner.refCount)
	{
		assert(refCount);
		refCount->weakCount++;
	}

	WeakPointer & operator=(const WeakPointer & other)
	{
		if (other->pointer == pointer)
			return *this;
		pointer = pointer;
		refCount = other.refCount;
		refCount->weakCount++;
		return *this;
	}

	template<class U>
	WeakPointer & operator=(const WeakPointer<U> & other)
	{
		if (other->pointer == pointer)
			return *this;
		pointer = pointer;
		refCount = other.refCount;
		refCount->weakCount++;
		return *this;
	}

	template<class U>
	WeakPointer & operator=(const SmartPointer<U> & other)
	{
		if (refCount != nullptr)
			refCount->weakCount--;
		refCount = other.refCount;
		refCount->weakCount++;
		pointer = other.pointer;
		return *this;
	}

	

	WeakPointer<T> & operator=(std::nullptr_t nullp)
	{
		if (refCount != nullptr)
			refCount->weakCount--;
		refCount = nullptr;
		pointer = nullptr;
		return *this;
	}

	inline SmartPointer<T> CreateSmartPointer() const
	{
		return SmartPointer<T>(*this);
	}

	inline bool operator==(const SmartPointer<T> & other) const
	{
		return pointer == other.pointer;
	}

	template<class U>
	inline bool operator==(const SmartPointer<U> & other) const
	{
		return pointer == other.pointer;
	}

	inline bool operator==(const WeakPointer<T> & other) const
	{
		return pointer == other.pointer;
	}

	template<class U>
	inline bool operator==(const WeakPointer<U> & other) const
	{
		return pointer == other.pointer;
	}

	inline bool operator==(T * ptr) const
	{
		return pointer == ptr;
	}

	template<class U>
	inline bool operator==(U * ptr) const
	{
		return pointer == ptr;
	}

	inline bool operator!=(const SmartPointer<T> & other) const
	{
		return pointer != other.pointer;
	}

	template<class U>
	inline bool operator!=(const SmartPointer<U> & other) const
	{
		return pointer != other.pointer;
	}

	inline bool operator!=(const WeakPointer<T> & other) const
	{
		return pointer != other.pointer;
	}

	template<class U>
	inline bool operator!=(const WeakPointer<U> & other) const
	{
		return pointer != other.pointer;
	}

	inline bool operator!=(T * ptr) const
	{
		return pointer != ptr;

	}

	template<class U>
	inline bool operator!=(U * ptr) const
	{
		return pointer != ptr;
	}

	inline operator bool() const
	{
		return pointer != nullptr;
	}
};



