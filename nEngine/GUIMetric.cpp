/**
	@file GUIMetric.cpp
	@author Licker Nandor

	This file is part of nEngine.
	(c) 2012 Licker Nandor
*/

#include "nHeaders.hpp"
#include "Error.hpp"
#include "GUIMetric.hpp"

namespace nEngine {
	std::regex gMetricRegex("(\\+|\\-)?[0-9]+(%|px)");
	
	// ------------------------------------------------------------------
	GUIMetric::GUIMetric()
	{
		mCount = 20;
		mType = GUI_METRIC_PIXEL;
	}

	// ------------------------------------------------------------------
	GUIMetric::GUIMetric(int pixels)
	{
		mType = GUI_METRIC_PIXEL;
		mCount = pixels;
	}

	// ------------------------------------------------------------------
	GUIMetric::GUIMetric(int count, GUIMetricType type)
	{
		mType = type;
		mCount = count;
	}

	// ------------------------------------------------------------------
	GUIMetric::GUIMetric(const std::string& str)
	{
		if (!std::regex_match(str, gMetricRegex)) {
			throw Error("GUIMetric", "Invalid metric: " + str);
		}

		std::string type;
		std::stringstream ss(str);

		ss >> mCount >> type; 

		mType = (type == "px") ? GUI_METRIC_PIXEL : GUI_METRIC_PERCENT;
	}

	// ------------------------------------------------------------------
	int GUIMetric::getMetric(int parent) const
	{
		return (mType == GUI_METRIC_PIXEL) ? mCount : (parent * mCount / 100);
	}
};