#include "MapCreator.h"
#include "vendor/stb_image/stb_image.h"
#include <vector>
#include <iostream>
#include <fstream>
#include <stdexcept>

//TODO: temporary
void outputMap(std::vector<std::vector<point>>& map, int width, int height) {
	std::ofstream stream("ImageProcessingOutput/output.txt", std::ios::out);
	if (stream) {
		for (int y = 0; y < height; ++y) {
			for (int x = 0; x < width; ++x) {
				if (map[y][x].color == color::WHITE) {
					stream << 'X';
				}
				else if (map[y][x].color == color::BLACK) {
					stream << ' ';
				}
				else if (map[y][x].color == color::GRAY) {
					stream << '/'; 
				}
				else {
					throw std::runtime_error("???");
				}
			}
			stream << '\n';
		}
	}
	stream.close();
}

HorseMap::HorseMap(std::string& filepath) {

	//possible TODO: 
	//open image
	openImage(filepath);

	//reserve space for image sized map
	m_basicMap = std::vector<std::vector<point>> (m_height, std::vector<point>(m_width));
	
	stringToVector(); // convert image to vector of points
	outputMap(m_basicMap, m_width, m_height); // output map to file
	findLargestArea(); // find largest area in the map
	stringToVector(); // reset map vector to original state
	
	// indentify boundaries of largest section
	}

void HorseMap::openImage(std::string& filepath) {

	int channels;
	m_mapImage = stbi_load(filepath.c_str(), &m_width, &m_height, &channels, 1);

	//TODO: none of this is really necessary |
	//										 V
	std::cout << "Channels: " << channels << std::endl;
	std::cout << "Dimensions: " << m_width << " x " << m_height
		<< " = " << m_width * m_height << std::endl;

}

void HorseMap::stringToVector() {

	for (int y = 0; y < m_height; ++y) {
		for (int x = 0; x < m_width; ++x) {
			if (m_mapImage[m_width * y + x] == 0) {

				m_basicMap[y][x] = { x, y, color::BLACK }; //black pixel
			}
			else {
				m_basicMap[y][x] = { x, y, color::WHITE }; //white pixel
			}
		}
	}
}

void HorseMap::findLargestArea() {

	m_areas.reserve(10); //reserve space for 10 areas, can be changed later
	for (auto& row : m_basicMap) {
		for (auto& pixel : row) {
			if (pixel.color == color::WHITE) {
				m_areaSize = 0; //reset area size
				exploreArea(pixel); //explore the area starting from this pixel
				if (m_areaSize > 0) { //if we found an area
					m_areas.push_back({ pixel.x, pixel.y, m_areaSize });
				}
			}
		}
	}
		
	int max = 0;
	for (int i = 0; i < m_areas.size(); ++i) {

		if (m_areas[i].size > max) {
			max = i;
		}
	}

	m_largestAreaStart = { m_areas[max].x, m_areas[max].y, color::WHITE };
}


void HorseMap::exploreArea(point& pixel) {
	//TODO: causes stack overflow

	if ((pixel.color == color::WHITE) && !select) {
		//TODO: extend the vectors by 1
		//in x and y so that coords match with indices
		// or not, it's just semantics


		//if we are not selecting an area, we want to
		//change the color of the pixel to black so that we can
		//mark it as visited and not eligible for seeking again
		pixel.color = color::BLACK;
		++m_areaSize;

		int a;
		a = pixel.x - 1;
		if (pixel.x > 0)
			exploreArea(m_basicMap[pixel.y][a]);	

		a = pixel.x + 1;
		if (pixel.x < (m_width - 1))
			exploreArea(m_basicMap[pixel.y][a]);	

		a = pixel.y - 1;
		if (pixel.y > 0)
			exploreArea(m_basicMap[a][pixel.x]);

		a = pixel.y + 1;
		if (pixel.y < (m_height - 1))
			exploreArea(m_basicMap[a][pixel.x]);	
	}
	else if ((pixel.color == color::WHITE) && select) {
		//if we are selecting an area, we want to keep the pixel
		//but change its color so that we can see it
		pixel.color = color::GRAY;

		int a;
		a = pixel.x - 1;
		if (pixel.x > 0)
			exploreArea(m_basicMap[pixel.y][a]);
		a = pixel.x + 1;
		if (pixel.x < (m_width - 1))
			exploreArea(m_basicMap[pixel.y][a]);
		a = pixel.y - 1;
		if (pixel.y > 0)
			exploreArea(m_basicMap[a][pixel.x]);
		a = pixel.y + 1;
		if (pixel.y < (m_height - 1))
			exploreArea(m_basicMap[a][pixel.x]);
	}

}

void HorseMap::resetMapVector() {

	stringToVector();
}

void HorseMap::selectArea(point& start) {

	select = true; //set select to true so that we can explore the area
	exploreArea(start);
	select = false; //reset select to false so that we don't select accidentally

}
