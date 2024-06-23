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
	//������Ʈ Ǯ���� ���� ������ �����ε�
	
	static void* operator new(size_t size)
	{	
		//���� �����忡�� �����ϴ°��� �����ϱ� ���� ����ϴ� ��ũ���� 
		//������ ������ ���� ����(race condition)�� ����
		long long pos = InterlockedIncrement64(&m_HeadPos) - 1;
		//and ������ ����Ͽ� ���� ����� ��ġ���� ������
		long long insert = pos & POOL_SIZE_MASK;
		// insert = 0 & 7
		// insert = 0000 0000 & 0000 0111
		// insert = 0000 0000 (���: 0)
		void* value = InterlockedExchangePointer(&m_pool[insert], nullptr);
		
		if (value != nullptr)
		{
			return value;
		}

		//16����Ʈ ���Ĺ������ �޸𸮸� �Ҵ�
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

//�ʱ�ȭ
template <typename T>
void* volatile ObjectPool<T>::m_pool[POOL_MAX_SIZE] = {};

template <typename T>
long long volatile ObjectPool<T>::m_HeadPos(0);

template <typename T>
long long volatile ObjectPool<T>::m_TailPos(0);
