/*ideas:

	use line of best fit algorithm from
	https://github.com/antkowiak/LineOfBestFit/blob/master/lobf.cpp
	to find the line of best fit from the immediate points in like a 2 radius
	and extract a normal vector from that

	to distinguish points that are at the edge, only add the points (black)
	that are next to white points to the list of points which
	will be used to find the line of best fit. 

	Instead of checking if a horse is within collision distance of every boundary,
	only check if within collision distance to points that are within a certain area.
	This would require a map to be stored as as a 2d array, where each point 
	is either a boundary or open space.

	First try the approach of an array of boundaries. If performance is bad, try 2d array approach. 
*/

#include "MapCreator.h"
#include <string>

int main() {
	//set up window and buttons
	//load and process image
	std::string filepath = "res/maps/beutifulimage.png";
	HorseMap map = HorseMap(filepath);
	//run simulation
	return 0;
}