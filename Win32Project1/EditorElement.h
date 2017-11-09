#pragma once
#include "Editor.h"
#include <SFML/Graphics.hpp>
#include "ChainVertex.h"
#include <string>
class EditorElement
{
public:
	coord pos;
	int isRotated;
	double resistance;
    float sizeX, sizeY;
	float deltaX, deltaY;
	double getVoltage();
	ChainVertex * v1, * v2;
	ElementRect getElementRect();
	void draw(sf::RenderWindow * window);
	void rotate();
	void move(float dX, float dY);
	void updateRotation();
	std::vector<std::pair<std::string, std::string> > getInspectorElements();
	sf::Texture *texture;
	EditorElement();
	EditorElement(coord pos, int isRotated);
	~EditorElement();
};

class Resistor : public EditorElement
{
public:
	//void draw(sf::RenderWindow * window);
	Resistor();
	Resistor(coord posT, int isRotatedT);
	std::vector<std::pair<std::string, std::string>> getInspectorElements();
};

class Battery : public EditorElement
{
public:
	//void draw(sf::RenderWindow * window);
	Battery();
	Battery(coord posT, int isRotatedT);
	std::vector<std::pair<std::string, std::string>> getInspectorElements();
};

class Lamp : public EditorElement
{
public:
	//void draw(sf::RenderWindow * window);
	Lamp();
	Lamp(coord posT, int isRotatedT);
	std::vector<std::pair<std::string, std::string>> getInspectorElements();
};