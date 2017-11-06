#include "InspectorSection.h"



InspectorSection::InspectorSection()
{
}

InspectorSection::InspectorSection(std::string name, std::string value, sf::Vector2f pos, sf::Vector2f size, float outlineThickness, float lineSize, sf::Color color, float leftTextMargin, sf::Font * font)
	:name(name)
	, value(value)
	, pos(pos)
	, size(size)
	, outlineThickness(outlineThickness)
	, lineSize(lineSize)
	, color(color)
	, leftTextMargin(leftTextMargin)
	, font(font)
{

}


InspectorSection::~InspectorSection()
{
}

void InspectorSection::draw(sf::RenderWindow * window)
{
	sf::RectangleShape BG(size);
	BG.setFillColor(sf::Color::White);
	BG.setOutlineColor(color);
	BG.setOutlineThickness(outlineThickness);
	BG.setPosition(pos);
	window->draw(BG);
	sf::Text nameText;
	nameText.setString(name);
	nameText.setFont(*font);
	nameText.setFillColor(sf::Color::Black);
	nameText.setPosition(sf::Vector2f(pos.x - outlineThickness + leftTextMargin, pos.y - outlineThickness));
	window->draw(nameText);
	sf::Text valueText;
	valueText.setFont(*font);
	valueText.setString(value);
	valueText.setFillColor(sf::Color::Black);
	valueText.setPosition(sf::Vector2f(pos.x - outlineThickness + leftTextMargin, pos.y + lineSize));
	window->draw(valueText);
}
