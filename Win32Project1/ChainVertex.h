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
	ChainVertex(const ChainVertex &other);
	void draw(sf::RenderWindow * window);
	~ChainVertex();
};

