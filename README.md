# DoomsdayCalc

This project implements John Conway's Doomsday algorithm to find the day of the week for user provided dates. Dates are parsed from the command line using the getopt standard, with several options for entering dates. When multiple dates are provided the number of days between dates is reported.

I started with some basic code from [a 2002 page on Code Project](https://www.codeproject.com/articles/2501/any-day-of-the-week-using-the-doomsday-rule). I noticed some issues and decided to build a more complete version of what original author Paul J. Weiss uploaded. No built-in date or time libraries have been used. The programming behind this project is broken into two main parts:

1. Parsing and organizing user provided date formats

2. Processing them to find the day of the week and distance between dates.

These two elements work together in order to handle holidays. Some holidays occur on specific days of the week rather than specific dates, like Easter Sunday or Memorial Day Monday. Users can provide a year and a holiday and DoomsdayCalc will find the date where that holiday falls in that year, along with the day of the week. If the user enters in a date that turns out to be a holiday, DoomsdayCalc will recognize it as well.

## What is a Doomsday?

This project won't tell you when the world is going to end. But, if you happened to know the specific date of the end of the world, DoomsdayCalc could tell you what day of the week it would be on and if it falls on a major holiday.

So what is a "Doomsday" then? John Conway uses doomsdays as term for anchor days, from which the day of the week can be calculated. For example, the century Doomsday for 2000-2099 is Tuesday. Using this information you can calculate the year Doomsday for 2017. And from there you use the Doomsday of the month in question along with the date to find the matching day of the week.

This is a very broad strokes synopsis of the Doomsday algorithm. More detail is included in a section below.

## Example Problem

How many days are there between Thanksgiving and Christmas? While Christmas is always on Dec 25th, Thanksgiving falls on the fourth Thursday in November. That means the traditional Christmas season doesn't have a fixed length, it varies from year to year.

This question can be answered by DoomsdayCalc for any provided year.

`./DoomsdayCalc -y 2017 -h Thanksgiving -y 2017 -h Christmas`

The output looks like this:

![alt tag](https://github.com/zakagan/DoomsdayCalc/blob/master/screenshots/christmas_1.png)

These inputs lead the program to create two DoomsdayDate objects, one for Thanksgiving 2017 and one for Christmas 2017. Each object stores it's date, significance (if it's a holiday and if so what holiday), and day of the week. Then this information is reported back to the user, along with the number of days difference between the two dates.

But DoomsdayCalc can handle a lot more than just two dates at a time. A priority queue organizes the DoomsdayDate objects into chronological order, and they are reported back accordingly along with the days between them. By doing this a user can find the span of holiday seasons across different years.

`./DoomsdayCalc -y 2017 -h Thanksgiving -y 2016 -h Thanksgiving -y 2015  -h Thanksgiving -y 2017 -h Christmas -y 2016 -h Christmas -y 2015 -h Christmas`

The output looks like this:

![alt tag](https://github.com/zakagan/DoomsdayCalc/blob/master/screenshots/christmas_2.png)

What if a user enters a date that turns out to be a holiday? DoomsdayCalc can recognize these dates as well and will report back the significance. For example:

![alt tag](https://github.com/zakagan/DoomsdayCalc/blob/master/screenshots/easter.png)

What about older dates? The famous Battle of Hastings between the Normans and the Anglo-Saxons occurred on Saturday, October 14th, 1066. Does DoomsdayCalc get it right?

![alt tag](https://github.com/zakagan/DoomsdayCalc/blob/master/screenshots/hastings.png)

## Options for Date Formatting

Any valid date between 99,999 BC and 99,999 AD can be used (note: there was no "year 0"). There are three ways for users to input dates.

1. A standard formatted date e.g. 7/20/1969. Slashes do not need to be used as separators since DoomsdayCalc uses a regex expression to extract date information. However some non-numerical separator is required. This format uses the -D flag or --Date flag.

2. A segmented date e.g. -d 20 -m 20 -y 1969. This format allows the user to explicitly list each individual element of the date being inputted, where the -d flag takes in the day of the month, the -m flag takes in the month, and the -y flag takes in the year. These flags also have the following longer options, respectively: --day, --month, --year. They do not need to be provided in any particular order.

3. A year and holiday pair, as shown in the previous section. Give any holiday string using the flag -h or the longer flag --holiday. A list of implemented holidays is provided below.

Each date can be augmented with the flags --BC/--BCE or --AD/--CE to specify whether or not it takes place before the start of the common era. The shorter version of these flags are -b and -a respectively. By default DoomsdayCalc assumes a date takes place in the common era.

Dates are parsed and reported using the American date system by default as well (meaning the month is given before the day of the month). To tell DoomadayCalc to use the European system simply pass an -e flag or a longer --European flag. The user may also input -r or --American to switch back to the American system.

Finally, dates are reported with slashes ('/') as separators by default, but this can be changed by passing a new separator with the -s or --separator flag.

The following is an example of how all these options may be used:

![alt tag](https://github.com/zakagan/DoomsdayCalc/blob/master/screenshots/details.png)

## Notes on the Doomsday Algorithm

There are a lot of resources about John Conway's Doomsday Algorithm (also called the Doomsday Rule). Most popular articles focus on the mental counting techniques, which could be handy if you want a trick for your next cocktail party. Without those tricks the math is somewhat straightforward.

Days of the week are broken down numerically as follows:
  * Sunday = 0
  * Monday = 1
  * Tuesday = 2
  * Wednesday = 3
  * Thursday = 4
  * Friday = 5
  * Saturday = 6

Here is an abridged run through of the method using October 21st, 2015 (brief aside: what movie is made this date famous?):

1. Find the doomsday of the century by taking the "century part" (e.g. 20 from 2015), then taking that modulo 4.
  * if the result is 0, the century Doomsday is Tuesday
  * if the result is 1, the century Doomsday is Sunday
  * if the result is 2, the century Doomsday is Friday
  * if the result is 3, the century Doomsday is Wednesday

2. Next take the "year part" of the date (e.g. 15 from 2015), compute the following: `century doomsday + year part + floor(year part ÷ 4) % 7`. Taking the century doomsday from the previous step (Tuesday, or 2 in numerical form), we compute `2 + 15 + floor(15 ÷ 4) % 7 = 6` which is Saturday. This is the "year doomsday".

3. Then find the doomsday of the month. Each of the 12 months has its own anchor day, for October it is the 10th (note that it's an anchor day of the month as opposed to the day of the week). From what was previously calculated, the 10th is a Saturday. From there clock either forward or back to the date in question. The 21st is a week and 4 days from the 10th. This can be calculated by taking `month doomsday + year doomsday % 7`. The result is 3, or Wednesday

Thus October 21st, 2015 is a Wednesday. There are a few additional details (for example the method is different for pre-Gregionian dates) but the code is commented to shed some additional light.

For a detailed explanation on the Doomsday rule, [please read these lecture notes from Mathematics Prof. S.W. Graham](http://people.cst.cmich.edu/graha1sw/Pub/Doomsday/Doomsday.html).

## Notes on holidays

Holidays strings can be provided using any number of keywords. Strings are searched for appropriate key words and the proper holidays are added to the queue of DoomsdayDate objects. The strings are not case sensitive (in fact, all non-letter chars are stripped before searching), and shorthand 3 letter abbreviations can be used instead of a holiday's full name.

For the most part, the included holidays are based on dates observed in the United States.

The following Holidays are implemented:

1. **New Year's Day**

    * **keyword**: "New Years" or just "nyd"  
    * **occurs**: January 1st  
    * **range**: Every year has a New Year's Day

2. **Martin Luther King Jr. Day**

    * **keyword**: "Martin Luther King" or just "mlk"  
    * **occurs**: 3rd Monday in January  
    * **range**: Every year since 1986
    
3. **Ground Hog Day**

    * **keyword**: "Ground Hog" or just "ghd"  
    * **occurs**: February 2nd  
    * **range**: Every year since 1887
    
4. **Valentine's Day**

    * **keyword**: "Valentines" or "valentine" or just "vld"  
    * **occurs**: February 14th  
    * **range**: Every year since 496 (using a loose interpretation of the Valentine's celebration)
    
5. **Washington's Birthday (President's Day)**

    * **keyword**: "Washington" or "Lincoln" or "president" or just "wbd"  
    * **occurs**: 3rd Monday in February  
    * **range**: Every year since 1971
    
6. **Leap Day**

    * **keyword**: "Leap Day" or "leap" or just "lpd"  
    * **occurs**: February 29th  
    * **range**: In Gregorian calendar, every four years except every year divisible by 100, but including years divisible by both 100 and 400. In the Julian calendar it is simply every four years.
    
7. **Good Friday**

    * **keyword**: "Good Friday" or just "gfd"  
    * **occurs**: A Friday between March 20th and April 23rd, two days before Easter.  
    * **range**: Every year since 325 AD, the First Council of Nicaea.
        
8. **Easter**

    * **keyword**: "Easter" or just "est"  
    * **occurs**: A Sunday between March 22th and April 25rd  
    * **range**: Every year since 325 AD, the First Council of Nicaea.  
    * **note**: Easter is calculated using John Conway's algorithm
    
9. **Passover**

    * **keyword**: "Passover" or just "psv"  
    * **occurs**: Between March 26th and April 25th   
    * **range**: Every year since 3760 BC, first year of the Jewish calendar  
    * **note**: Based on that year's Rosh Hashanah. Starts the sundown before reported date.
    
10. **Memorial Day**

    * **keyword**: "Memorial" or just "mrd"  
    * **occurs**: Last Monday in May since 1971, and May 30th every year from 1868 before that  
    * **range**: Every year since 1868, later standardized to Monday by the Uniform Monday Holiday Act
    
11. **Independence Day**

    * **keyword**: "Independence" or just "ipd"  
    * **occurs**: July 4th  
    * **range**: Every year since 1776
    
12. **Labor Day**

    * **keyword**: "Labor" or just "lbd"  
    * **occurs**: First Monday in September  
    * **range**: Every year since 1882
    
13. **Rosh Hashanah**

    * **keyword**: "Rosh" or "Hashanah" or just "rsh"  
    * **occurs**: Between September 5th and October 5th  
    * **range**: Every year since 3760 BC, first year of the Jewish calendar  
    * **note**: Calculated using a variation of Gauss's method. Starts the sundown before reported date.
    
14. **Yom Kippur**

    * **keyword**: "Rosh" or "Hashanah" or just "rsh"  
    * **occurs**: Between September 14th and October 14th  
    * **range**: Every year since 3760 BC, first year of the Jewish calendar  
    * **note**: 9 days after Rosh Hashanah. Starts the sundown before reported date.
    
15. **Columbus Day**

    * **keyword**: "Columbus" or just "cbd"  
    * **occurs**: Second Monday in October  
    * **range**: Every year since 1971 via the Uniform Monday Holiday Act

16. **Halloween**

    * **keyword**: "Halloween" or just "hlw"  
    * **occurs**: First Monday in September  
    * **range**: Every year since 1911 (when the holiday became commonly celebrated)
    
 17. **Election Day**
 
     * **keyword**: "election" or just "eld"   
     * **occurs**: First Tuesday in November   
     * **range**: Every year since 1845, when it was standardized
    
 18. **Veteran's Day**
 
     * **keyword**: "Veterans" or "veteran" or just "vtd"  
     * **occurs**: November 11th  
     * **range**: Every year since 1919 (after WW1)
    
 19. **Thanksgiving Day**
 
     * **keyword**: "Thanksgiving" or just "tgd"  
     * **occurs**: Fourth Thursday in November  
     * **range**: Every year since 1863, when it was made a national holiday
    
 20. **Christmas Day**
 
     * **keyword**: "christmas" or just "chr"  
     * **occurs**: December 25th  
     * **range**: Every year since the start of the common era (very generally speaking)

Finally, the holiday keyword "all" will put all of that year's holidays on to the queue and report them all to the terminal:

![alt tag](https://github.com/zakagan/DoomsdayCalc/blob/master/screenshots/all.png)

## The Julian-Gregorian switch

In October, 1582, Pope Gregory XIII reformed the Julian calendar into the Gregorian calendar we use today. The one main difference between the two calendars is the number of leap days. Leap days in the Julian Calendar happen ever four years. However there are less than 365.25 days in a year, meaning there are too many leap days in the Julian system. The Gregorian calendar corrects this by removing leap days from every year divisible by 100 but not 400.

In order to facilitate the switch, 10 days were omitted from the calendar between October 4th, 1582 and October 15th. While the new calendar was slowly adopted across Europe and eventually the world, DoomsdayCalc considers these days to not have existed. Holidays and significant dates before October 4th, 1582 follow the Julian calendar, and afterwards follow the Gregorian calendar. This extends to holidays like Easter and Passover, which are calculated slightly differently in each calendar system.

## Usage message

```
usage: ././DoomsdayCalc {-D date | -y year -m month -d day | -y year -h holiday} [-a | -b] ... [-e | -r] [-s separator]
    -D/--Date: formatted date string e.g. 7/20/1969
    -y/--year, -m/--month, -d/--day: segmented numerical portions of a date. Accepted year range is 1 to 99,999 AD or BC.
    -h/--holiday: when paired with a year, gives the holiday date for that year
    -a/--AD/--CE | -b/--BC/--BCE: sets the date provided as either AD or BC (AD is default)
    -e/--European | -r/--American: formats reported dates using either American or European standards (American is default)
    -s/separator: sets the separator between formatted dates (/ is default as in 9/13/1959)
```
