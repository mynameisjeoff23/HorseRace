#pragma once
#include <string>
#include <iostream>
#include <vector>

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
	unsigned char* m_mapImage;

	void openImage(std::string& filepath);

};