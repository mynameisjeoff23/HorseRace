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
	std::vector<std::vector<boundary>> boundaries(m_width, std::vector<boundary>(m_height));
	
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

	//TODO: none of this is really necessary vvvv
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
