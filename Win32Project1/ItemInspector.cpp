#include "ItemInspector.h"



ItemInspector::ItemInspector()
{
}


ItemInspector::~ItemInspector()
{
}

void ItemInspector::draw(sf::RenderWindow * window)
{
	sf::RectangleShape inspectorBG(sf::Vector2f(inspectorWidth, topMargin + editorFieldSizeY));
	inspectorBG.setFillColor(sf::Color::White);
	inspectorBG.setOutlineColor(mainColor);
	inspectorBG.setOutlineThickness(separatorThickness);
	inspectorBG.setPosition(sf::Vector2f(leftMargin + editorFieldSizeX, 0));
	window->draw(inspectorBG);
	sf::RectangleShape nameTextBG(sf::Vector2f(inspectorWidth, inspectorLineSize));
	nameTextBG.setFillColor(sf::Color::White);
	nameTextBG.setOutlineColor(mainColor);
	nameTextBG.setOutlineThickness(separatorThickness);
	nameTextBG.setPosition(leftMargin + editorFieldSizeX, 0);
	window->draw(nameTextBG);
	sf::Text inspectorNameText("Properties:", arial);
	inspectorNameText.setPosition(sf::Vector2f(leftMargin + editorFieldSizeX - separatorThickness + inspectorLeftTextMargin, -separatorThickness));
	inspectorNameText.setFillColor(sf::Color::Black);
	window->draw(inspectorNameText);
	for (int i = 0; i < sections.size(); i++) {
		InspectorSection temp(sections[i].first, sections[i].second, sf::Vector2f(leftMargin + editorFieldSizeX, (2*i+1)*(inspectorLineSize + separatorThickness)), sectionSize);
		temp.draw(window);
	}
}
