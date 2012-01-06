/**
	@file Console.hpp
	@author Licker Nandor

	This file is part of nEngine.
	(c) 2011 Licker Nandor
*/

#ifndef SINGLETON_HPP
#define SINGLETON_HPP

#include "types.hpp"

namespace nEngine {
	template<typename T> class Singleton {
	private:
	
		NAPI Singleton(const Singleton<T>&);
		NAPI Singleton& operator= (const Singleton<T>&);

	protected:
		Singleton() {}
		NAPI static T* mInstance;

	public:
		static T& inst() {
			return *(mInstance ? mInstance : (mInstance = new T()));
		}
		
		static T* instPtr()
		{
			return mInstance ? mInstance : (mInstance = new T());
		}
	};
};

#endif /* SINGLETON_HPP */