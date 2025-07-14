#include "MapCreator.h"
#include "vendor/stb_image/stb_image.h"
#include <vector>
#include <iostream>
#include <fstream>
#include <stdexcept>

//TODO: temporary
static void outputMap(std::vector<std::vector<point>>& map, int width, int height) {
	std::ofstream stream("ImageProcessingOutput/output.txt", std::ios::out);
	if (stream) {
		for (int y = 0; y < height; ++y) {
			for (int x = 0; x < width; ++x) {
				if (map[y][x].color == color::WHITE) {
					stream << ':';
				}
				else if (map[y][x].color == color::BLACK) {
					stream << ' ';
				}
				else if (map[y][x].color == color::GRAY) {
					stream << 'X'; 
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

	//open image
	openImage(filepath);

	//reserve space for image sized map
	m_basicMap = std::vector<std::vector<point>> (m_height, std::vector<point>(m_width));
	
	stringToVector(); // convert image to vector of points
	outputMap(m_basicMap, m_width, m_height); // output map to file
	findLargestArea(); // find largest area in the map
	std::cout << "Number of areas found: " << m_areas.size() << std::endl;
	std::cin.get(); // wait for user input to continue
	selectLargestArea(); // select the largest area in the map
	outputMap(m_basicMap, m_width, m_height); // output map to file again
	
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
	int areaSize;
	for (auto& row : m_basicMap) {
		for (auto& pixel : row) {
			if (pixel.color == color::WHITE) {
				areaSize = exploreArea(pixel); //explore the area starting from this pixel
				if (areaSize > 0) { //if we found an area
					m_areas.push_back({ pixel.x, pixel.y, areaSize });
				}
			}
		}
	}
		
	int maxIndex{0};
	int maxSize{0};
	for (int i = 0; i < m_areas.size(); ++i) {

		if (m_areas[i].size > maxSize) {
			maxIndex = i;
			maxSize = m_areas[i].size;
		}
	}

	m_largestAreaStart = { m_areas[maxIndex].x, m_areas[maxIndex].y, color::WHITE };
}


int HorseMap::exploreArea(point pixel, color countThisColor) {

	int count{ 0 };
	m_floodFillQueue.push({ pixel.x, pixel.y });

	int x, y;
	while (!m_floodFillQueue.empty()) {
		
		x = m_floodFillQueue.front()[0];
		y = m_floodFillQueue.front()[1];
		m_floodFillQueue.pop();

		if (x < 0 || x >= m_width || y < 0 || y >= m_height)
			continue; //out of bounds

		pixel = m_basicMap[y][x]; //get the pixel from the map

		if (countThisColor == color::BLACK && pixel.color == color::BLACK) {
			++count;
			continue;
		}

		if (pixel.color != color::WHITE) 
			continue;

		pixel.color = color::GRAY; //mark the pixel as visited

		if (countThisColor == color::WHITE) {
			++count;
		}

		m_basicMap[y][x].color = pixel.color; //update the pixel in the map

		//push the neighboring pixels to the queue
		m_floodFillQueue.push({ x - 1, y }); //left
		m_floodFillQueue.push({ x + 1, y }); //right
		m_floodFillQueue.push({ x, y - 1 }); //up
		m_floodFillQueue.push({ x, y + 1 }); //down

	}

	return count;
}

void HorseMap::selectLargestArea() {

	stringToVector(); // reset map vector to original state
	//TODO: at this point, what it counts are bounding edges, not bounding pixels
	int numberBoundaries = countBoundaries(m_largestAreaStart); //count the boundaries of the largest area
	std::cout << "Number of boundaries: " << numberBoundaries << std::endl;

}

int HorseMap::countBoundaries(point& start) {

	int count = exploreArea(start, color::BLACK); 
	return count;
}
