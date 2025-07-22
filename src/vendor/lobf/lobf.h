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

    std::string toString() const
    {
        std::stringstream ss;
        ss << "y = (" << slope << " * x) + (" << y_intercept << ")";
        return ss.str();
    }

    TNumber slope;
    TNumber y_intercept;
};

// 2-dimensional point (x, y)
struct LOBFPoint
{
    LOBFPoint() : x(0), y(0) {}
    LOBFPoint(const TNumber& x0, const TNumber& y0) : x(x0), y(y0) {}

    std::string toString() const
    {
        std::stringstream ss;
        ss << "(" << x << ", " << y << ")";
        return ss.str();
    }

    TNumber x;
    TNumber y;
};

// Calculate a line of best fit, using the least square method
LOBFLine CalculateLineOfBestFit(const std::vector<LOBFPoint>& points)
{
    if (points.empty())
    {
        return LOBFLine();
    }

    TNumber totalX = 0;
    TNumber totalY = 0;

    for (std::vector<LOBFPoint>::const_iterator i = points.begin(); i != points.end(); ++i)
    {
        totalX += i->x;
        totalY += i->y;
    }

    TNumber meanX = totalX / static_cast<TNumber>(points.size());
    TNumber meanY = totalY / static_cast<TNumber>(points.size());

    TNumber sumDeviationProducts = 0;
    TNumber sumXDeviationSquared = 0;

    for (std::vector<LOBFPoint>::const_iterator i = points.begin(); i != points.end(); ++i)
    {
        TNumber xDeviation = (i->x - meanX);
        TNumber yDeviation = (i->y - meanY);

        sumDeviationProducts += (xDeviation * yDeviation);
        sumXDeviationSquared += (xDeviation * xDeviation);
    }

    if (sumXDeviationSquared == 0)
    {
        return LOBFLine();
    }

    TNumber slope = sumDeviationProducts / sumXDeviationSquared;
    TNumber yInt = meanY - (slope * meanX);

    return LOBFLine(slope, yInt);
}