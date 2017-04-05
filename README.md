# DoomsdayCalc

This project implements John Conway's Doomsday algoritm to find the day of the weekfor user provided dates. Dates are parsed from the command line using the getopt standard, with several options for entering dates. When multiple dates are provided the number of days between dates is reported.

No built in date or time libraries have been used. The programming behind this project is broken into two main parts:

1. Parsing and organizing user provided date formats

2. Processing them to find the day of the week and distance between dates.

These two elements come together to handle holidays. Some holidays occur on specifics days of the weeek, like Easter Sunday or Memorial Day Monday. Users can provide a year and a holiday and DoomsdayCalc will find the date where that holiday falls in that year, along with the day of the week. If the user enters in a date that turns out to be a holiday, DoomsdayCalc will recognize it as well.

## Example Problem

How many days are there between Thanksgiving and Christmas? While Christmas is always on Dec 25th, Thanksgiving falls on the 4th Thursday in November. That means the traditional Christmas season doesn't have a fixed length, it varies from year to year.

This question can be answered by DoomsdayCalc for any provided year:

`./DoomsdayCalc -y 2017 -h Thanksgiving -y 2017 -h Christmas`

These series of inputs leads the program to crate two DoomsdayDate objects, one for Thanksgiving 2017 and one for Christmas 2017. Each object stores it's date, significance (if it's a holiday and if so what holiday), and day of the week. Then this information is reported back to the user, along with the number of days difference between the two dates.

But DoomsdayCalc can handle a lot more than just two dates at a time. A priority queue organizes the DoomsdayDate objects into chronlogical order, and they are reported back accordingly along with the days between them. By doing this we can find the span of holiay seasons across different years.

`./DoomsdayCalc -y 2017 -h Thanksgiving -y 2016 -h Thanksgiving -y 2015  -h Thanksgiving -y 2017 -h Christmas -y 2016 -h Christmas -y 2015 -h Christmas`

## Options for Date Formatting

There are three ways for users to input dates.

1. A standard formatted date e.g. 7/20/1969. Slashes do not need to be used as seperators, since DoomsdayCalc uses a regex expression to extract date information. This format uses the -D flag or --Date flag.

2. A segmented dat e.g. -d 20 -m 20 -y 1969. This format allows the user to explicitly list each indvigual elements of the date being inputted, where the -d flag takes in the day of the month, the -m flag takes in the month, and the -y flag takes in the year. These flags also have the following longer options, respectively: --day, --month, --year.

3. A year and holiday pair, as shown in the previous section.

Each date can be augmented with the flags --BC/--BCE or --AD/--CE to specify whether or not it takes place before the start of the common era. The shorter version of these flags are -b and -a respectively. By defualt DoomsdayCalc assumes a date takes place in the common era.

By defualt dates are also parsed and reported using the American date system (meaning the month is given before the day of the month). To tell DoomadayCalc to use the European system simply pass an -e flag or a longer --European flag. The user may also input -r or --American to switch back to the American system.

Finally, dates are reported with slashes ('/') as seperators by default, but this can be changed by passing a new seperator with the -s or --separator flag.

The following is an example of how all these options may be used.

## Notes on the Doomsday Algorithm

## Notes on holidays
