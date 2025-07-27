#include "lobf.h"

std::string LOBFLine::toString() const
{
    std::stringstream ss;
    ss << "y = (" << slope << " * x) + (" << y_intercept << ")";
    return ss.str();
}

std::string LOBFPoint::toString() const
{
    std::stringstream ss;
    ss << "(" << x << ", " << y << ")";
    return ss.str();
}

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
