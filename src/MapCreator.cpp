#include "MapCreator.h"
#include "vendor/stb_image/stb_image.h"
#include <vector>
#include <iostream>
#include <fstream>
#include <stdexcept>

HorseMap::HorseMap(std::string& filepath) {
	//open image
	int channels;
	const unsigned char* mapImage = stbi_load(filepath.c_str(), &width, &height, &channels, 1);
	std::vector<std::vector<boundary>> boundaries(width, std::vector<boundary>(height));
	std::cout << "Channels: " << channels << std::endl;
	std::cout << "Dimensions: " << width << " x " << height 
			  << " = " <<  width * height << std::endl;

	try {
		std::ofstream stream("ImageProcessingOutput/output.txt", std::ios::out);
		if (stream) {

			for (int i = 0; i < width * height; ++i) {

				char c = mapImage[i] ? '.' : 'X';

				stream << c;

				if (!((i + 1) % width)){
					
					stream << '\n';
				}
			}
		}
		stream.close();
	}
	catch (std::runtime_error& e) {
		std::cout << "you stupid" << std::endl;
	}

	//check if only black and white
	//find largest contiguous section
	//indentify boundaries of largest section
	}