#pragma once
#include "Editor.h"
#include <SFML/Graphics.hpp>
class EditorElement
{
public:
	coord pos;
	int isRotated;
	double resistance;
    float sizeX, sizeY;
	void draw(sf::RenderWindow * window);
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
};

class Battery : public EditorElement
{
public:
	//void draw(sf::RenderWindow * window);
	Battery();
	Battery(coord posT, int isRotatedT);
};

class Lamp : public EditorElement
{
public:
	//void draw(sf::RenderWindow * window);
	Lamp();
	Lamp(coord posT, int isRotatedT);
};