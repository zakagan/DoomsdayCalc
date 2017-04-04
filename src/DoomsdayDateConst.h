#ifndef _DOOMSDAYDATE_CONST_H_
#define _DOOMSDAYDATE_CONST_H_

namespace // hide from global namespace
{

namespace Weekday
{
    const int SUNDAY = 0;
    const int MONDAY = 1;
    const int TUESDAY = 2;
    const int WEDNESDAY = 3;
    const int THURSDAY = 4;
    const int FRIDAY = 5;
    const int SATURDAY = 6;

    const char* days[] = {
                            "Sunday",
                            "Monday",
                            "Tuesday",
                            "Wednesday",
                            "Thursday",
                            "Friday",
                            "Saturday" };

    inline bool Valid(int weekday) { return (weekday>-1&&weekday<7); }
    inline const char* GetString(int day)
    {
        if( Valid(day) ) return days[day];
        return "";
    }
};

namespace Month
{
    const int JAN = 1;
    const int FEB = 2;
    const int MAR = 3;
    const int APR = 4;
    const int MAY = 5;
    const int JUN = 6;
    const int JUL = 7;
    const int AUG = 8;
    const int SEP = 9;
    const int OCT = 10;
    const int NOV = 11;
    const int DEC = 12;
    inline bool Valid(int month) { return (month>0 && month<13); }
    inline int GetNumDays(int month, bool leapyear)
    {
        switch(month)
            {
            case APR: // 30 day months
            case JUN:
            case SEP:
            case NOV:
                return 30;
            case FEB:
                if(!leapyear)
                    return 28;
                else
                    return 29;
            default:
                return 31;
            }
    }

};

namespace Year
{
    inline bool Valid(int year) { return (year>0 /* consider adding a top cap*/); }
};

namespace Day
{
    inline bool Valid(int day) { return (day>0&&day<32); }
};


};

#endif