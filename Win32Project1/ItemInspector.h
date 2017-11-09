#pragma once
#include "Editor.h"
#include "InspectorSection.h"
#include "SFML/Graphics.hpp"
#include <string>

class ItemInspector
{
public:
	ItemInspector();
	~ItemInspector();
	void draw(sf::RenderWindow * window);
	std::vector<std::pair<std::string, std::string> > sections;
};

