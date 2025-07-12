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
	bool isSpace;
};


class HorseMap {
public:
	HorseMap(std::string& filename);
private:
	int m_width, m_height;
	bool select = false;
	unsigned char* m_mapImage;
	std::vector<std::vector<point>> m_basicMap;
	std::vector<area> m_areas;
	std::queue<std::array<int, 2>> m_floodFillQueue;
	point m_largestAreaStart;

	void openImage(std::string& filepath);
	void stringToVector();
	void findLargestArea();
	int exploreArea(point start);
	void resetMapVector();
	void selectLargestArea();
};