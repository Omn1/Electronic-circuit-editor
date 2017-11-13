#pragma once
#include "detectpoint.h"

class detectLine
{
public:
	detectLine();
	~detectLine();
	int x1;
	int x2;
	int y1;
	int y2;
	detectLine(detectPoint a, detectPoint b);
	detectPoint detectBuildByAzimut(detectPoint a, float angle, float length);
	detectLine(int x1, int y1, int x2, int y2);
	detectLine(detectPoint a, float angle, float length);
	float length();

};


