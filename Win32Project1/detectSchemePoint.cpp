#include "detectSchemePoint.h"


detectSchemePoint::detectSchemePoint()
{
	for (int i = 0; i < 48; i++) {
		neighbours[i] = -1;
		val[i] = 0;
		power[i] = 0;
	}
}

detectSchemePoint::detectSchemePoint(int x, int y) {
	this->x = x;
	this->y = y;
	this->isNotPartOfElement = true;
	for (int i = 0; i < 48; i++) {
		neighbours[i] = -1;
		val[i] = 0;
		power[i] = 0;
	}

}

void detectSchemePoint::clear() {
	for (int i = 0; i < 48; i++) {
		neighbours[i] = -1;
		val[i] = 0;
		power[i] = 0;
	}
}


detectSchemePoint::~detectSchemePoint()
{
}