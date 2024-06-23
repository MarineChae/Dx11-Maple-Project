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
	//오브젝트 풀링을 위한 연산자 오버로딩
	
	static void* operator new(size_t size)
	{	
		//여러 스레드에서 접근하는것을 방지하기 위해 사용하는 매크로임 
		//원자적 동작은 경쟁 상태(race condition)를 방지
		long long pos = InterlockedIncrement64(&m_HeadPos) - 1;
		//and 연산을 사용하여 현재 헤드의 위치값을 가져옴
		long long insert = pos & POOL_SIZE_MASK;
		// insert = 0 & 7
		// insert = 0000 0000 & 0000 0111
		// insert = 0000 0000 (결과: 0)
		void* value = InterlockedExchangePointer(&m_pool[insert], nullptr);
		
		if (value != nullptr)
		{
			return value;
		}

		//16바이트 정렬방식으로 메모리를 할당
		return _aligned_malloc(size, MEMORY_ALLOCATION_ALIGNMENT);
	
	}

	static void operator delete(void* obj)
	{
		long long pos = InterlockedIncrement64(&m_TailPos) - 1;
		long long insert = pos & POOL_SIZE_MASK;
		void* value = InterlockedExchangePointer(&m_pool[insert], obj);
		if (value != nullptr)
		{
			_aligned_free(value);
		}
	
	}
	static void AllFree()
	{
		for (int iSize = 0; iSize < POOL_MAX_SIZE; ++iSize)
		{
			void* value = InterlockedExchangePointer(m_pool[iSize], nullptr);
			if (value != nullptr)
			{
				_aligned_free(value);
			}
		}
	}


private:
	static void* volatile m_pool[POOL_MAX_SIZE];
	static long long volatile m_HeadPos;
	static long long volatile m_TailPos;
};

//초기화
template <typename T>
void* volatile ObjectPool<T>::m_pool[POOL_MAX_SIZE] = {};

template <typename T>
long long volatile ObjectPool<T>::m_HeadPos(0);

template <typename T>
long long volatile ObjectPool<T>::m_TailPos(0);
