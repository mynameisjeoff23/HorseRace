#include "MapCreator.h"

/*Process of creating a map:
* 1. Load image as black and white
* 2. Convert image to vector
*	- Darken borders of map
* 3. Find the largest area in the map
*	- Use flood fill algorithm to find all areas
*	- Find size by counting white pixels
*	- Mark visited pixels with gray
*	- Store areas in vector with starting point and size
* 4. Select the largest area
*	- Reset map to black and white
* 	- Use flood fill algorithm to mark the largest area with gray
* 	- Count the number of boundaries by counting black pixels next to gray pixels
*	- Mark boundaries with red
* 5. Create boundary map
*	- For each red pixel, find the line of best fit for nearby red pixels
* 	- Calculate the normal vector for the line of best fit
* 	- Store the boundary with its normal vector in the boundary map
*/

//Enormous TODO: remove temporary variables from class definition and make them passed as arguments instead
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
				else if (map[y][x].color == color::RED) {
					stream << "@";
				}
				else if (map[y][x].color == color::GREEN) {
					stream << "N";
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

	openImage(filepath);
	m_basicMap = std::vector<std::vector<point>> (m_height, std::vector<point>(m_width));
	
	stringToVector();
	findLargestArea(); 
	std::cout << "Number of areas found: " << m_areas.size() << std::endl;
	
	selectLargestArea();
	m_boundaryMap = std::vector<std::vector<boundary>>(m_height, std::vector<boundary>(m_width));
	makeBoundaryMap(); 

	outputMap(m_basicMap, m_width, m_height); 
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
				m_basicMap[y][x] = { x, y, color::BLACK };
			else
				m_basicMap[y][x] = { x, y, color::WHITE };
		}
	}
	darkenMapBorders();
}

void HorseMap::darkenMapBorders() {
	for (int i = 0; i < m_width; ++i) {
		int y = m_height - 1;
		m_basicMap[0][i].color = color::BLACK;						// top border
		m_basicMap[y][i].color = color::BLACK;						// bottom border
	}

	for (int i = 0; i < m_height; ++i) {
		int x = m_width - 1;
		m_basicMap[i][0].color = color::BLACK;						// left border
		m_basicMap[i][x].color = color::BLACK;						// right border
	}
}

void HorseMap::findLargestArea() {
	m_areas.reserve(10);											// reserve space for 10 areas, can be changed later
	int areaSize;
	for (auto& row : m_basicMap) {
		for (auto& pixel : row) {
			if (pixel.color == color::WHITE) {
				areaSize = exploreArea(pixel);
				if (areaSize > 0)									// if we found an area
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
			continue;												// out of bounds

		pixel = m_basicMap[y][x];

		if (countThisColor == color::BLACK && pixel.color == color::BLACK) {
			++count;

			m_basicMap[y][x].color = color::RED;					//mark as a bounding pixel
			continue;
		}

		if (pixel.color != color::WHITE) 
			continue;

		pixel.color = color::GRAY;									// mark as area pixel

		if (countThisColor == color::WHITE) 
			++count;
		

		m_basicMap[y][x].color = pixel.color;						// update the pixel in the map

		m_floodFillQueue.push({ x - 1, y });						// left
		m_floodFillQueue.push({ x + 1, y });						// right
		m_floodFillQueue.push({ x, y - 1 });						// up
		m_floodFillQueue.push({ x, y + 1 });						// down

	}

	return count;
}

void HorseMap::selectLargestArea() {

	stringToVector();
	int numberBoundaries = countBoundaries(m_largestAreaStart);
	std::cout << "Number of boundaries: " << numberBoundaries << std::endl;

}

int HorseMap::countBoundaries(point& start) {

	int count = exploreArea(start, color::BLACK); 
	return count;
}

void HorseMap::makeBoundaryMap() {
	std::ofstream stream("ImageProcessingOutput/errorLog.txt", std::ios::out);
	for (auto& row : m_basicMap) {
		for (auto& pixel : row) {
			if (pixel.color == color::RED) {
				m_boundaryMap[pixel.y][pixel.x] = \
				processNearbyBoundaries(pixel, stream);				//set as boundary
			}
			else {
				// TOOD: remove bool to save data if you really want to
				m_boundaryMap[pixel.y][pixel.x] = \
				{ pixel.x, pixel.y, 0.0f, false };					//set non-boundary
			}
			
		}
	}
}

boundary HorseMap::processNearbyBoundaries(point& start, std::ofstream& stream) {
	//TODO: shrink function size
	double vectorX, vectorY;
	
	std::vector<LOBFPoint> points;
	addNearbyBoundaries(start, points);								// scan for other boundaries nearby
	 	
	LOBFLine lobf = CalculateLineOfBestFit(points); 

	// calculate the normal vector
	if ((int)lobf.slope == 9999) {
		vectorX = 1;												// vertical line
		vectorY = 0;
	}
	else {
		vectorX = lobf.slope;
		vectorY = -1;
	}

	// normalize normal vector;
	double magV = std::sqrt(vectorX * vectorX + vectorY * vectorY);
	if (magV != 0) {
		vectorX /= magV;
		vectorY /= magV;
	}

	// check if vector lands inside the area
	int checkX = start.x + (int)std::round(vectorX);
	int checkY = start.y + (int)std::round(vectorY);

	// if out of bounds, flip vector
	if (checkX < 0 || checkX >= m_width || checkY < 0 || checkY >= m_height) {
		if (stream) {
			stream << "Normal vector out of bounds: <" \
				<< checkX << ", " << checkY << ">" << std::endl;
		}
		// flip vector and change check coords accordingly
		vectorX *= -1;
		vectorY *= -1;
		checkX = start.x + (int)std::round(vectorX);
		checkY = start.y + (int)std::round(vectorY);
	}

	// if color is white or black, flip vector 
	if (m_basicMap[checkY][checkX].color == color::WHITE || m_basicMap[checkY][checkX].color == color::BLACK) {
		vectorX *= -1;
		vectorY *= -1;
		checkX = start.x + (int)std::round(vectorX);
		checkY = start.y + (int)std::round(vectorY);
	}

	//again flip vectors if out of bounds
	if (checkX < 0 || checkX >= m_width || checkY < 0 || checkY >= m_height) {
		if (stream) {
			stream << "Normal vector out of bounds again: <" \
				<< checkX << ", " << checkY << ">" << std::endl;
		}
		vectorX *= -1;
		vectorY *= -1;
		checkX = start.x + (int)std::round(vectorX);
		checkY = start.y + (int)std::round(vectorY);
	}

	if (m_basicMap[checkY][checkX].color == color::WHITE || m_basicMap[checkY][checkX].color == color::BLACK) {
		
		m_basicMap[checkY][checkX].color = color::GREEN; 			//cant flip or else would be out of bounds

	}
	
	else if (m_basicMap[checkY][checkX].color == color::GREEN){}	// do nothing, already marked

	else if (m_basicMap[checkY][checkX].color == color::RED) {

		vectorX *= 2;
		vectorY *= 2;
		checkX = start.x + (int)std::round(vectorX);
		checkY = start.y + (int)std::round(vectorY);

		if (m_basicMap[checkY][checkX].color != color::RED) {
			m_basicMap[checkY][checkX].color = color::GREEN;
		}
	}

	else if (m_basicMap[checkY][checkX].color == color::GRAY) {		// if color is gray, vector is correct
		m_basicMap[checkY][checkX].color = color::GREEN;
	}
	else {
		if (stream) {
			stream << "tiny little bug at (" << checkX << ", " << checkY << "): "
				<< m_basicMap[checkY][checkX].color << std::endl;
		}
	}

	float normal = std::atan2(vectorY, vectorX);

	boundary processedBoundary = { start.x, start.y, normal, true };
	return processedBoundary;
}

std::vector<LOBFPoint> HorseMap::addNearbyBoundaries(point& start, std::vector<LOBFPoint>& points) {

	for (int x = -1; x < 2; ++x) {
		for (int y = -1; y < 2; ++y) {

			int newX = start.x + x;
			int newY = start.y + y;

			if (newX < 0 || newX >= m_width || \
				newY < 0 || newY >= m_height) {						// check if the new pixel is within bounds
				continue;
			}

			if (m_basicMap[newY][newX].color == color::RED) {
				points.push_back(LOBFPoint(newX, newY));
			}
		}
	}

	return points;
}

void HorseMap::calculateNormal(TNumber slope, point& start, std::ofstream& stream) const{

	double vectorX = slope;
	double vectorY = -1;

	// normalize normal vector;
	double magV = std::sqrt(vectorX * vectorX + vectorY * vectorY);
	if (magV != 0) {
		vectorX /= magV;
		vectorY /= magV;
	}

	// coords to check if vector lands inside the area
	int checkX = start.x + (int)std::round(vectorX);
	int checkY = start.y + (int)std::round(vectorY);

	if (checkX < 0 || checkX >= m_width || checkY < 0 || checkY >= m_height) {
		std::cout << "Normal vector out of bounds: <" \
			<< checkX << ", " << checkY << ">" << std::endl;

		vectorX *= -1;												// flip the vector if out of bounds
		vectorY *= -1;
		checkX = start.x + (int)std::round(vectorX);				// flip check coords to other side
		checkY = start.y + (int)std::round(vectorY);
	}
}

const char* HorseMap::vectorToString(const std::vector<std::vector<point>>& map) const
{
	return nullptr;
}

