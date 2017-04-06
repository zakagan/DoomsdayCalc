#include <stdlib.h>
#include <iostream> 
#include <string>
#include <queue>
#include <getopt.h>
#include "DoomsdayDate.h"
#include "Helpers.h"

int main (int argc, char **argv)
{
	int c;
	int option_index = 0;
	int temp;
	char* endptr;
	char* separator = NULL;

	int european_flag     = 0;
	int warning_flag      = 0;
	bool all_h_bool       = false;

	std::string holiday_temp;
	DoomsdayDate* dd_ptr;
	std::queue<int> Format_q;      // 1 is formatted, 2 is segmented, 3 is holiday
	std::queue<int> dayofmonth_q;
	std::queue<int> month_q;
	std::queue<int> year_q;
	std::queue<bool> ad_q;
	std::queue<std::string> holiday_q;
	std::queue<std::string> raw_date_q;
	std::priority_queue<DoomsdayDate*, std::vector<DoomsdayDate*>, Helpers::CompareDates> dd_q;
	static const struct option long_options[] =
	{
		/* These options set a flag. */
		{"European",  no_argument,		&european_flag,    1 },
		{"American",  no_argument,		&european_flag,    0 },	
		/* These options are distinguished directly by their indices. */
		{"AD",        no_argument,		      0,      'a'},
		{"BC",        no_argument,		      0,      'b'},
		{"BCE",       no_argument,		      0,      'b'},
		{"CE",        no_argument,		      0,      'a'},	
		{"Date",      required_argument,      0,      'D'},
		{"day",       required_argument,      0,      'd'},
		{"month",     required_argument,      0,      'm'},
		{"year",      required_argument,      0,      'y'},
		{"holiday",   required_argument,      0,      'h'},
		{"separator", required_argument,      0,      's'},		
		{0, 0, 0, 0}
	};

	while((c=getopt_long(argc, argv, ":D:abcd:eh:lm:rs:vy:", long_options, &option_index)) != -1)
	{
		switch (c) {
			case 0:
				// case of a flag modifying option, in this case do nothing else now.
				if (long_options[option_index].flag != 0)
					break;
				//used for reporting options that may have been improperly interpreted as modifying a flag
				std::cerr << "Error: option " << long_options[option_index].name;
				if (optarg)
					std::cerr << " with arg " << optarg;
				std::cerr << " failed to modify a flag value" << std::endl;
				Helpers::usageStatement(argv[0]);
				return EXIT_FAILURE;

				break;
			case 'D':
				raw_date_q.push(std::string(optarg));
				ad_q.push(true);
				Format_q.push(1);
				break;
			case 'a':
				// default is common era
				break;
			case 'b':
			case 'c':
				if (!ad_q.empty())
				{
					ad_q.pop();      // remove true value
					ad_q.push(false);
				}
				break;
			case 'd':
				endptr=NULL;
				temp=strtol(optarg, &endptr, 10);
				if (*endptr) 
				{
					std::cerr << "Error: day argument (--day/-d) " << optarg << " could not be parsed" << std::endl;
					Helpers::usageStatement(argv[0]);
					return EXIT_FAILURE;
				}
				dayofmonth_q.push(temp);
				ad_q.push(true);
				Format_q.push(2);
				break;
			case 'e':
				european_flag = 1;
				break;
			case 'h':
				holiday_temp = std::string(optarg);
				holiday_q.push(holiday_temp);
				ad_q.push(true);
				Format_q.push(3);
				break;
			case 'm':
				endptr=NULL;
				temp=strtol(optarg, &endptr, 10);
				if (*endptr) 
				{
					std::cerr << "Error: month argument (--month/-m) " << optarg << " could not be parsed" << std::endl;
					Helpers::usageStatement(argv[0]);
					return EXIT_FAILURE;
				}
				month_q.push(temp);
				break;
			case 'r':
				european_flag = 0;
				break;
			case 's':
				separator = optarg;
				break;
			case 'y':
				endptr=NULL;
				temp=strtol(optarg, &endptr, 10);
				if (*endptr) 
				{
					std::cerr << "Error: year argument (--year/-y) " << optarg << " could not be parsed" << std::endl;
					Helpers::usageStatement(argv[0]);
					return EXIT_FAILURE;
				}
				year_q.push(temp);
				break;
			case ':':		/*missing option argument*/
				std::cerr << "Warning" << argv[0] << ": Option -" << optopt << " requires an argument"  << std::endl;
				if (!warning_flag) {warning_flag =1;}

			case '?':
				/* getopt_long already printed an error message. */
				break;

			default:
				std::cerr << "Unexpected error: switch value from option parsing passed to default" << std::endl;
				Helpers::usageStatement(argv[0]);
				return EXIT_FAILURE;
		}
	}
	//Process the remaining commandline options (the date or year provided)
	if (argc > optind) 
	{
		std::cerr << "Warning: the following non-option inputs cannot be parsed:" << std::endl;
		if (!warning_flag) {warning_flag =1;}
		while (optind < argc)
		{
			std::cerr << argv[optind++] << std::endl;
		}
	}

	while (!Format_q.empty())
	{
		switch(Format_q.front())
		{
			case 1:				// formatted dates
				if (!raw_date_q.empty())
				{
					dd_ptr = new DoomsdayDate::DoomsdayDate();
					if (Helpers::parseDate(dd_ptr, raw_date_q.front(), ad_q.front(), european_flag))
					{
						dd_q.push(dd_ptr);
					}
					else
					{
						delete dd_ptr;
						std::cerr << "Warning: formatted date is invalid" << std::endl;
						if (!warning_flag) {warning_flag =1;}
					}
					raw_date_q.pop();
					ad_q.pop();
				}
				Format_q.pop();
				break;

			case 2:				// segmented dates
				if (!dayofmonth_q.empty() && !month_q.empty() && !year_q.empty())
				{
					dd_ptr = new DoomsdayDate(month_q.front(), dayofmonth_q.front(), year_q.front(), ad_q.front());

					if (dd_ptr->Validity())
					{
						dd_q.push(dd_ptr);
					}
					else
					{
						delete dd_ptr;
						std::cerr << "Warning: segemented date is invalid" << std::endl;
						if (!warning_flag) {warning_flag =1;}
					}
					month_q.pop();
					dayofmonth_q.pop();
					year_q.pop();
					ad_q.pop();
				}
				Format_q.pop();
				break;

			case 3:				// inputted holiday dates
				if (!holiday_q.empty() && !year_q.empty())
				{
					dd_ptr = new DoomsdayDate();
					if (Helpers::parseHoliday(dd_ptr, holiday_q.front(), year_q.front(), ad_q.front(), all_h_bool))
					{
						if (!all_h_bool)
							dd_q.push(dd_ptr);
						else
						{
							if (dd_ptr->SetNewYears(year_q.front(), ad_q.front()))
							{
								dd_q.push(dd_ptr);
								dd_ptr = new DoomsdayDate();
							}
							
							if (dd_ptr->SetMartinLutherKingJr(year_q.front(), ad_q.front(), true))
							{
								dd_q.push(dd_ptr);
								dd_ptr = new DoomsdayDate();
							}
							if (dd_ptr->SetGroundHogDay(year_q.front(), ad_q.front(), true))
							{
								dd_q.push(dd_ptr);
								dd_ptr = new DoomsdayDate();
							}
							if (dd_ptr->SetValentinesDay(year_q.front(), ad_q.front(), true))
							{
								dd_q.push(dd_ptr);
								dd_ptr = new DoomsdayDate();
							}
							if (dd_ptr->SetWashingtonsBirthday(year_q.front(), ad_q.front(), true))
							{
								dd_q.push(dd_ptr);
								dd_ptr = new DoomsdayDate();
							}
							if (dd_ptr->SetLeapDay(year_q.front(), ad_q.front(), true))
							{
								dd_q.push(dd_ptr);
								dd_ptr = new DoomsdayDate();
							}
							if (dd_ptr->SetPassover(year_q.front(), ad_q.front(), true))
							{
								dd_q.push(dd_ptr);
								dd_ptr = new DoomsdayDate();
							}
							if (dd_ptr->SetEaster(year_q.front(), ad_q.front(), true))
							{
								dd_q.push(dd_ptr);
								dd_ptr = new DoomsdayDate();
							}
							if (dd_ptr->SetGoodFriday(year_q.front(), ad_q.front(), true))
							{
								dd_q.push(dd_ptr);
								dd_ptr = new DoomsdayDate();
							}
							if (dd_ptr->SetMemorialDay(year_q.front(), ad_q.front(), true))
							{
								dd_q.push(dd_ptr);
								dd_ptr = new DoomsdayDate();
							}
							if (dd_ptr->SetIndependenceDay(year_q.front(), ad_q.front(), true))
							{
								dd_q.push(dd_ptr);
								dd_ptr = new DoomsdayDate();
							}
							if (dd_ptr->SetLaborDay(year_q.front(), ad_q.front(), true))
							{
								dd_q.push(dd_ptr);
								dd_ptr = new DoomsdayDate();
							}
							if (dd_ptr->SetRoshHashanah(year_q.front(), ad_q.front(), true))
							{
								dd_q.push(dd_ptr);
								dd_ptr = new DoomsdayDate();
							}
							if (dd_ptr->SetYomKippur(year_q.front(), ad_q.front(), true))
							{
								dd_q.push(dd_ptr);
								dd_ptr = new DoomsdayDate();
							}
							if (dd_ptr->SetColumbusDay(year_q.front(), ad_q.front(), true))
							{
								dd_q.push(dd_ptr);
								dd_ptr = new DoomsdayDate();
							}
							if (dd_ptr->SetHalloween(year_q.front(), ad_q.front(), true))
							{
								dd_q.push(dd_ptr);
								dd_ptr = new DoomsdayDate();
							}
							if (dd_ptr->SetElectionDay(year_q.front(), ad_q.front(), true))
							{
								dd_q.push(dd_ptr);
								dd_ptr = new DoomsdayDate();
							}
							if (dd_ptr->SetVeteransDay(year_q.front(), ad_q.front(), true))
							{
								dd_q.push(dd_ptr);
								dd_ptr = new DoomsdayDate();
							}
							if (dd_ptr->SetThanksgiving(year_q.front(), ad_q.front(), true))
							{
								dd_q.push(dd_ptr);
								dd_ptr = new DoomsdayDate();
							}
							if (dd_ptr->SetChristmasDay(year_q.front(), ad_q.front(), true))
							{
								dd_q.push(dd_ptr);
								dd_ptr = new DoomsdayDate();
							}
							delete dd_ptr;
							all_h_bool = false;
						}
					}
					else
						if (!warning_flag) {warning_flag =1;}
					
					holiday_q.pop();
					year_q.pop();
					ad_q.pop();
					}
				Format_q.pop();
				break;
		}
	}
	if (!dayofmonth_q.empty() || !month_q.empty() || !year_q.empty() || !holiday_q.empty())
	{
		std::cerr << "Warning: the following elements have been ignored" << std::endl;
		if (!warning_flag) {warning_flag =1;}
		do {
			if (!dayofmonth_q.empty())
			{
				std::cerr << "  Day of month: " << dayofmonth_q.front() << std::endl;
				dayofmonth_q.pop();

			}
			else if (!month_q.empty())
			{
				std::cerr << "  Month (number): " << month_q.front() << std::endl;
				month_q.pop();
			}
			else if (!year_q.empty())
			{
				std::cerr << "  Year: " << year_q.front() << std::endl;
				year_q.pop();
			}
			else if (!holiday_q.empty())
			{
				std::cerr << "  Holiday: " << holiday_q.front() << std::endl;
				holiday_q.pop();
			}
		} while (!dayofmonth_q.empty() || !month_q.empty() || !year_q.empty() || !holiday_q.empty());
	}
	if (dd_q.empty())
	{
		std::cerr << "Warning: at least one proper provided date is required." << std::endl;
		Helpers::usageStatement(argv[0]);
	}
	else  // queue is not empty and the dates must be processed
	{
		if (warning_flag)
		{
			std::cerr << "=== End of Warnings ===" << std::endl;
			Helpers::usageStatement(argv[0]);
		}
		dd_ptr = dd_q.top();
		dd_q.pop();
		(separator) ? dd_ptr->Print(european_flag, separator) : dd_ptr->Print(european_flag);
		while (!dd_q.empty())
		{
			c =dd_ptr->DaysUntil(*dd_q.top());
			if (c != 1)
			{
				std::cout << "  " << c <<  " days until" << std::endl;
			}
			else
			{
				std::cout << "  " << c <<  " day until" << std::endl;
			}
			dd_ptr = dd_q.top();
			(separator) ? dd_ptr->Print(european_flag, separator) : dd_ptr->Print(european_flag);
			dd_ptr = dd_q.top();
			dd_q.pop();
		}
	}

	while (!dd_q.empty())
	{
		delete dd_q.top();
		dd_q.pop();
	}

	return EXIT_SUCCESS;
}