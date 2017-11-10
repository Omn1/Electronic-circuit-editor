#include "InspectorInput.h"
#include <string>
#include "Editor.h"
#include <SFML/Graphics.hpp>
#include <vector>
#include <algorithm>

InspectorInput::InspectorInput()
{
}

InspectorInput::~InspectorInput()
{
}

void InspectorInput::draw()
{
	sf::RenderWindow * window = new sf::RenderWindow(sf::VideoMode(295, fields.size()*50+45), "Input:");
	sf::RectangleShape BG(sf::Vector2f(295, fields.size() * 50 + 45));
	BG.setFillColor(sf::Color::White);
	BG.setOutlineThickness(separatorThickness);
	BG.setOutlineColor(mainColor);
	int curField = 0;
	sf::Text text("", arial, 24);
	text.setFillColor(sf::Color::Black);
	sf::RectangleShape selectedFieldBG(sf::Vector2f(140, 40));
	selectedFieldBG.setFillColor(sf::Color::White);
	selectedFieldBG.setOutlineThickness(separatorThickness);
	selectedFieldBG.setOutlineColor(sf::Color(120, 120, 120));
	selectedFieldBG.setPosition(150, 5);
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
				else if (event.mouseButton.x >= 145.f && event.mouseButton.y < 50 * fields.size() - 5) {
					curField = floor(event.mouseButton.y / 45);
					selectedFieldBG.setPosition(150, 5 + curField * 45);
				}
			}
			else if (event.type == sf::Event::TextEntered) {
				if (event.text.unicode == '\b') {
					if (fields[curField].size() > 0) {
						fields[curField].pop_back();
					}
				}
				else if (isdigit(event.text.unicode) || (event.text.unicode=='.' && fields[curField].find('.')==-1)) {
					if(fields[curField].size()<8)
						fields[curField] += static_cast<char>(event.text.unicode);
					else
						fields[curField][7] = static_cast<char>(event.text.unicode);
				}
			}
		}

		window->clear();
		window->draw(BG);
		text.setString("OK");
		text.setPosition(130, 50 * fields.size() + 5);
		window->draw(text);
		sf::RectangleShape temp(sf::Vector2f(150, 50));
		temp.setFillColor(sf::Color::White);
		temp.setOutlineColor(mainColor);
		temp.setOutlineThickness(separatorThickness);
		for (int i = 0; i < fields.size(); i++) {
			temp.setPosition(0, i * 45);
			window->draw(temp);
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
