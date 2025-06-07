#pragma once

struct line {
	int x, y;
	float slope; 
};

struct boundary {
	int x, y;
	float normal; //normal is angle in radians
};

struct map {
	boundary boundaries[];
};

map CreateMap(char* filepath);