# DoomsdayCalc

This project implements John Conway's Doomsday algorithm to find the day of the week for user provided dates. Dates are parsed from the command line using the getopt standard, with several options for entering dates. When multiple dates are provided the number of days between dates is reported.

I started with some basic code from [a 2002 page on Code Project](https://www.codeproject.com/articles/2501/any-day-of-the-week-using-the-doomsday-rule). I noticed some issues and decided to build a more complete version of what orginal author Paul J. Weiss uploaded. No built-in date or time libraries have been used. The programming behind this project is broken into two main parts:

1. Parsing and organizing user provided date formats

2. Processing them to find the day of the week and distance between dates.

These two elements come together to handle holidays. Some holidays occur on specifics days of the weeek rather than specific dates, like Easter Sunday or Memorial Day Monday. Users can provide a year and a holiday and DoomsdayCalc will find the date where that holiday falls in that year, along with the day of the week. If the user enters in a date that turns out to be a holiday, DoomsdayCalc will recognize it as well.

## Example Problem

How many days are there between Thanksgiving and Christmas? While Christmas is always on Dec 25th, Thanksgiving falls on the 4th Thursday in November. That means the traditional Christmas season doesn't have a fixed length, it varies from year to year.

This question can be answered by DoomsdayCalc for any provided year.

`./DoomsdayCalc -y 2017 -h Thanksgiving -y 2017 -h Christmas`

The output looks like this:

https://raw.githubusercontent.com/zakagan/DoomsdayCalc/screenshots/christmas_1.png

These series of inputs leads the program to create two DoomsdayDate objects, one for Thanksgiving 2017 and one for Christmas 2017. Each object stores it's date, significance (if it's a holiday and if so what holiday), and day of the week. Then this information is reported back to the user, along with the number of days difference between the two dates.

But DoomsdayCalc can handle a lot more than just two dates at a time. A priority queue organizes the DoomsdayDate objects into chronlogical order, and they are reported back accordingly along with the days between them. By doing this we can find the span of holiday seasons across different years.

`./DoomsdayCalc -y 2017 -h Thanksgiving -y 2016 -h Thanksgiving -y 2015  -h Thanksgiving -y 2017 -h Christmas -y 2016 -h Christmas -y 2015 -h Christmas`

The output looks like this:

What if a user enters a date that turns out to be a holiday? DoomsdayCalc can recognize these dates are will report back the significance. For example:

What about older dates? The famous Battle of Hastings between the Normans and the Anglo-Saxons occured on a Saturday, October 14th, 1066. Does DoomsdayCalc get it right?

## Options for Date Formatting

There are three ways for users to input dates.

1. A standard formatted date e.g. 7/20/1969. Slashes do not need to be used as seperators, since DoomsdayCalc uses a regex expression to extract date information. However, some non-numerical seperator is required. This format uses the -D flag or --Date flag.

2. A segmented dat e.g. -d 20 -m 20 -y 1969. This format allows the user to explicitly list each indvigual elements of the date being inputted, where the -d flag takes in the day of the month, the -m flag takes in the month, and the -y flag takes in the year. These flags also have the following longer options, respectively: --day, --month, --year. They do not need to be provided in any particular order.

3. A year and holiday pair, as shown in the previous section. Give any holiday string using the flag -h or the longer flag --holiday. A list of implamented holidays is provided below.

Each date can be augmented with the flags --BC/--BCE or --AD/--CE to specify whether or not it takes place before the start of the common era. The shorter version of these flags are -b and -a respectively. By defualt DoomsdayCalc assumes a date takes place in the common era.

By defualt dates are also parsed and reported using the American date system (meaning the month is given before the day of the month). To tell DoomadayCalc to use the European system simply pass an -e flag or a longer --European flag. The user may also input -r or --American to switch back to the American system.

Finally, dates are reported with slashes ('/') as seperators by default, but this can be changed by passing a new seperator with the -s or --separator flag.

The following is an example of how all these options may be used.

## Notes on the Doomsday Algorithm

There are lot of resources about John Conway's Doomsday Algorithm (also called the Doomsday Rule). It works by finding anchors, which Conway calls Doomsdays, and then combines them to find the day of the week for the provided date.

For a detailed explaination on the method, [please read these lecture notes from Mathematics Prof. S.W. Graham](http://people.cst.cmich.edu/graha1sw/Pub/Doomsday/Doomsday.html).

## Notes on holidays

Holidays strings can be provided using any number of keywords. Strings are searched for appropriate key words and the proper holidays are added to the queue of DoomsdayDate objects. The strings are not cap sensitive (in fact, all non-letter chars are stripped before searching), and shorthand 3 letter abbreviations can be used instead of a holiday's full name.

For the most part, the included holidays are based on dates observed in the United States.

The following Holidays are implemented:

1. **New Year's Day**

    **keyword**: "New Years" or just "nyd"  
    **occurs**: January 1st  
    **range**: Every year has a New Year's Day

2. **Martin Luther King Jr. Day**

    **keyword**: "Martin Luther King" or just "mlk"  
    **occurs**: 3rd Monday in January  
    **range**: Every year since 1986
    
3. **Ground Hog Day**

    **keyword**: "Ground Hog" or just "ghd"  
    **occurs**: Febuary 2nd  
    **range**: Every year since 1887
    
4. **Valentine's Day**

    **keyword**: "Valentines" or "valentine" or just "vld"  
    **occurs**: Febuary 14th  
    **range**: Every year since 496 (using a loose interpretation of the Valentine's celebration)
    
5. **Washington's Birthday (President's Day)**

    **keyword**: "Washington" or "Lincoln" or "president" or just "wbd"  
    **occurs**: 3rd Monday in Feburary  
    **range**: Every year since 1971
    
6. **Leap Day**

    **keyword**: "Leap Day" or "leap" or just "lpd"  
    **occurs**: Feburary 29th  
    **range**: In Gregorian calander, every four years except every year divisible by 100, but including years divisible by both 100 and 400. In the Julian calander it is simply every four years.
    
7. **Good Friday**

    **keyword**: "Good Friday" or just "gfd"  
    **occurs**: A Friday between March 20th and April 23rd, two days before Easter.  
    **range**: Every year since 325 AD, the First Council of Nicaea.
        
8. **Easter**

    **keyword**: "Easter" or just "est"  
    **occurs**: A Sunday between March 22th and April 25rd  
    **range**: Every year since 325 AD, the First Council of Nicaea.  
    **note**: Easter is calculated using John Conway's algorithm
    
9. **Passover**

    **keyword**: "Passover" or just "psv"  
    **occurs**: Between March 26th and April 25th   
    **range**: Every year since 3760 BC, first year of the Jewish calander  
    **note**: Based on that year's Rosh Hashanah. Starts the sundown before reported date.
    
10. **Memorial Day**

    **keyword**: "Memorial" or just "mrd"  
    **occurs**: Last Monday in May since 1971, and May 30th every year from 1868 before that  
    **range**: Every year since 1868, later standardized to Monday by the Uniform Monday Holiday Act
    
11. **Independence Day**

    **keyword**: "Independence" or just "ipd"  
    **occurs**: July 4th  
    **range**: Every year since 1776
    
12. **Labor Day**

    **keyword**: "Labor" or just "lbd"  
    **occurs**: First Monday in September  
    **range**: Every year since 1882
    
13. **Rosh Hashanah**

    **keyword**: "Rosh" or "Hashanah" or just "rsh"  
    **occurs**: Between September 5th and October 5th  
    **range**: Every year since 3760 BC, first year of the Jewish calander  
    **note**: Calculated using a variation of Gauss's method. Starts the sundown before reported date.
    
14. **Yom Kippur**

    **keyword**: "Rosh" or "Hashanah" or just "rsh"  
    **occurs**: Between September 14th and October 14th  
    **range**: Every year since 3760 BC, first year of the Jewish calander  
    **note**: 9 days after Rosh Hashanah. Starts the sundown before reported date.
    
15. **Columbus Day**

    **keyword**: "Columbus" or just "cbd"  
    **occurs**: Second Monday in October  
    **range**: Every year since 1971 via the Uniform Monday Holiday Act

16. **Halloween**

    **keyword**: "Halloween" or just "hlw"  
    **occurs**: First Monday in September  
    **range**: Every year since 1911 (when the holiday became commonly celebrated)
    
 17. **Election Day**
 
    **keyword**: "election" or just "eld"   
    **occurs**: First Tuesday in November   
    **range**: Every year since 1845, when it was standardized
    
 18. **Veteran's Day**

    **keyword**: "Veterans" or "veteran" or just "vtd"  
    **occurs**: November 11th  
    **range**: Every year since 1919 (after WW1)
    
 19. **Thanksgiving Day**

    **keyword**: "Thanksgiving" or just "tgd"  
    **occurs**: Fourth Thursday in November  
    **range**: Every year since 1863, when it was made a national holiday
    
 20. **Christmas Day**

    **keyword**: "christmas" or just "chr"  
    **occurs**: December 25th  
    **range**: Every year since the start of the common era (very generally speaking)
    
Finally, the holiday keyword "all" will put all of that year's holidays on to the queue, and report them all to the terminal:

## The Julian-Gregorian switch

In October, 1582, Pope Gregory XIII reformed the Julian calander into the Gregorian calander that we use today. The one main difference bwtee the two calanders is the number of leap days. Leap days in the Julian Calander happen ever four years, which is too often. The Gregorian calander corrects this by removing leap days from every year divisible by 100 but not 400.

In order to facilitate the switch, 10 days were omitted from the calander between October 4th, 1582 and October 15th. While the new calander was slowly adopted across Europe and eventually the world, DoomsdayCalc considers these days to not have existsed. Holidays and significant dates before October 4th, 1582 follow the Julian calander, and afterwards follow the Gregorian calander. This extends to holidays like Easter and Passover, which are calaculated slightly differently in each calander system.

## Usage

```
usage: ././DoomsdayCalc {-D date | -y year -m month -d day | -y year -h holiday} [-a | -b] ... [-e | -r] [-s separator]
    -D/--Date: formatted date string e.g. 7/20/1969
    -y/--year, -m/--month, -d/--day: segmented numerical portions of a date
    -h/--holiday: when paired with a year, gives the holiday date for that year
    -a/--AD/--CE | -b/--BC/--BCE: sets the date provided as either AD or BC (AD is default)
    -e/--European | -r/--American: formats reported dates using either American or European standards (American is default)
    -s/separator: sets the seperator between formatted dates (/  is default as in 9/13/1959)
```
