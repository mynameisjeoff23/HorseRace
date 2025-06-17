#include "MapCreator.h"
#include "vendor/stb_image/stb_image.h"
#include <vector>
#include <iostream>
#include <fstream>
#include <stdexcept>

HorseMap::HorseMap(std::string& filepath) {

	//possible TODO: 
	//open image
	openImage(filepath);

	//reserve space for image sized map
	m_basicMap = std::vector<std::vector<point>> (m_height, std::vector<point>(m_width));
	
	// find largest contiguous section
		// master controller function()
			// inside-> small map_area function()
				// find boundaries	
				// color areas black
				// find size
			// make list of areas, choose the one with the biggest area
	
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

	try {
		std::ofstream stream("ImageProcessingOutput/output.txt", std::ios::out);
		if (stream) {

			for (int i = 0; i < m_width * m_height; ++i) {

				char c = m_mapImage[i] ? '.' : 'X';
				stream << c;

				if (!((i + 1) % m_width)) 
					stream << '\n';
			}
		}
		stream.close();
	}
	catch (std::runtime_error& e) {
		std::cout << "you stupid" << std::endl;
	}
}

void HorseMap::stringToVector() {

	for (int y = 0; y < m_width; ++y) {
		for (int x = 0; x < m_width; ++x) {
			if (m_mapImage[x * y + x] == 255) {

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
}

void HorseMap::exploreArea(point &start) {


}

void HorseMap::fillNextPixels(point& pixel) {
	if (pixel.color == color::WHITE) {
		//TODO: extend the vectors by 1
		//in x and y so that coords match with indices

		pixel.color = color::BLACK; //make ineligible for seeking again
		++m_areaSize;

		int a;
		a = pixel.x - 1;
		if (pixel.x > 0)
			fillNextPixels(m_basicMap[pixel.y][a]);	

		a = pixel.x + 1;
		if (pixel.x < (m_width - 1))
			fillNextPixels(m_basicMap[pixel.y][a]);	

		a = pixel.y - 1;
		if (pixel.y > 0)
			fillNextPixels(m_basicMap[a][pixel.x]);

		a = pixel.y + 1;
		if (pixel.y < (m_height - 1))
			fillNextPixels(m_basicMap[a][pixel.x]);	
	}
}
