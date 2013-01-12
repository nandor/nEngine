/**
	@file Error.hpp
	@author Licker Nandor

    @brief Provides exceptions and error handling

	This file is part of nEngine.
	(c) 2011 Licker Nandor
*/

#ifndef ERROR_HPP
#define ERROR_HPP

#include "types.hpp"

namespace nEngine {
	class Error {
	public:
		NAPI Error();
		NAPI Error(const std::string& error);
		NAPI Error(const std::string& module, const std::string& error);
		NAPI Error(const std::string& module, const std::string& id, const std::string& error);
		NAPI ~Error();

		NAPI std::string getMessage();
	private:
		std::string mErrorMsg;
		std::string mModule;
		std::string mId;
	};
};

#endif /*ERROR_HPP*/
