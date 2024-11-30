#pragma once
#include "Compiler/pstdint.h"
#include <functional>

namespace Tools
{
	using ListenerId = uint64_t;

	template <class... ArgTypes>
	class Event
	{
	public:
		using CallBack = std::function<void(ArgTypes...)>;

		ListenerId AddListener(CallBack callBack);

		ListenerId operator +=(CallBack callBack);

		bool RemoveListener(ListenerId listener);

		bool operator -=(ListenerId listener);

		void RemoveAllListeners();

		uint64_t GetListenerCount();

		void Invoke(ArgTypes... args);

	private:

		std::unordered_map<ListenerId, CallBack> mCallBacks;
		ListenerId                               mAvailableListenerId = 0;

	};

	template<class ...ArgTypes>
	inline ListenerId Event<ArgTypes...>::AddListener(CallBack callBack)
	{
		ListenerId listenerId = mAvailableListenerId++;
		mCallBacks.emplace(listenerId, callBack);
		return listenerId;
	}

	template<class ...ArgTypes>
	inline ListenerId Event<ArgTypes...>::operator+=(CallBack callBack)
	{
		return AddListener(callBack);
	}

	template<class ...ArgTypes>
	inline bool Event<ArgTypes...>::RemoveListener(ListenerId listener)
	{
		return mCallBacks.erase(listener) != 0;
	}

	template<class ...ArgTypes>
	inline bool Event<ArgTypes...>::operator-=(ListenerId listener)
	{
		return RemoveListener(listener);
	}

	template<class ...ArgTypes>
	inline void Event<ArgTypes...>::RemoveAllListeners()
	{
		mCallBacks.clear();
	}

	template<class ...ArgTypes>
	inline uint64_t Event<ArgTypes...>::GetListenerCount()
	{
		return uint64_t(mCallBacks.size());
	}

	template<class ...ArgTypes>
	inline void Event<ArgTypes...>::Invoke(ArgTypes ...args)
	{
		for (auto const& [key, value] : mCallBacks)
			value(args...);
	}
}