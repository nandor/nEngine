/**
	@file GUIMetric.hpp
	@author Licker Nandor

	This file is part of nEngine.
	(c) 2012 Licker Nandor
*/

namespace nEngine {
	
	enum GUIMetricType {
		GUI_METRIC_PIXEL,
		GUI_METRIC_PERCENT
	};

	class GUIMetric {
	public:
		GUIMetric();
		GUIMetric(int pixels);
		GUIMetric(int count, GUIMetricType type);
		GUIMetric(const std::string& string);

		
		GUIMetricType getType() const
		{
			return mType;
		}

		int getMetric(int parent = 100) const;

	private:

		int mCount;
		
		GUIMetricType mType;
	};
};