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
	ChainVertex(ChainVertex * vert);
	void operator = (const ChainVertex &vert);
	void operator = (ChainVertex * vert);
	void draw(sf::RenderWindow * window);
	~ChainVertex();
};

