#pragma once


template <typename T>
class Singleton
{
public:
	static T& GetInstance()
	{
		static T instance;

		return instance;
	}

public:
	virtual ~Singleton<T>();


};

template<typename T>
inline Singleton<T>::~Singleton()
{
}
