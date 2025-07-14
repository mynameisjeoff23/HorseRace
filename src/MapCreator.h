#pragma once
#include <string>
#include <iostream>
#include <vector>
#include <queue>
#include <array>

enum color {
	BLACK = 0,
	WHITE = 1, 
	GRAY = 2
};

struct area {
	int x, y; //first pixel to come up in search
	int size; //number of pixels in area
};

struct point {
	int x, y;
	color color;
};

struct line {

	int x, y;
	float slope; 
};

struct boundary {

	int x, y;
	float normal; //normal is angle in radians
	//bool isSpace; //use only with 2d array approach
};


class HorseMap {
public:
	HorseMap(std::string& filename);
private:
	int m_width, m_height;
	unsigned char* m_mapImage;
	std::vector<std::vector<point>> m_basicMap;
	std::vector<area> m_areas;
	std::vector<boundary> m_boundaries;
	std::queue<std::array<int, 2>> m_floodFillQueue;
	point m_largestAreaStart;

	void openImage(std::string& filepath);
	void stringToVector();
	/**
	 * @brief Finds the largest contiguous white area in the map.
	 *
	 * This function iterates through the map, identifying all contiguous areas of white pixels
	 * using a flood fill algorithm (exploreArea). Each discovered area is recorded with its starting
	 * coordinates and size. After all areas are found, the largest area is determined and its starting
	 * point is stored in m_largestAreaStart.
	 *
	 * The function modifies m_areas to store all found areas and updates m_largestAreaStart
	 * to the starting point of the largest area.
	 */
	void findLargestArea();

	/**
	* @brief Explores a contiguous area in the map starting from the given pixel using a flood fill algorithm.
	*
	* This function marks all connected pixels of the specified color (default: color::WHITE) as visited (color::GRAY).
	* It uses a queue-based approach to traverse the area, updating the map in-place.
	*
	* @param pixel The starting point for the area exploration.
	* @param countThisColor The color to count during exploration. If color::BLACK, counts black pixels; if color::GRAY, counts gray pixels; default is color::WHITE.
	* @return int The number of pixels counted according to countThisColor.
	*
	* @note color::GRAY should never be used as a parameter, as it is a temporary color used to mark visited pixels.
	*/
	int exploreArea(point start, color countThisColor = color::WHITE);

    /**
    * @brief Selects the largest contiguous white area found in the map.
    *
    * This function highlights or marks the largest area previously identified by findLargestArea().
    * It resets m_basicMap vector to be black and white, and utilizes exploreArea() to mark the largest area.
	* It counts the number of bounding black pixels, and creates a list of boundaries to the area.
    *
    * The function assumes that findLargestArea() has already been called and that m_largestAreaStart
    * is set to the starting point of the largest area.
    */
    void selectLargestArea();

	/**
	* @brief Counts the number of boundary pixels in the area of the given point.
	* 
	* This function uses the flood fill algorithm in exploreArea() to count the number 
	*/
	int countBoundaries(point& start);
};