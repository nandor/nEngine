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
		NAPI static T* __inst;

	public:
		static T& inst() {
			return *((__inst == NULL) ? (__inst = new T()) : (__inst));
		}

		static T* instPtr() {
			return (__inst == NULL) ? (__inst = new T()) : (__inst);
		}

		static void kill() {
			if (__inst != NULL) {
				delete __inst;
			}
		}
	};
};

#endif /* SINGLETON_HPP */