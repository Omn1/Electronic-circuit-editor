#include "detectLine.h"


detectLine::detectLine()
{
}

detectLine::detectLine(int x1, int y1, int x2, int y2) {
	this->x1 = x1;
	this->y1 = y1;
	this->x2 = x2;
	this->y2 = y2;
}

detectPoint detectLine::detectBuildByAzimut(detectPoint a, float angle, float length) {
	float newX = a.x + length * cos(angle);
	float newY = a.y - length * sin(angle);
	return detectPoint(round(newX), round(newY));
}

detectLine::detectLine(detectPoint a, detectPoint b) {
	x1 = a.x;
	y1 = a.y;
	x2 = b.x;
	y2 = b.y;
}
detectLine::detectLine(detectPoint a, float angle, float length) {
	x2 = a.x;
	y2 = a.y;
	detectPoint b = detectBuildByAzimut(a, angle, length);
	x1 = b.x;
	y1 = b.y;
}

float detectLine::length() {
	return sqrt(float(x1 - x2) * float(x1 - x2) + float(y1 - y2) * float(y1 - y2));
}


detectLine::~detectLine()
{
}

