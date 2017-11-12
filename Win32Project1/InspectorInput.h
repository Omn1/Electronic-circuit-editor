#pragma once
#include <string>
#include <SFML/Graphics.hpp>
class InspectorInput
{
public:
	std::string name;
	std::string value;
	sf::Vector2f pos;
	sf::Vector2f size;
	std::vector<std::string> fieldNames;
	std::vector<std::string> fields;
	bool isNumberOnly;
	int sizeX1, sizeX2;
	int mxlen;
	InspectorInput();
	InspectorInput(bool tisNumberOnly, int tsizeX1, int tsizeX2, int tmxlen);
	~InspectorInput();
	void draw();
};

