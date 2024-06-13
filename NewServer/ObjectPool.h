#pragma once

template<typename T>
class ObjectPool
{
public:
	enum
	{
		POOL_MAX_SIZE = 8,
		POOL_SIZE_MASK = POOL_MAX_SIZE - 1,
	};

public:
	static void* operator new(size_t size)
	{
		long long pos = InterlockedIncrement64(&m_HeadPos) - 1;
		long long insert = pos & POOL_SIZE_MASK;
		void* value = InterlockedExchangePointer(&m_Pool[insert], nullptr);

		if (value != nullptr)
		{
			return value;
		}

		return _aligned_malloc(size, MEMORY_ALLOCATION_ALIGNMENT);

	}
	static void operator delete(void* obj)
	{

		long long pos = InterlockedIncrement64(&m_TailPos) - 1;
		long long insert = pos & POOL_SIZE_MASK;
		void* value = InterlockedExchangePointer(&m_Pool[insert], obj);
		if (value != nullptr)
		{
			_aligned_free(value);
		}

	}
	static void AllFree()
	{

		for (int isize = 0; isize < POOL_MAX_SIZE; ++isize)
		{
			void* value = InterlockedExchangePointer(&m_Pool[isize], nullptr);
			if (value != nullptr)
			{
				_aligned_free(value);
			}
		}

	}
private:
	static void* volatile m_Pool[POOL_MAX_SIZE];
	static long long volatile m_HeadPos;
	static long long volatile m_TailPos;
};
template<typename T>
void* volatile ObjectPool<T>::m_Pool[POOL_MAX_SIZE] = {};
template<typename T>
long long volatile ObjectPool<T>::m_HeadPos(0);
template<typename T>
long long volatile ObjectPool<T>::m_TailPos(0);
