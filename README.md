# DoomsdayCalc

This project implements John Conway's Doomsday algoritm to find the day of the weekfor user provided dates. Dates are parsed from the command line using the getopt standard, with several options for entering dates. When multiple dates are provided the number of days between dates is reported.

No built-in date or time libraries have been used. The programming behind this project is broken into two main parts:

1. Parsing and organizing user provided date formats

2. Processing them to find the day of the week and distance between dates.

These two elements come together to handle holidays. Some holidays occur on specifics days of the weeek, like Easter Sunday or Memorial Day Monday. Users can provide a year and a holiday and DoomsdayCalc will find the date where that holiday falls in that year, along with the day of the week. If the user enters in a date that turns out to be a holiday, DoomsdayCalc will recognize it as well.

## Example Problem

How many days are there between Thanksgiving and Christmas? While Christmas is always on Dec 25th, Thanksgiving falls on the 4th Thursday in November. That means the traditional Christmas season doesn't have a fixed length, it varies from year to year.

This question can be answered by DoomsdayCalc for any provided year:

`./DoomsdayCalc -y 2017 -h Thanksgiving -y 2017 -h Christmas`

These series of inputs leads the program to crate two DoomsdayDate objects, one for Thanksgiving 2017 and one for Christmas 2017. Each object stores it's date, significance (if it's a holiday and if so what holiday), and day of the week. Then this information is reported back to the user, along with the number of days difference between the two dates.

But DoomsdayCalc can handle a lot more than just two dates at a time. A priority queue organizes the DoomsdayDate objects into chronlogical order, and they are reported back accordingly along with the days between them. By doing this we can find the span of holiday seasons across different years.

`./DoomsdayCalc -y 2017 -h Thanksgiving -y 2016 -h Thanksgiving -y 2015  -h Thanksgiving -y 2017 -h Christmas -y 2016 -h Christmas -y 2015 -h Christmas`

## Options for Date Formatting

There are three ways for users to input dates.

1. A standard formatted date e.g. 7/20/1969. Slashes do not need to be used as seperators, since DoomsdayCalc uses a regex expression to extract date information. This format uses the -D flag or --Date flag.

2. A segmented dat e.g. -d 20 -m 20 -y 1969. This format allows the user to explicitly list each indvigual elements of the date being inputted, where the -d flag takes in the day of the month, the -m flag takes in the month, and the -y flag takes in the year. These flags also have the following longer options, respectively: --day, --month, --year.

3. A year and holiday pair, as shown in the previous section. Give any holiday string using the flag -h or the longer flag --holiday. A list of implamented holidays is provided below.

Each date can be augmented with the flags --BC/--BCE or --AD/--CE to specify whether or not it takes place before the start of the common era. The shorter version of these flags are -b and -a respectively. By defualt DoomsdayCalc assumes a date takes place in the common era.

By defualt dates are also parsed and reported using the American date system (meaning the month is given before the day of the month). To tell DoomadayCalc to use the European system simply pass an -e flag or a longer --European flag. The user may also input -r or --American to switch back to the American system.

Finally, dates are reported with slashes ('/') as seperators by default, but this can be changed by passing a new seperator with the -s or --separator flag.

The following is an example of how all these options may be used.

## Notes on the Doomsday Algorithm

There are lot of resources about John Conway's Doomsday Algorithm (also called the Doomsday Rule). It works by finding anchors, which Conway calls Doomsdays, and then combines them to find the day of the week for the provided date.

For a detailed xplaination on the method, [please read these lecture notes from Mathematics Prof. S.W. Graham](http://people.cst.cmich.edu/graha1sw/Pub/Doomsday/Doomsday.html).

## Notes on holidays

Holidays strings can be provided using any number of keywords. Strings are searched for appropriate key words and the proper holidays are added to the queue of DoomsdayDate objects. The strings are not cap sensitive (in fact, all non-letter chars are stripped before searching), and shorthand 3 letter abbreviations can be used instead of a holiday's full name.

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
    **range**: In Gregorian calander, every four years except every year divisible by 100, but including years divisible by both 100 and 4. In the Julian calander it is simply every fout years.
    
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
