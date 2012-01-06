/**
	@file Error.cpp
    @author Licker Nandor

    @brief Provides exceptions and error handling

	This file is part of nEngine.
	(c) 2011 Licker Nandor
*/
#include "nHeaders.hpp"
#include "Error.hpp"

namespace nEngine {
	// ------------------------------------------------------------------
	Error::Error()
		:mErrorMsg("No error"),
		 mId(""),
		 mModule("Error")
	{

	}

	// ------------------------------------------------------------------
	Error::Error(const std::string& msg)
		:mId(""),
		 mErrorMsg(msg),
		 mModule("Error")
	 {

	 }

	// ------------------------------------------------------------------
	Error::Error(const std::string& msg, const std::string& data)
		:mErrorMsg(data),
		 mId(""),
		 mModule(msg)
	{

	}

	// ------------------------------------------------------------------
	Error::Error (const std::string& module, const std::string& id, const std::string& message)
		:mErrorMsg(message),
		 mId(id),
		 mModule(module)
	{

	}

	// ------------------------------------------------------------------
	Error::~Error()
	{

	}

	// ------------------------------------------------------------------
	std::string Error::getMessage()
	{
		if (mId != "") {
			return mModule + " <" + mId + ">: " + mErrorMsg + "\n";
		}

		return mModule + ": " + mErrorMsg + "\n";
	}
}