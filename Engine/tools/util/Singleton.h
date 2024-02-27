#pragma once
#include <xcall_once.h>
#include <mutex>


template <typename T>
class Singleton
{
public:
	static T& GetInstance()
	{
		std::call_once(mOnceFlag, []() {
			mInstance.reset(new T());
			});
		return *mInstance;
	}


protected:
	Singleton() = default;
	~Singleton() = default;
	Singleton(const Singleton<T>&) = delete;
	Singleton<T>& operator=(const Singleton<T>&) = delete;

	static std::unique_ptr<T> mInstance;
	static std::once_flag     mOnceFlag;
};

template<typename T>
std::unique_ptr<T> Singleton<T>::mInstance;

template<typename T>
std::once_flag Singleton<T>::mOnceFlag;