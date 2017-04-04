#include "DoomsdayDate.h"

//=====================================================
// Core methods methods 
// (public & private for the doomsday algorithm)
//=====================================================


// Uses list initalization to set up a basic DoomsdayDate class
DoomsdayDate::DoomsdayDate(bool JulianBeforeGregorian)
:
month_(-1),
day_(-1),
year_(-1),
weekday_(-1),
ad_(true),
gregorian_(true),
leapyear_(false),
valid_(false),
useJulianBeforeGregorian_(JulianBeforeGregorian),
significance_(None)
{
}

// Determines the validity of a date and sets th doomsday date class as such
bool DoomsdayDate::Set_(int month, int day, int year, bool ad, bool JulianBeforeGregorian, Holiday significance)
{
    valid_ = true;
	month_ = month;
	day_ = day;
	year_ = year;
	ad_ = ad;
	useJulianBeforeGregorian_ = JulianBeforeGregorian;
	significance_ = significance;
    if( !Month::Valid(month) || !Day::Valid(day) || !Year::Valid(year) )
    {
        valid_ = false;
		return false;
	}
	if( !ad )
	{
		gregorian_ = false;
	}
	else
	{
		gregorian_ = Gregorian(month, day, year);
	}

	leapyear_ = LeapYear();
    if( !valid_ ) return false;
    if (day > Month::GetNumDays(month_, leapyear_)) return false;

	weekday_ = DetermineWeekday();
	if (significance == TBD)
	{
		significance_ = DetermineSignificance();
	}
	else
	{
		significance_ = significance;
	}
	valid_ = true;
	return valid_;
}

// Determines if the set date belongs to a leap year
bool DoomsdayDate::LeapYear() const
{
	int modFour = year_ % 4;
	int modHundred;
	if( gregorian_ )
	{
		modHundred = year_ % 100;
		if(((modFour == 0) && (modHundred != 0)) || ((modHundred == 0) && (modHundred % 4 == 0)))
			return true;
	}
	else
	{
		if (modFour == 0) {return true;}
	}
	return false;
}

// Determines whether or not the set date could fall within the georgian calander, which started October 15, 1582
bool DoomsdayDate::Gregorian(int month, int day, int year)
{
	bool r = false;
	if( year > 1582 )     //provided years are known to be AD
	{
		r = true;
	}
	else if( year < 1582 )
	{
		r = false;
	}
	else // ( yyyy == 1582 )
	{
		if( month == 10 )
		{
			if( day > 14 )
			{
				r = true;
			}
			else if( day < 5 )
			{
				r = false;
			}
			else
			{
                if( !useJulianBeforeGregorian_ ) // useJulianBeforeGregorian_ seems to just invalidate the removed julian dates...
                {
                    valid_ = true;
                }
                else
                {
                    std::cerr << "Warning: October 5-14, 1582 were omitted from Gregorian calendar" << std::endl;
                    valid_ = false; 
                }
				r = valid_;
			}
		}
		else if( month < 10 )
		{
			r = false;
		}
		else // Months after the switch
		{
			r = true;
		}
	}
	return r;
}

// Method that actually determines the day of the week, given a valid set date, then sets this weekday and returns it
int DoomsdayDate::DetermineWeekday() const
{
	int r = -1;			//return value, intially an invalid day of the week
	int x = 0, y = 0;
	int ddcentury = -1;
	int ddmonth = DoomsdayMonth(month_);
	if( gregorian_ ) // Gregorian Calendar
	{
		int century = year_ - (year_ % 100);            // not centuary as in the 21st, currently it would be 2000 (the last 2 digits are replaced w/ zeros)
		ddcentury = DoomsdayCentury(century);
		if( ddcentury < 0 ) return -1; // invalid results are communicated through -1 values
		if( ddmonth < 0 ) return -1;
		if( ddmonth > day_ )
		{
            x = 7 - ((ddmonth-day_) % 7 ); // translates the day to a matching day after the month's doomsday
		}
        else
        {
            x = day_ - ddmonth;            // the offset between the doomsday and the (translated) date in question
        }									
		x %= 7;
		y = ddcentury + (year_-century) + (floor((year_-century)/4));   // year_-century is the year part
		y %= 7;
		r = (x+y)%7;
	}
	else if( !ad_ ) // B.C -> AD Julian
	{
		int dd = -1;
		if( year_ > 699 )
		{
			dd = (year_ - (year_ % 700) + 701) - year_;
		}
		else
		{
			dd = (year_ - (year_ % 28) + 29) - year_;
		}
		if( dd > 0 )
		{
			ddcentury = (((year_ - (year_ % 100)) / 100) * 6) % 7;   // In the Julian Calendar, Doomsday for the year C·100 is 6C.
			y = ((dd%100)%7) + (int)floor((dd%100)/4)%7;					
			if( ddmonth > day_ )
				x = 7 - ((ddmonth-day_) % 7);
			else
				x = day_ - ddmonth;
			x %= 7;
			y = ddcentury + y;
			y %= 7;
			r = (x+y)%7;
		}
	}
	else // Julian Calendar
	{
		ddcentury = (((year_ - (year_ % 100)) / 100) * 6) % 7;       // In the Julian Calendar, Doomsday for the year C·100 is 6C.
		y = (year_%100) + (int)floor((year_%100)/4);					
		if( ddmonth > day_ )
			x = 7 - ((ddmonth-day_) % 7);
		else
			x = day_ - ddmonth;
		x %= 7;
		y = ddcentury + y;
		y %= 7;
		r = (x+y)%7;
	}
	return r;
}

// Returns the anchor or doomsday for the month, in which the set date falls for gregorian calendars 
int DoomsdayDate::DoomsdayMonth(int month) const
{
	int r = -1;
	switch(month)
	{
    case Month::JAN:
		if( leapyear_ )
			r = 32;
		else
			r = 31;
		break;
    case Month::FEB:
		if( leapyear_ )
			r = 29;
		else
			r = 28;
		break;
    case Month::MAR:
		r = 7;
		break;
    case Month::APR:
		r = 4;
		break;
    case Month::MAY:
		r = 9;
		break;
    case Month::JUN:
		r = 6;
		break;
    case Month::JUL:
		r = 11;
		break;
    case Month::AUG:
		r = 8;
		break;
    case Month::SEP:
		r = 5;
		break;
    case Month::OCT:
		r = 10;
		break;
    case Month::NOV:
		r = 7;
		break;
    case Month::DEC:
		r = 12;
		break;
	}
	return r;
}

// The anchor or doomsday of the century, within the set date falls (for Georgorian dates)
int DoomsdayDate::DoomsdayCentury(int century) const
{
	int r = -1;
	switch( century % 400 )
	{
	case 0:
		r = 2;
		break;
	case 100:
		r = 0;
		break;
	case 200:
		r = 5;
		break;
	case 300:
		r = 3;
		break;
	}
	return r;
}

//=====================================================
// Methods comparing dates & measuring days between them
//=====================================================

// overloaded operator used to compare dates
bool DoomsdayDate::operator<(const DoomsdayDate& rhs) const
{
	int lhs_y = ad_ ? (year_) : (-year_);
	int rhs_y = rhs.ad_ ? (rhs.year_) : (-rhs.year_);

	if (lhs_y < rhs_y)
	{
		return true;
	}
	else if (lhs_y == rhs_y)
	{
		if (month_ < rhs.month_)
		{
			return true;
		}
		else if (month_ == rhs.month_)
		{
			if (day_ < rhs.day_)
			{
				return true;
			}
		}
	}
	return false;
}

// counts the days so far in the set year
int DoomsdayDate::DaysSoFar() const
{
	int x = leapyear_ ? 1 : 0;
	switch(month_)
	{
	case 1:		//JAN							
		return day_;
	case 2:		//FEB
		return 31 + day_;
	case 3:		//MAR
		return 59 + x + day_;
	case 4:		//APL
		return 90 + x + day_;
	case 5:		//MAY
		return 120 + x + day_;
	case 6:		//JUN
		return 151 + x + day_;
	case 7:		//JUL
		return 181 + x + day_;
	case 8:		//AUG
		return 212 + x + day_;
	case 9:		//SEP
		return 243 + x + day_;
	case 10:	//OCT
		return 273 + x + day_;
	case 11:	//NOV
		return 304 + x + day_;
	case 12:	//DEC
		return 334 + x + day_;
	default:
		std::cerr << "Warning: month " << month_ << " not valid" << std::endl;
		return 0;
	}
}

// returns the number of leap years since the year zero, returns negative for BC dates
int DoomsdayDate::LeapYearsSoFar() const
{
	int y = year_ - 1;
	if (gregorian_)
	{
		return (y / 4) - (y / 100) + (y / 400) + 12;
		// last addition corrects for the 12 omitted leap days from the pre-1582 julian calander
	}
	if (!ad_)
	{
		return -(y / 4);
	}
	else
	{
		return (y / 4);
	}
}

// Days until the date object provided, which is assumed to be after the set date
int DoomsdayDate::DaysUntil(const DoomsdayDate& rhs) const
{
	int lhs_y = ad_ ? (year_) : (-year_);
	int rhs_y = rhs.ad_ ? (rhs.year_) : (-rhs.year_);
	int r = rhs_y - lhs_y;
	if (ad_ != rhs.ad_) r--; 				//correct for there being no year zero
	r= (r * 365) + (rhs.LeapYearsSoFar() - LeapYearsSoFar()) + (rhs.DaysSoFar() - DaysSoFar());
	if (gregorian_ != rhs.gregorian_) r -= 10;  //correct for the omitted days when the gregorian calander started

	return r;
}

//=====================================================
// Methods to find the nth x-day of a month
//=====================================================

// Used to find holidays that fall on the first ___day of the month
bool DoomsdayDate::SetFirst(int weekday, int month, int year, Holiday significance, bool JulianBeforeGregorian)
{
    int day;
    if( !Month::Valid(month) || !Year::Valid(year) || !Weekday::Valid(weekday) )
    {
    	return false;
    }
	day = 1;
	Set_(month, day, year, true, JulianBeforeGregorian);
	day += weekday - weekday_;	  		
	if (day < 0) {day += 7;}					//moves dates forward to first of month                

	Set_(month, day, year, true, JulianBeforeGregorian, significance);
    return true;
}

// Used to find holidays that fall on the second ___day of the month
bool DoomsdayDate::SetSecond(int weekday, int month, int year, Holiday significance)
{
    if( SetFirst(weekday, month, year, None, false) )     // this initial SetFirst must not use JulianBeforeGregorian to avoid issues w/ omitted dates
    {
        Set_(month, day_+7, year, true, true, significance);
        return true;
    }
    return false;
}

// Used to find holidays that fall on the third ___day of the month
bool DoomsdayDate::SetThird(int weekday, int month, int year, Holiday significance)
{
    if( SetFirst(weekday, month, year, None, false) )
    {
        Set_(month, day_+14, year, true, true, significance);
        return true;
    }
    return false;
}

// Used to find holidays that fall on the forth ___day of the month
bool DoomsdayDate::SetFourth(int weekday, int month, int year, Holiday significance)
{
    if( SetFirst(weekday, month, year, None, false))
    {
        Set_(month, day_+21, year, true, true, significance);
        return true;
    }
    return false;
}

// Used to find holidays that fall on the last ___day of the month
bool DoomsdayDate::SetLast(int weekday, int month, int year, Holiday significance)
{
	int temp_month, temp_year;
	if (month < 12)
	{
		temp_month = month + 1; //next month
		temp_year = year;
	}
	else       // given month is december
	{
		temp_month = 1;
		temp_year = year + 1;
	}
    if( SetFirst(weekday, temp_month, temp_year, None, false) )
    {
    	Set_(month, Month::GetNumDays(month, leapyear_) -7 + day_, year, true, true, significance);
    	// in the case the next month is a new year you still will not run into leapday issues
        return true;
    }
    return false;
}

// Used to check if the set date falls on the first ___day of the month
bool DoomsdayDate::CheckFirst(int weekday) const
{
    if (weekday_ == weekday && day_ < 7)
    {
    	return true;
    }
    return false;
}

// Used to check if the set date falls on the second ___day of the month
bool DoomsdayDate::CheckSecond(int weekday) const
{
    if (weekday_ == weekday && day_ > 6 && day_ < 14)
    {
    	return true;
    }
    return false;
}

// Used to check if the set date falls on the third ___day of the month
bool DoomsdayDate::CheckThird(int weekday) const
{
    if (weekday_ == weekday && day_ > 13 && day_ < 21)
    {
    	return true;
    }
    return false;
}

// Used to check if the set date falls on the fourth ___day of the month
bool DoomsdayDate::CheckFourth(int weekday) const
{
    if (weekday_ == weekday && day_ > 20 && day_ < 28)
    {
    	return true;
    }
    return false;
}

// Used to check if the set date falls on the last ___day of the month
bool DoomsdayDate::CheckLast(int weekday) const
{
    if (weekday_ == weekday)
    {
    	return day_ > (Month::GetNumDays(month_, leapyear_) - 7);
    }
    return false;
}

//=====================================================
// Setter methods for holidays
//=====================================================

DoomsdayDate::Holiday DoomsdayDate::DetermineSignificance() const
{

	switch(month_)
	{
		case Month::JAN:
			if (day_ == 1)
				return NewYears;				  //only holiday with no cap starting date	

			if (ad_ && year_ >= 1986  && CheckThird(Weekday::MONDAY)) //year the holiday was first observed 
				return MartinLutherKingJr;
			break;

		case Month::FEB:
			if (ad_) // no BC holidays in Feb
			{
				if (year_ >= 1887 && day_ == 2)
					return GroundHogDay;

				if (year_ >= 496 && day_ == 14)
					return ValentinesDay;

				if (year_ >= 1971 && CheckThird(Weekday::MONDAY)) // standardized to monday by the Uniform Monday Holiday Act
					return WashingtonsBirthday;

				if (leapyear_ && day_ ==29)
					return LeapDay;
			}

			break;

		case Month::MAR:

			if (ad_ && year_ >= 325)
			{
				if  (weekday_ == Weekday::FRIDAY && day_ >= 20)
				{
					DoomsdayDate dd_temp;
					dd_temp.SetGoodFriday(year_, ad_);
					if (*this == dd_temp)
						return GoodFriday;
				}

				if  (weekday_ == Weekday::SUNDAY && day_ >= 22)
				{
					DoomsdayDate dd_temp;
					dd_temp.SetEaster(year_, ad_);
					if (*this == dd_temp)
						return Easter;
				}
			}
			

			if ( (ad_ || year_ <= 3760) && day_ >= 26 )  // first paren checks for range of Jewish calander
			{
				DoomsdayDate dd_temp;
				dd_temp.SetPassover(year_, ad_);
				if (*this == dd_temp)
					return Passover;
			}

			break;

		case Month::APR:

			if (ad_ && year_ >= 325)
			{
				if  (weekday_ == Weekday::FRIDAY && day_ <= 23)
				{
					DoomsdayDate dd_temp;
					dd_temp.SetGoodFriday(year_, ad_);
					if (*this == dd_temp)
						return GoodFriday;
				}

				else if  (weekday_ == Weekday::SUNDAY && day_ <= 25)
				{
					DoomsdayDate dd_temp;
					dd_temp.SetEaster(year_, ad_);
					if (*this == dd_temp)
						return Easter;
				}
			}
			if  ((ad_ || year_ <= 3760) && day_ <= 25)
			{
				DoomsdayDate dd_temp;
				dd_temp.SetPassover(year_, ad_);
				if (*this == dd_temp)
					return Easter;
			}

			break;

		case Month::MAY:
			if (ad_) // no BC holidays in May
			{
				if (year_ >= 1971 && CheckLast(Weekday::MONDAY)) // standardized to monday by the Uniform Monday Holiday Act
					return MemorialDay;
				
				else if (year_ >= 1868 && day_ == 30)                        // celebrated on this date before the Uniform Monday Holiday Act
					return MemorialDay;
			}
			break;

		case Month::JUN:

			break;

		case Month::JUL:
			if (ad_ && year_ >= 1776 && day_ == 4)
				return IndependenceDay;

			break;

		case Month::AUG:
			// no holidays in aug
			break;

		case Month::SEP:
			if (ad_ && year_ >= 1882 && CheckFirst(Weekday::MONDAY))   // first celebrated in this year
				return LaborDay;

			if (ad_ || year_ <= 3760)
			{
				if  (day_ >= 5)
				{
					DoomsdayDate dd_temp;
					dd_temp.SetRoshHashanah(year_, ad_);
					if (*this == dd_temp)
						return RoshHashanah;
				}
				if  (day_ >= 14)
				{
					DoomsdayDate dd_temp;
					dd_temp.SetYomKippur(year_, ad_);
					if (*this == dd_temp)
						return YomKippur;
				}
			}
			break;

		case Month::OCT:

			if (ad_)
			{
			if (year_ >= 1971 && CheckSecond(Weekday::MONDAY))  // standardized to monday by the Uniform Monday Holiday Act
				return ColumbusDay;

			if (year_ >= 1911 && day_ == 31)
				return Halloween;
			}
			
			if (ad_ || year_ <= 3760)
			{
				if  (day_ <= 5)
				{
					DoomsdayDate dd_temp;
					dd_temp.SetRoshHashanah(year_, ad_);
					if (*this == dd_temp)
						return RoshHashanah;
				}
				if  (day_ <= 14)
				{
					DoomsdayDate dd_temp;
					dd_temp.SetYomKippur(year_, ad_);
					if (*this == dd_temp)
						return YomKippur;
				}
			}
			break;

		case Month::NOV:
			if (ad_)		//no BC holidays in Novemeber
			{	
				if (year_ >= 1845 && CheckFirst(Weekday::TUESDAY))  // standardized to this Tuesday in 1845
					return ElectionDay;

				if (year_ >= 1919 && day_ == 11) // first celebrated in this year
					return VeteransDay;
				
				if (year_ >= 1863 && CheckFourth(Weekday::THURSDAY))
					return Thanksgiving;
			}
			break;

		case Month::DEC:
			if (ad_ && year_ >= 1 && day_ == 25)
				return ChristmasDay;
			
			break;
	}
	return None;
}

// New Year's Day, January 1.
bool DoomsdayDate::SetNewYears(int year, bool ad)
{
    return Set_(Month::JAN, 1, year, ad, true, NewYears); //no warnings since every year has a new years day
}

// Birthday of Martin Luther King, Jr., the third Monday in January.
bool DoomsdayDate::SetMartinLutherKingJr(int year, bool ad, bool silence)
{
    if (ad && year >= 1986) {
        return SetThird(Weekday::MONDAY, Month::JAN, year, MartinLutherKingJr);
    }
    else
    {
        if (!silence)
        	std::cerr << "Warning: MLK day was not celebrated until 1986, year provided was " << year << std::endl;
        return false;
    }
}

// Ground Hog Day, Feburary 2. 
bool DoomsdayDate::SetGroundHogDay(int year, bool ad, bool silence)
{
    if (ad && year >= 1887)
    	return Set_(Month::FEB, 2, year, ad, true, GroundHogDay);
	else
    {
        if (!silence)
        	std::cerr << "Warning: Gound Hog Day was not celebrated until 1887, year provided was " << year << std::endl;
        return false;
    }
}

// Valentine's Day, Feburary 14. 
bool DoomsdayDate::SetValentinesDay(int year, bool ad, bool silence)
{
    if (ad && year >= 496)
   		return Set_(Month::FEB, 14, year, ad, true, ValentinesDay);
	else
    {
        if (!silence)
        	std::cerr << "Warning: Valentine's Day was not celebrated until 496, year provided was " << year << std::endl;
        return false;
    }
}

// Washington's Birthday, the third Monday in February.
bool DoomsdayDate::SetWashingtonsBirthday(int year, bool ad, bool silence)
{
    if (ad && year >=  1971 ) {
        return SetThird(Weekday::MONDAY, Month::FEB, year, WashingtonsBirthday);
    } 
    else
    {
        if (!silence)
        	std::cerr << "Warning: Washington's Birthday was not celebrated until 1971, year provided was " << year << std::endl;
        return false;
    }
}

// Leap Day, as determined by the Gregorian or Julian calander
 bool DoomsdayDate::SetLeapDay(int year, bool ad, bool silence)
{
    if (Set_(Month::FEB, 29, year, ad, true, LeapDay))
    {
    	return true;
    }
    else
    {
        if (!silence)
        	std::cerr << "Warning: " << year << " was not a leap year" << std::endl;
        return false;
    }
}

// Used to find the date of Easter, using John Conway's method
bool DoomsdayDate::SetEaster(int year, bool ad, bool silence)
{
    if (ad)
    {
    	if(year >= 1583) //gregorian calculation
    	{
	    	int a = year % 19;
			int b = year / 100;
			int c = year % 100;
			int d = b / 4;
			int e = b % 4;
			int f = (b + 8) / 25;
			int g = (b - f + 1) / 3;
			int h = (19 * a + b - d - g + 15) % 30;
			int i = c / 4;
			int k = c % 4;
			int l = (32 + 2 * e + 2 * i - h - k) % 7;
			int m = (a + 11 * h + 22 * l) / 451;
			int n = (h + l - 7 * m + 114) / 31;
			int p = (h + l - 7 * m + 114) % 31;
			return Set_(n , p + 1, year, ad, true, Easter);
		}
		else if (year >= 325) //  // First Council of Nicaea (325 AD)
		{
			int a = year % 19;
			int b = (19 * a + 15) % 30;
			int c = (year + (year/4) + b) % 7;
			int d = b - c + 28;
			if (d > 31)
			{
				a = d - 31;  //day
				b = 4;       //month
			}
			else
			{
			    a = d;
			    b = 3;
			}
			return Set_(b, a, year, ad, true, Easter);
	    }
	}
	if (!silence)
		std::cerr << "Warning: Easter was not codified until 325 AD, year provided was" << year << std::endl;
	return false;
}

// Used to find the date of Good Friday
bool DoomsdayDate::SetGoodFriday(int year, bool ad, bool silence)
{
	if (SetEaster(year, ad, silence))
	{
		if (day_ > 2) 
			return Set_(month_, day_ - 2, year, ad, true, GoodFriday);
		else								///meaning an good friday is in march and easter is in april
			return Set_(month_ - 1, 29 + day_ ,year, ad, true, GoodFriday);
	}
    return false;
}

// Memorial Day, the last Monday in May.
bool DoomsdayDate::SetMemorialDay(int year, bool ad, bool silence)
{
    if (ad && year >= 1971 ) {            
        return SetLast(Weekday::MONDAY, Month::MAY, year, MemorialDay);
    }
    else if (ad && year >= 1868 )
    {
        return SetLast(Weekday::MONDAY, Month::MAY, year, MemorialDay);           
    }
    else
    {
        if (!silence)
        	std::cerr << "Warning: Memorial day was not celebrated until 1868, year provided was " << year << std::endl;
        return false;
    }
}

// Independence Day, July 4.
bool DoomsdayDate::SetIndependenceDay(int year, bool ad, bool silence)
{
    if (ad && year >= 1776) 
    {               
        return Set_(Month::JUL, 4, year, ad, IndependenceDay);
    }
    else
    {
        if (!silence)
        	std::cerr << "Warning: Independence day was not celebrated until 1776, year provided was " << year << std::endl;
        return false;
    }
}

// Labor Day, the first Monday in September.
bool DoomsdayDate::SetLaborDay(int year, bool ad, bool silence)
{
    if (ad && year >=  1882 )
    {              
        return SetFirst(Weekday::MONDAY, Month::SEP, year, LaborDay);
    }
    else
    {
        if (!silence)
        	std::cerr << "Warning: Labor day was not celebrated until 1882, year provided was " << year << std::endl;
        return false;
    }
}

// Rosh Hashanah, first day of the jewish new year
bool DoomsdayDate::SetRoshHashanah(int year, bool ad, bool silence)
{
    if (ad || (!ad && year < 3761))  // 3760 is the 1st year of the jewish calander
	{
	    int r = (12 * ((year % 19) + 1)) % 19;
	    int n;
	    double v,f;
	 	
	 	if (year > 1851)   		// Rosh Hashanah 1582 starts after the Gregorian switch
	   		v =  floor( year / 100.0 ) - floor( year / 400.0 ) - 2;
	   	else
	   		v = 0;
	    v += 765433.0 * r / 492480;
	    v += ( year % 4 ) / 4.0;
	    v -= ( 313.0 * year + 89081 ) / 98496;
    	n = (int) v;
    	f = v - n;

    	if (n < 31)
    		Set_(Month::SEP, n, year, ad, true, RoshHashanah);
    	else
    		Set_(Month::OCT, n - 30, year, ad, true, RoshHashanah);

    
    	if (weekday_ == Weekday::SUNDAY || weekday_ == Weekday::WEDNESDAY || weekday_ == Weekday::FRIDAY)
    		n+=1;
    	else if (weekday_ == Weekday::MONDAY && f >= (23269.0 / 25920) && r > 11)
    		n+=1;
    	else if (weekday_ == Weekday::TUESDAY && f >= (1367.0 / 2160) && r > 6)
    		n+=2;
    	else
    		return valid_;

    	if (n < 31)
    		return Set_(month_, n , year, ad, true, RoshHashanah);
    	else
    		return Set_(Month::OCT, n - 30, year, ad, true, RoshHashanah);
    	// Rosh Hashanah will not run into the end of october

    }
    if (!silence)
    	std::cerr << "Warning: the jewish calander starts at 3760 BC, " << year << std::endl;
    return false;
}
// Yom Kippur, the Jewish day of atonement
bool DoomsdayDate::SetYomKippur(int year, bool ad, bool silence)
{
	if (SetRoshHashanah(year, ad, silence))
	{
		if (day_ < 22) 		// The jewish high holidays span early SEP to early OCT, adding 10 days will not run into nov
			return Set_(month_, day_ + 9, year, ad, true, YomKippur);
		else
			return Set_(Month::OCT, day_ - 21, year, ad, true, YomKippur);
	}
	return false;
}

// Passover, the Jewish holiday celebrating the exodus from egypt
bool DoomsdayDate::SetPassover(int year, bool ad, bool silence)
{
	if (SetRoshHashanah(year, ad, silence))
	{
		int m;
		if (month_ == Month::SEP)
			m = day_;
		else   // October
			m = day_ + 30;

		m += 21;
		if (m > 31)
			return Set_(Month::APR, m - 31, year, ad, true, Passover);
		else
			return Set_(Month::MAR, m, year, ad, true, Passover);
	}
	return false;
}


// Columbus Day, the second Monday in October.
bool DoomsdayDate::SetColumbusDay(int year, bool ad, bool silence)
{
    if (ad && year >= 1971 ) {                 
        return SetSecond(Weekday::MONDAY, Month::OCT, year, ColumbusDay);  
    }
    else
    {
        if (!silence)
        	std::cerr << "Warning: Columbus day was not celebrated until 1971, year provided was " << year << std::endl;
        return false;
    }
}

// Halloween, October 31st
bool DoomsdayDate::SetHalloween(int year, bool ad, bool silence)
{
    if (ad_ && year >= 1911)
    	return Set_(Month::OCT, 31, year, ad, true, Halloween);
   	else
   	{
   		if (!silence)
   			std::cerr << "Warning: Halloween wasn't commonly celebrated until 1911, year provided was " << year << std::endl;
        return false;
   	}
}

// Election Day, First Tuesday in Novemeber
bool DoomsdayDate::SetElectionDay(int year, bool ad, bool silence)
{
    if (ad && year >= 1845) {               
        return SetFirst(Weekday::TUESDAY, Month::NOV, year, ElectionDay);
    }
    else
    {
        if (!silence)
        	std::cerr << "Warning: Veterans day was not celebrated until 1919, year provided was " << year << std::endl;
        return false;
    }
}

// Veterans Day, November 11.
bool DoomsdayDate::SetVeteransDay(int year, bool ad, bool silence)
{
    if (ad && year >= 1919) {               
        return Set_(Month::NOV, 11, year, ad, VeteransDay);
    }
    else
    {
        if (!silence)
        	std::cerr << "Warning: Veterans day was not celebrated until 1919, year provided was " << year << std::endl;
        return false;
    }
}

// Thanksgiving, the fourth Thursday in November.
bool DoomsdayDate::SetThanksgiving(int year, bool ad, bool silence)
{
    if (ad && year >= 1863) {                 
        return SetFourth(Weekday::THURSDAY, Month::NOV, year, Thanksgiving);
    }    
    else
    {
        if (!silence)
        	std::cerr << "Warning: Thanksgiving was not celebrated until 1863, year provided was " << year << std::endl;
        return false;
    }
}

// Christmas Day, December 25.
bool DoomsdayDate::SetChristmasDay(int year, bool ad, bool silence)
{
    if ( ad ) {               
        return Set_(Month::DEC, 25, year, ad, true, ChristmasDay);
    }
    else
    {
        if (!silence)
        	std::cerr << "Warning: Christmas was not celebrated before the start of the Common Era" << std::endl;
        return false;
    }
}

//=====================================================
// Reporting methods
//=====================================================

const char* DoomsdayDate::default_sep = "/";

// reports the weekday, and full date of the set date for this instance of DoomsdayDate
void DoomsdayDate::Print(const int european_order, const char* separator) const
{
    if( !valid_ )
    {
        std::cerr << "Date Not Valid: ";
        if (european_order)
        {
        	std::cerr << day_ << separator << month_ << separator << year_;
        }
        else
        {
        	std::cerr << month_ << separator << day_ << separator << year_;
        }
        if( !ad_ )
	    {
	        std::cerr << " BC ";
	    }
        std::cerr << std::endl; 
        return;
    }
    std::cout << WeekdayStr() << " ";
    if (european_order)
    {
    	std::cout << day_ << separator << month_ << separator << year_;
    }
    else
    {
    	std::cout << month_ << separator << day_ << separator << year_;
    }
    if( !ad_ )
    {
        std::cout << " BC";
    }
    switch(significance_)
    {
    	case NewYears:
    		std::cout << ", New Year's Day";
    		break;

    	case GroundHogDay:
    	    std::cout << ", Ground Hog Day";
    		break;

    	case ValentinesDay:
    	    std::cout << ", Valentine's Day";
    		break;

    	case MartinLutherKingJr:
    	    std::cout << ", Martin Luther King Jr Day";
    		break;

    	case WashingtonsBirthday:
    		std::cout << ", Washington's Birthday (President's Day)";
    		break;

    	case MemorialDay:
    		std::cout << ", Memorial Day";
    		break;

    	case GoodFriday:
    		std::cout << ", Good Friday";
    		break;

    	case Easter:
    		std::cout << ", Easter Sunday";
    		break;

    	case Passover:
    		std::cout << ", Passover";
    		break;

     	case LeapDay:
    		std::cout << ", Leap Day";
    		break;

    	case IndependenceDay:
    		std::cout << ", Independence Day";
    		break;    	

    	case LaborDay:
    		std::cout << ", Labor Day";
    		break;    	

    	case RoshHashanah:
    		std::cout << ", Rosh Hashanah";
    		break;   
    	
    	case YomKippur:
    		std::cout << ", Yom Kippur";
    		break;   

    	case ColumbusDay:
    		std::cout << ", Columbus Day";
    		break;

    	case VeteransDay:
    		std::cout << ", Veterans Day";
    		break;

    	case Halloween:
    		std::cout << ", Halloween";
    		break;

    	case ElectionDay:
    		std::cout << ", Election Day";
    		break;

    	case Thanksgiving:
    		std::cout << ", Thanksgiving";
    		break;

    	case ChristmasDay:
    		std::cout << ", Christmas Day";
    		break;

    	default:
    		// TBD should not reach this point, and None significance should not be printed
    		break;
    }
    std::cout << std::endl; 
}

// Version of WeekdayStr method defined in header, but this taking an int input, translating it to a weekday string
const char* DoomsdayDate::WeekdayStr(int weekDay) const
{
    if( !Weekday::Valid(weekDay) )
    {
    	std::cerr << "Warning: weekday Not Valid" << std::endl;
    }
    return Weekday::GetString(weekDay);
}
