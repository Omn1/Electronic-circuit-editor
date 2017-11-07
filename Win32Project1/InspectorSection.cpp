#include "InspectorSection.h"
#include "Editor.h"


InspectorSection::InspectorSection()
{
}

InspectorSection::InspectorSection(std::string name, std::string value, sf::Vector2f pos, sf::Vector2f size)
	:name(name)
	, value(value)
	, pos(pos)
	, size(size)
{

}


InspectorSection::~InspectorSection()
{
}

void InspectorSection::draw(sf::RenderWindow * window)
{
	sf::RectangleShape BG(size);
	BG.setFillColor(sf::Color::White);
	BG.setOutlineColor(mainColor);
	BG.setOutlineThickness(separatorThickness);
	BG.setPosition(pos);
	window->draw(BG);
	sf::Text nameText;
	nameText.setString(name);
	nameText.setFont(arial);
	nameText.setFillColor(sf::Color::Black);
	nameText.setPosition(sf::Vector2f(pos.x - separatorThickness + inspectorLeftTextMargin, pos.y - separatorThickness));
	window->draw(nameText);
	sf::Text valueText;
	valueText.setFont(arial);
	valueText.setString(value);
	valueText.setFillColor(sf::Color::Black);
	valueText.setPosition(sf::Vector2f(pos.x - separatorThickness + inspectorLeftTextMargin, pos.y + inspectorLineSize));
	window->draw(valueText);
}
