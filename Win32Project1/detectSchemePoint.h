#pragma once
class detectSchemePoint
{
public:
	detectSchemePoint();
	int x;
	int y;
	bool isNotPartOfElement;
	int neighbours[48];
	double val[48];
	int power[48];
	detectSchemePoint(int x, int y);
	void clear();
	~detectSchemePoint();
};

