#pragma once
class detectElement
{
public:
	detectElement();
	detectElement(int x, int y, int type, int rotation);
	~detectElement();
	int x;
	int y;
	int type; //0 - wire 1 - resistor 2 - lamp 3 - battery 4 - switch
	int rotation;
};

