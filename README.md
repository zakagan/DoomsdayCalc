# DoomsdayCalc

This project implements John Conway's Doomsday algoritm to find the day of the weekfor user provided dates. Dates are parsed from the command line using the getopt standard, with several options for entering dates. When multiple dates are provided the number of days between dates is reported.

No built in date or time libraries have been used. The programming behind this project is broken into two main parts:

1. Parsing and organizing user provided date formats

2. Processing them to find the day of the week and distance between dates.

These two elements come together when it comes to holidays. Some holidays occur on specifics days of the weeek, like Easter Sunday or Memorial Day Monday. Users can provide a year and a holiday and DoomsdayCalc will find the date where that holiday falls in that year, along with the day of the week. If the user enters in a date that turns out to be a holiday, DoomsdayCalc will recognize it as well.

## Example Problem

How many days are there between Thanksgiving and Christmas? While Christmas is always on Dec 25th, Thanksgiving falls on the last Thursday of November. That means the traditional Christmas season doesn't have a fixed length, it varies from year to year.

This question can be answered by DoomsdayCalc for any provided year.
