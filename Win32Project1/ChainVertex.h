#pragma once
#include <SFML/Graphics.hpp>
#include "Editor.h"
class ChainVertex
{
public:
	coord pos;
	double potential;
	ChainVertex();
	ChainVertex(coord posT);
	void draw(sf::RenderWindow * window);
	~ChainVertex();
};

