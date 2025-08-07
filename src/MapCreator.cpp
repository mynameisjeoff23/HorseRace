#include "MapCreator.h"

// TODO: temporary
static void outputMap(std::vector<std::vector<point>>& map, int width, int height) {
	std::ofstream stream("ImageProcessingOutput/output.txt", std::ios::out);
	if (stream) {
		for (int y = 0; y < height; ++y) {
			for (int x = 0; x < width; ++x) {
				if (map[y][x].color == color::WHITE) {
					stream << ':';
				}
				else if (map[y][x].color == color::BLACK) {
					stream << '.';
				}
				else if (map[y][x].color == color::GRAY) {
					stream << 'X';
				}
				else if (map[y][x].color == color::RED)
					stream << "@";
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

	// open image
	openImage(filepath);

	// reserve space for image sized map
	m_basicMap = std::vector<std::vector<point>> (m_height, std::vector<point>(m_width));
	
	stringToVector();
	darkenMapBorders();
	// outputMap(m_basicMap, m_width, m_height); // output map to file
	findLargestArea(); // find largest area in the map
	std::cout << "Number of areas found: " << m_areas.size() << std::endl;
	
	// indentify boundaries of largest section
	selectLargestArea(); // select the largest area in the map
	outputMap(m_basicMap, m_width, m_height); // output map to file again
	
	m_boundaryMap = std::vector<std::vector<boundary>>(m_height, std::vector<boundary>(m_width));

	}

void HorseMap::openImage(std::string& filepath) {

	int channels;
	m_mapImage = stbi_load(filepath.c_str(), &m_width, &m_height, &channels, 1);

	// TODO: none of this is really necessary |
	//										 V
	std::cout << "Channels: " << channels << std::endl;
	std::cout << "Dimensions: " << m_width << " x " << m_height
		<< " = " << m_width * m_height << std::endl;

}

void HorseMap::stringToVector() {

	for (int y = 0; y < m_height; ++y) {
		for (int x = 0; x < m_width; ++x) {

			if (m_mapImage[m_width * y + x] == 0) 
				m_basicMap[y][x] = { x, y, color::BLACK }; // black pixel
			else
				m_basicMap[y][x] = { x, y, color::WHITE }; // white pixel
		}
	}
}

void HorseMap::darkenMapBorders() {
	for (int i = 0; i < m_width; ++i) {
		int y = m_height - 1;
		m_basicMap[0][i].color = color::BLACK; // top border
		m_basicMap[y][i].color = color::BLACK; // bottom border
	}

	for (int i = 0; i < m_height; ++i) {
		int x = m_width - 1;
		m_basicMap[i][0].color = color::BLACK; // left border
		m_basicMap[i][x].color = color::BLACK; // right border
	}
}

void HorseMap::findLargestArea() {
	m_areas.reserve(10); // reserve space for 10 areas, can be changed later
	int areaSize;
	for (auto& row : m_basicMap) {
		for (auto& pixel : row) {
			if (pixel.color == color::WHITE) {
				areaSize = exploreArea(pixel); // explore the area starting from this pixel
				if (areaSize > 0) // if we found an area
					m_areas.push_back({ pixel.x, pixel.y, areaSize });
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
			continue; // out of bounds

		pixel = m_basicMap[y][x]; // get the pixel from the map

		if (countThisColor == color::BLACK && pixel.color == color::BLACK) {
			++count;

			// mark the pixel as visited
			// should make so pixels are counted instead of edges			
			m_basicMap[y][x].color = color::RED;	

			continue;
		}

		if (pixel.color != color::WHITE) 
			continue;

		pixel.color = color::GRAY; // mark the pixel as visited

		if (countThisColor == color::WHITE) 
			++count;
		

		m_basicMap[y][x].color = pixel.color; // update the pixel in the map

		// push the neighboring pixels to the queue
		m_floodFillQueue.push({ x - 1, y }); // left
		m_floodFillQueue.push({ x + 1, y }); // right
		m_floodFillQueue.push({ x, y - 1 }); // up
		m_floodFillQueue.push({ x, y + 1 }); // down

	}

	return count;
}

void HorseMap::selectLargestArea() {

	stringToVector(); // reset map vector to original state
	int numberBoundaries = countBoundaries(m_largestAreaStart); // count the boundaries of the largest area
	std::cout << "Number of boundaries: " << numberBoundaries << std::endl;

}

int HorseMap::countBoundaries(point& start) {

	int count = exploreArea(start, color::BLACK); 
	return count;
}

void HorseMap::makeBoundaryMap() {

	for (auto& row : m_basicMap) {
		for (auto& pixel : row) {
			if (pixel.color == color::RED) 
				m_boundaryMap[pixel.y][pixel.x] = processNearbyBoundaries(pixel); //set as boundary
			else 
				// TOOD: remove bool to save data if you really want to
				m_boundaryMap[pixel.y][pixel.x] = { pixel.x, pixel.y, 0.0f, false }; //set non-boundary
			
		}
	}
}

boundary HorseMap::processNearbyBoundaries(point& start) {
	//TODO: shrink function size
	boundary processedBoundary;
	LOBFLine lobf;
	std::vector<LOBFPoint> points;
	points.clear();

	// scan for other boundaries nearby
	for (int x = -3; x < 4; ++x) {
		for (int y = -3; y < 4; ++y) {

			int newX = start.x + x;
			int newY = start.y + y;

			// check if the new pixel is within bounds
			if (newX < 0 || newX >= m_width || newY < 0 || newY >= m_height)
				continue; //out of bounds

			if (m_basicMap[newY][newX].color == color::RED) 
				points.push_back(LOBFPoint(newX, newY)); // add the boundary point to the list
			
		}
	}

	// calculate the line of best fit for the points
	lobf = CalculateLineOfBestFit(points); 

	// calculate the normal vector
	double vectorX = lobf.slope;
	double vectorY = -1;

	// normalize normal vector;
	double magV = std::sqrt(vectorX * vectorX + vectorY * vectorY);
	if (magV != 0) {
		vectorX /= magV;
		vectorY /= magV;
	}

	// check if vector lands inside the area
	int checkX = start.x + (int)std::round(vectorX);
	int checkY = start.y + (int)std::round(vectorY);

	if (checkX < 0 || checkX >= m_width || checkY < 0 || checkY >= m_height) {
		std::cout << "Normal vector out of bounds: <" \
			<< checkX << ", " << checkY << ">" << std::endl;
		// flip the vector if out of bounds
		vectorX *= -1;
		vectorY *= -1;

		// flip check coords to other side
		checkX = start.x + (int)std::round(vectorX);
		checkY = start.y + (int)std::round(vectorY);
	}

	// if color is white, flip vector
	// if color is gray, vector is correct
	if (m_basicMap[checkY][checkX].color == color::WHITE) {
		vectorX *= -1;
		vectorY *= -1;
	}
	else if (m_basicMap[checkY][checkX].color == color::GRAY) {
		// do nothing, vector is correct
	}
	else {
		std::cout << "tiny little bug at (" << checkX << ", " << checkY << "): "
			<< m_basicMap[checkY][checkX].color << std::endl;
	}

	float normal = std::atan2(vectorY, vectorX); // calculate the angle of the normal vector

	// create the boundary object
	processedBoundary = { start.x, start.y, normal, true };
	return processedBoundary;
}
