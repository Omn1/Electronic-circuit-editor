#include "InspectorInput.h"
#include <string>
#include <SFML/Graphics.hpp>
#include <vector>
#include <algorithm>

InspectorInput::InspectorInput()
	:isNumberOnly(1)
	,sizeX1(150)
	,sizeX2(150)
	,mxlen(8)
{
}

InspectorInput::InspectorInput(bool tisNumberOnly, int tsizeX1, int tsizeX2, int tmxlen)
	: isNumberOnly(tisNumberOnly)
	, sizeX1(tsizeX1)
	, sizeX2(tsizeX2)
	, mxlen(tmxlen)
{
}

InspectorInput::~InspectorInput()
{
}

void InspectorInput::draw()
{
	sf::RenderWindow * window = new sf::RenderWindow(sf::VideoMode(sizeX1 + sizeX2 + separatorThickness, fields.size() * 50 + 45), "Input:", sf::Style::Titlebar | sf::Style::Close);
	sf::RectangleShape BG(sf::Vector2f(sizeX1 + sizeX2 + separatorThickness, fields.size() * 50 + 45));
	BG.setFillColor(sf::Color::White);
	BG.setOutlineThickness(separatorThickness);
	BG.setOutlineColor(mainColor);
	int curField = 0;
	sf::Text text("", arial, 24);
	text.setFillColor(sf::Color::Black);
	sf::RectangleShape selectedFieldBG(sf::Vector2f(sizeX2 + 2 * separatorThickness, 40));
	selectedFieldBG.setFillColor(sf::Color::White);
	selectedFieldBG.setOutlineThickness(separatorThickness);
	selectedFieldBG.setOutlineColor(sf::Color(120, 120, 120));
	selectedFieldBG.setPosition(sizeX1, 5);
	while (window->isOpen())
	{
		sf::Event event;
		while (window->pollEvent(event))
		{
			if (event.type == sf::Event::Closed) {
				window->close();
				return;
			}
			else if (event.type == sf::Event::MouseButtonPressed) {
				if (event.mouseButton.y >= 50 * fields.size() - 5) {
					window->close();
					return;
				}
				else if (event.mouseButton.x >= sizeX1 + separatorThickness && event.mouseButton.y < 50 * fields.size() - 5) {
					curField = floor(event.mouseButton.y / 45);
					selectedFieldBG.setPosition(sizeX1, 5 + curField * 45);
				}
			}
			else if (event.type == sf::Event::TextEntered) {
				if (event.text.unicode == '\b') {
					if (fields[curField].size() > 0) {
						fields[curField].pop_back();
					}
				}
				else if ((!isNumberOnly) || isdigit(event.text.unicode) || (event.text.unicode=='.' && fields[curField].find('.')==-1)) {
					if(fields[curField].size()<mxlen)
						fields[curField] += static_cast<char>(event.text.unicode);
					else
						fields[curField][mxlen-1] = static_cast<char>(event.text.unicode);
				}
			}
		}

		window->clear();
		window->draw(BG);
		text.setString("OK");
		text.setPosition((sizeX1 + sizeX2) / 2 - 20, 50 * fields.size() + 5);
		window->draw(text);
		sf::RectangleShape temp(sf::Vector2f(150, 50));
		temp.setFillColor(sf::Color::White);
		temp.setOutlineColor(mainColor);
		temp.setOutlineThickness(separatorThickness);
		for (int i = 0; i < fields.size(); i++) {
			temp.setSize(sf::Vector2f(sizeX1, 50));
			temp.setPosition(0, i * 45);
			window->draw(temp);
			temp.setSize(sf::Vector2f(sizeX2, 50));
			temp.setPosition(145, i * 45);
			window->draw(temp);
		}
		window->draw(selectedFieldBG);
		for (int i = 0; i < fields.size(); i++) {
			text.setPosition(10, i * 45 + 10);
			text.setString(fieldNames[i]);
			window->draw(text);
			text.setPosition(155, i * 45 + 10);
			text.setString(fields[i]);
			window->draw(text);
		}
		window->display();
	}
}
