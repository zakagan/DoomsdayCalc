#ifndef _DOOMSDAYDATE_H_
#define _DOOMSDAYDATE_H_

#include <iostream>
#include <string>
#include <math.h>
#include "DoomsdayDateConst.h"

//-----------------------------------------------------
// This class is broken up into sections based on 
// function, to improve readablity
//-----------------------------------------------------

class DoomsdayDate
{
    private:
        // datatype used to hold a day's signifigance
        enum Holiday {TBD, None, NewYears, GroundHogDay, ValentinesDay, MartinLutherKingJr, WashingtonsBirthday, \
        LeapDay, GoodFriday, Easter, Passover, MemorialDay, IndependenceDay, LaborDay, RoshHashanah, YomKippur, \
        ColumbusDay, Halloween, ElectionDay, VeteransDay, Thanksgiving, ChristmasDay};

    public:

        //-----------------------------------------------------
        // JulianBeforeGregorian refers to the weird dates
        // between October 5-14, 1582 which were deleted from
        // the Gregorian calendar.  Technically the days never
        // existed.  The default is true which sets the date
        // as invalid.  
        //-----------------------------------------------------

    	DoomsdayDate(bool JulianBeforeGregorian = true);
        inline DoomsdayDate(int month, int day, int year, bool ad=true, bool JulianBeforeGregorian = true)
        {
            Set_(month, day, year, ad, JulianBeforeGregorian);
        }

        inline DoomsdayDate(const DoomsdayDate& dd)
        {
            copy_(dd);
        }
        inline const DoomsdayDate& operator=(const DoomsdayDate& dd)
        {
            copy_(dd);
            return *this;        
        }
        inline bool operator==(const DoomsdayDate& rhs) const
        {
            return ((ad_ && rhs.ad_) && (year_ == rhs.year_) && (month_ == rhs.month_) && (day_ == rhs.day_));       
        }

        inline void Set(int month, int day, int year, bool ad=true, bool JulianBeforeGregorian = true)
        {
            Set_(month, day, year, ad, JulianBeforeGregorian);
        }

        //=====================================================
        // Getter methods
        //=====================================================

        inline bool AD() const { return ad_; }
        inline bool BC() const { return !ad_; }   
        inline bool Validity() const { return valid_; }     
        inline int Day() const { return day_; }
        inline int Month() const { return month_; }
        inline int Year() const { return year_; }
        Holiday Significance() const { return significance_;}

        //=====================================================
        // Setter methods for holidays
        //=====================================================

        bool SetNewYears(int year, bool ad);
        bool SetMartinLutherKingJr(int year, bool ad, bool silence = false);
        bool SetGroundHogDay(int year, bool ad, bool silence = false);
        bool SetValentinesDay(int year, bool ad, bool silence = false);
        bool SetWashingtonsBirthday(int year, bool ad, bool silence = false);
        bool SetLeapDay(int year, bool ad, bool silence = false);
        bool SetPassover(int year, bool ad, bool silence = false);
        bool SetEaster(int year, bool ad, bool silence = false);
        bool SetGoodFriday(int year, bool ad, bool silence = false);
        bool SetMemorialDay(int year, bool ad, bool silence = false);
        bool SetIndependenceDay(int year, bool ad, bool silence = false);
        bool SetLaborDay(int year, bool ad, bool silence = false);
        bool SetRoshHashanah(int year, bool ad, bool silence = false);
        bool SetYomKippur(int year, bool ad, bool silence = false);
        bool SetColumbusDay(int year, bool ad, bool silence = false);
        bool SetElectionDay(int year, bool ad, bool silence = false);
        bool SetVeteransDay(int year, bool ad, bool silence = false);
        bool SetHalloween(int year, bool ad, bool silence = false);
        bool SetThanksgiving(int year, bool ad, bool silence = false);
        bool SetChristmasDay(int year, bool ad, bool silence = false);

        //=====================================================
        // Methods comparing dates & measuring days between them
        //=====================================================

        bool operator<(const DoomsdayDate& rhs) const;
        int DaysUntil(const DoomsdayDate& rhs) const;

        //=====================================================
        // Reporting methods
        //=====================================================

        const char* WeekdayStr(int weekDay) const;
        const char* WeekdayStr() const { return WeekdayStr(weekday_); }   
        void Print(const int european_order = 0, const char* separator = default_sep) const;

    private:

        //=====================================================
        // class properties for setting and finding dates
        //=====================================================

        int month_;
        int day_;
        int year_;
        int weekday_;
        bool ad_;
        bool gregorian_;
        bool leapyear_;
        bool valid_;
        bool useJulianBeforeGregorian_;
        Holiday significance_;

        static const char* default_sep;          // used to seperate dates when reporting, eg 2/27/2017

        //=====================================================
        // Core private methods for setting and finding dates
        //=====================================================

        bool Set_(int month, int day, int year, bool ad = true, bool JulianBeforeGregorian = true, Holiday significance = TBD);
        inline void copy_(const DoomsdayDate& dd)
        {
            month_ = dd.month_;
            day_ = dd.day_;
            year_ = dd.year_;
            weekday_ = dd.weekday_;
            ad_ = dd.ad_;
            gregorian_ = dd.gregorian_;
            leapyear_ = dd.leapyear_;
            valid_ = dd.valid_;
            useJulianBeforeGregorian_ = dd.useJulianBeforeGregorian_;
            significance_ = dd.significance_;
        }
        int DoomsdayCentury(int century) const;
        int DoomsdayMonth(int month) const;
        bool Gregorian(int month, int day, int year);
        bool LeapYear() const;

        int DetermineWeekday() const;                        // method that executes the doomsday algorithm 

        //=====================================================
        // Methods comparing dates & measuring days between them
        //=====================================================

        int LeapYearsSoFar() const;
        int DaysSoFar() const;

        //=====================================================
        // Methods to find the nth x-day of a month
        //=====================================================

        bool SetFirst(int weekday, int month, int year, Holiday significance = None, bool JulianBeforeGregorian = true );
        bool SetSecond(int weekday, int month, int year, Holiday significance = None);
        bool SetThird(int weekday, int month, int year, Holiday significance = None);
        bool SetFourth(int weekday, int month, int year, Holiday significance = None);
        bool SetLast(int weekday, int month, int year, Holiday significance = None);
        bool CheckFirst(int weekday) const;
        bool CheckSecond(int weekday) const;
        bool CheckThird(int weekday) const;
        bool CheckFourth(int weekday) const;
        bool CheckLast(int weekday) const;

        Holiday DetermineSignificance() const;      // Finds and sets the significance (possible holiday) for a date

};

#endif