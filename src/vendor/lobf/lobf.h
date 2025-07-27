/**
 *
 * Copyright (c) 2016 Ryan Antkowiak
 * All rights reserved.
 * It is provided AS_IS, WITHOUT ANY WARRANTY either expressed or implied.
 * You may study, use, and modify it for any non-commercial purpose.
 * You may distribute it non-commercially as long as you retain this notice.
 * For a commercial license, contact .
 *
 * @author Ryan Antkowiak
 *
 *
 * Simple program that calculates a "Line of Best Fit" using the "Least Square Method."
 *
 **/

#pragma once
#include <cstdlib>
#include <iostream>
#include <sstream>
#include <string>
#include <vector>

 // The type to use for numbers
typedef double TNumber;

// A line consists of a slope and a y-intercept
struct LOBFLine
{
    LOBFLine() : slope(0), y_intercept(0) {}
    LOBFLine(const TNumber& s, const TNumber& yInt) : slope(s), y_intercept(yInt) {}

    std::string toString() const;

    TNumber slope;
    TNumber y_intercept;
};

// 2-dimensional point (x, y)
struct LOBFPoint
{
    LOBFPoint() : x(0), y(0) {}
    LOBFPoint(const TNumber& x0, const TNumber& y0) : x(x0), y(y0) {}

    std::string toString() const;

    TNumber x;
    TNumber y;
};

// Calculate a line of best fit, using the least square method
LOBFLine CalculateLineOfBestFit(const std::vector<LOBFPoint>& points);
