#include "Helpers.h"

namespace Helpers
{

bool parseDate(DoomsdayDate* dd_ptr, std::string date_str, bool ad, const int european_flag)
{
	const std::regex r("[0-9]+");
	std::sregex_iterator Next(date_str.begin(), date_str.end(), r);
	std::sregex_iterator End;
	int indx=0, date_vals[3];

	while (Next != End && indx < 3) 
	{
		try 
		{
			date_vals[indx]=std::stoi(*Next->begin(), nullptr, 10);
		}
		catch (const std::invalid_argument& er)
		{
			std::cerr << "Partial date value " << *Next->begin() << " could not be interpreted" << std::endl;
			return  false;
		}
		catch (std::exception& err)
		{
			std::cerr << "Unexpected error: " << err.what() << std::endl;
			return false;
		}
		++Next;
		++indx;
	}
	if (indx < 3) {
		std::cerr << "Formatted dates require 3 seperate parts: day, month, and year." << std::endl;
		// usage
		return false;
	}

	if (Next != End)
	{
		std::cerr << "Warning: date values after " << *Next->begin() << " have been ignored." << std::endl;
		// usage
	}

	if (european_flag)
	{
		// European date system

		dd_ptr->Set(date_vals[1], date_vals[0], date_vals[2], ad);
		return dd_ptr->Validity();

	} 
	else
	{
		// American date system
		dd_ptr->Set(date_vals[0], date_vals[1], date_vals[2], ad);
		return dd_ptr->Validity();
	}
}

bool parseHoliday(DoomsdayDate* dd_ptr, const std::string holiday_str, const int year, const bool ad, bool& all_h_bool)
{
	
	std::string copy_str;
	const std::regex r("[^_[:alnum:]]");

	copy_str = std::regex_replace(holiday_str, r, "");
	std::transform(copy_str.begin(), copy_str.end(), copy_str.begin(), ::tolower);

	if (!copy_str.compare("all"))
	{
		all_h_bool = true;
		return true;
	}
	else if (!copy_str.compare("nyd") || copy_str.find("newyears") != std::string::npos)
	{
		return dd_ptr->SetNewYears(year, ad);
	}
	else if (!copy_str.compare("mlk") || copy_str.find("martinlutherking") != std::string::npos)
	{
		return dd_ptr->SetMartinLutherKingJr(year, ad);
	}
	else if (!copy_str.compare("ghd") || copy_str.find("groundhog") != std::string::npos)
	{
		return dd_ptr->SetGroundHogDay(year, ad);
	}
	else if (!copy_str.compare("vld") || copy_str.find("valentine") != std::string::npos)
	{
		return dd_ptr->SetValentinesDay(year, ad);
	}
	else if (!copy_str.compare("wbd") || copy_str.find("washington")!= std::string::npos \
		|| copy_str.find("president")!= std::string::npos || copy_str.find("lincoln") != std::string::npos)
	{
		return dd_ptr->SetWashingtonsBirthday(year, ad);
	}
	else if (!copy_str.compare("lpd") || copy_str.find("leap")!= std::string::npos)
	{
		return dd_ptr->SetLeapDay(year, ad);
	}
	else if (!copy_str.compare("gfd") || copy_str.find("goodfriday")!= std::string::npos)
	{
		return dd_ptr->SetGoodFriday(year, ad);
	}
	else if (!copy_str.compare("est") || copy_str.find("easter") != std::string::npos)
	{
		return dd_ptr->SetEaster(year, ad);
	}
	else if (!copy_str.compare("psv") || copy_str.find("passover") != std::string::npos)
	{
		return dd_ptr->SetPassover(year, ad);
	}
	else if (!copy_str.compare("mrd") || copy_str.find("memorial") != std::string::npos)
	{
		return dd_ptr->SetMemorialDay(year, ad);
	}
	else if (!copy_str.compare("ipd") || copy_str.find("independence") != std::string::npos)
	{
		return dd_ptr->SetIndependenceDay(year, ad);
	}
	else if (!copy_str.compare("lbd") || copy_str.find("labor") != std::string::npos)
	{
		return dd_ptr->SetLaborDay(year, ad);
	}
	else if (!copy_str.compare("rsh") || copy_str.find("rosh") != std::string::npos \
		||  copy_str.find("hashanah") != std::string::npos )
	{
		return dd_ptr->SetRoshHashanah(year, ad);
	}
	else if (!copy_str.compare("ykp") || copy_str.find("yom") != std::string::npos \
		||  copy_str.find("kippur") != std::string::npos )
	{
		return dd_ptr->SetYomKippur(year, ad);
	}
	else if (!copy_str.compare("cbd") || copy_str.find("columbus") != std::string::npos)
	{
		return dd_ptr->SetColumbusDay(year, ad);
	}
	else if (!copy_str.compare("vtd") || copy_str.find("veteran") != std::string::npos)
	{
		return dd_ptr->SetVeteransDay(year, ad);
	}
	else if (!copy_str.compare("hlw") || copy_str.find("halloween") != std::string::npos)
	{
		return dd_ptr->SetHalloween(year, ad);
	}
	else if (!copy_str.compare("eld") || copy_str.find("election") != std::string::npos)
	{
		return dd_ptr->SetElectionDay(year, ad);
	}
	else if (!copy_str.compare("tgd") || copy_str.find("thanksgiving") != std::string::npos)
	{
		return dd_ptr->SetThanksgiving(year, ad);
	}
	else if (!copy_str.compare("chr") || copy_str.find("christmas") != std::string::npos)
	{
		return dd_ptr->SetChristmasDay(year, ad);
	}
	else
	{
		std::cerr << "Warning: could not recognize holiday string: " << holiday_str << std::endl;
		
		return false;
	}
}
	void usageStatement(std::string name)
	{
		std::cout << "usage: ./" << name << " {-D date | -y year -m month -d day | -y year -h holiday} [-a | -b] ... [-e | -r] [-s separator]" << std::endl;
		std::cout << "    -D/--Date: formatted date string e.g. 7/20/1969" << std::endl;
		std::cout << "    -y/--year, -m/--month, -d/--day: segmented numerical portions of a date" << std::endl;
		std::cout << "    -h/--holiday: when paired with a year, gives the holiday date for that year" << std::endl;
		std::cout << "    -a/--AD/--CE | -b/--BC/--BCE: sets the date provided as either AD or BC (AD is default)" << std::endl;
		std::cout << "    -e/--European | -r/--American: formats reported dates using either American or European standards (American is default)" << std::endl;
		std::cout << "    -s/separator: sets the seperator between formatted dates (/ is default as in 9/13/1959)" << std::endl;
	}
}
