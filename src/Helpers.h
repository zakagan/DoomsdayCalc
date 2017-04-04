#ifndef _HELPERS_H_
#define _HELPERS_H_

#include <algorithm>
#include <regex>
#include <string>
#include "DoomsdayDate.h"

namespace Helpers
{
	bool parseDate(DoomsdayDate* dd_ptr, std::string date_str, bool ad, const int european_flag);
	bool parseHoliday(DoomsdayDate* dd_ptr, const std::string holiday_str, const int year, const bool ad, bool& all_h_bool);
	struct CompareDates
	{
		bool operator()(const DoomsdayDate* lhs, const DoomsdayDate* rhs)
		{
			return !(*lhs < *rhs);
		}
	};
	void usageStatement(std::string name);
};

#endif