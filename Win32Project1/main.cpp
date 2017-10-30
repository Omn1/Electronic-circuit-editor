#define _CRT_SECURE_NO_WARNINGS
#include <SFML/Graphics.hpp>
#include <vector>
#include <algorithm>
#include <iostream>
#include <Windows.h>
#include <cstdio>

float gridThickness = 5;
float gridOutlineThickness = 2;
float n = 12, m = 16;
float editorFieldSizeX = 800;
float editorFieldSizeY = 600;
float topMargin = 50;

bool mat[1000][1000][2];

void addLine(int x, int y, int l, int ishor) {
	if (ishor) {
		if (l < 0) {
			x = x + l;
			l = -l;
		}
		for (int i = x; i < x + l; i++) {
			mat[i][y][0] = 1;
		}
	}
	else {
		if (l < 0) {
			y = y + l;
			l = -l;
		}
		for (int i = y; i < y + l; i++) {
			mat[x][i][1] = 1;
		}
	}
}

void connectVertexes(float startX, float startY, float endX, float endY) {
	addLine(startX, startY, endY - startY, 0);
	addLine(startX, endY, endX - startX, 1);
}

int main()
{
	//AllocConsole();
	//freopen("CONOUT$", "w", stdout);
	
	sf::Texture texture;
	if (!texture.loadFromFile("toolbar-sprite.png")) {
		std::cout << "error opening texture\n";
		return 0;
	}
	texture.setSmooth(true);

	sf::Sprite clearButton;
	clearButton.setTexture(texture);
	clearButton.setTextureRect(sf::IntRect(64, 32, 32, 32));
	clearButton.setPosition(sf::Vector2f(10, 9));
	sf::RenderWindow window(sf::VideoMode(editorFieldSizeX, topMargin+editorFieldSizeY), "Electronic circuit editor");
	sf::RectangleShape vline(sf::Vector2f(editorFieldSizeX, gridThickness));
	sf::RectangleShape hline(sf::Vector2f(gridThickness, editorFieldSizeY));
	sf::RectangleShape background(sf::Vector2f(editorFieldSizeX, topMargin + editorFieldSizeY));
	sf::RectangleShape menuBarBG(sf::Vector2f(editorFieldSizeX, topMargin));

	menuBarBG.setFillColor(sf::Color::White);
	menuBarBG.setOutlineThickness(-5);
	menuBarBG.setOutlineColor(sf::Color::Red);
	background.setFillColor(sf::Color::White);
	vline.setFillColor(sf::Color(0,0,0,120));
	hline.setFillColor(sf::Color(0,0,0,120));

	float startX, startY;
	bool isStarted = 0;

	//std::vector< std::pair<sf::Vector2f, sf::Vector2f> > lines;

	while (window.isOpen())
	{
		sf::Event event;
		while (window.pollEvent(event))
		{
			if (event.type == sf::Event::Closed)
				window.close();
			else if (event.type == sf::Event::MouseWheelScrolled) {
				float delta = event.mouseWheelScroll.delta;
				n *= (100.f + delta) / 100.f;
				m *= (100.f + delta) / 100.f;
				//gridThickness *= (100.f - delta) / 100.f;
				//gridThickness = std::max(gridThickness, 1.f);
				//gridThickness = round(gridThickness);
				gridThickness = round(editorFieldSizeX / m * 0.1);
				gridThickness = std::max(gridThickness, 1.f);
				//gridOutlineThickness *= (100.f - delta) / 100.f;
				gridOutlineThickness = gridThickness * 0.4;
				gridOutlineThickness = std::max(gridOutlineThickness, 1.f);
				vline.setSize(sf::Vector2f(editorFieldSizeX, gridThickness));
				hline.setSize(sf::Vector2f(gridThickness, editorFieldSizeY));
			}
			else if (event.type == sf::Event::MouseButtonPressed) {
				if (event.mouseButton.button == sf::Mouse::Left) {
					isStarted = 1;
					startX = event.mouseButton.x;
					startX = round(startX * m / editorFieldSizeX);
					startY = event.mouseButton.y;
					startY -= topMargin;
					startY = round(startY * n / editorFieldSizeY);
				}
			}
			else if (event.type == sf::Event::MouseButtonReleased) {
				if (event.mouseButton.button == sf::Mouse::Left) {
					isStarted = 0;
					float endX = event.mouseButton.x;
					endX = round(endX * m / editorFieldSizeX);
					float endY = event.mouseButton.y;
					endY -= topMargin;
					endY = round(endY * n / editorFieldSizeY);
					//lines.push_back({sf::Vector2f(startX, startY), sf::Vector2f(0, endY - startY)});
					//lines.push_back({sf::Vector2f(startX, endY), sf::Vector2f(1, endX - startX)});
					connectVertexes(startX, startY, endX, endY);
					//std::cout << lines.size() << std::endl;
				}
			}
		}

		window.clear();
		window.draw(background);
		window.draw(menuBarBG);
		for (int i = 1; i < n; i++) {
			vline.setPosition(sf::Vector2f(0, topMargin + editorFieldSizeY / n*i));
			window.draw(vline);
		}
		for (int i = 1; i < m; i++) {
			hline.setPosition(sf::Vector2f(editorFieldSizeX / m*i, topMargin));
			window.draw(hline);
		}
		sf::RectangleShape temp;
		temp.setFillColor(sf::Color::Black);
		temp.setOutlineThickness(gridOutlineThickness);
		temp.setOutlineColor(sf::Color::Black);
		/*for (int i = 0; i < lines.size(); i++) {
			sf::Vector2f pos = lines[i].first;
			pos.x *= editorFieldSizeX / m;
			pos.y *= editorFieldSizeY / n;
			pos.y += topMargin;
			temp.setPosition(pos);
			sf::Vector2f tempSize;
			if (lines[i].second.x == 0) {
				tempSize.x = round(gridThickness);
				tempSize.y = lines[i].second.y*editorFieldSizeY / n;
			}
			else {
				tempSize.y = round(gridThickness);
				tempSize.x = lines[i].second.y*editorFieldSizeX / m;
			}
			temp.setSize(tempSize);
			window.draw(temp);
		}*/
		for (int i = 0; i < 1000; i++) {
			for (int j = 0; j < 1000; j++) {
				if (mat[i][j][0]) {
					temp.setSize(sf::Vector2f(editorFieldSizeX / m, round(gridThickness)));
					temp.setPosition(sf::Vector2f(i*editorFieldSizeX / m, topMargin + j*editorFieldSizeY / n));
					window.draw(temp);
				}
				if (mat[i][j][1]) {
					temp.setSize(sf::Vector2f(round(gridThickness), editorFieldSizeX / m));
					temp.setPosition(sf::Vector2f(i*editorFieldSizeX / m, topMargin + j*editorFieldSizeY / n));
					window.draw(temp);
				}
			}
		}
		if (isStarted) {
			float endX = sf::Mouse::getPosition(window).x;
			endX = round(endX * m / editorFieldSizeX);
			float endY = sf::Mouse::getPosition(window).y;
			endY -= topMargin;
			endY = round(endY * n / editorFieldSizeY);
			temp.setFillColor(sf::Color::Green);
			temp.setOutlineColor(sf::Color::Green);

			temp.setSize(sf::Vector2f((endX - startX) / m * editorFieldSizeX, gridThickness));
			temp.setPosition(sf::Vector2f(startX / m * editorFieldSizeX,endY / n * editorFieldSizeY + topMargin));
			window.draw(temp);

			temp.setSize(sf::Vector2f(gridThickness, (endY - startY) / n * editorFieldSizeY));
			temp.setPosition(sf::Vector2f(startX / m * editorFieldSizeX,startY / n * editorFieldSizeY + topMargin));
			window.draw(temp);
		}
		window.draw(clearButton);
		window.display();
	}

	return 0;
}