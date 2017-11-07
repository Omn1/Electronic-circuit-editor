#pragma once
#include <SFML/Graphics.hpp>
#include "Editor.h"
class InspectorSection
{
public:
	std::string name;
	std::string value;
	sf::Vector2f pos;
	sf::Vector2f size;
	InspectorSection();
	InspectorSection(std::string name, std::string value, sf::Vector2f pos, sf::Vector2f size);
	~InspectorSection();
	void draw(sf::RenderWindow * window);
};

