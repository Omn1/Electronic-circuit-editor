#pragma once
#include <SFML/Graphics.hpp>
class InspectorSection
{
public:
	std::string name;
	std::string value;
	sf::Vector2f pos;
	sf::Vector2f size;
	float outlineThickness;
	float lineSize;
	sf::Color color;
	float leftTextMargin;
	sf::Font * font;
	InspectorSection();
	InspectorSection(std::string name, std::string value, sf::Vector2f pos, sf::Vector2f size, float outlineThickness, float lineSize, sf::Color color, float leftTextMargin, sf::Font * font);
	~InspectorSection();
	void draw(sf::RenderWindow * window);
};

