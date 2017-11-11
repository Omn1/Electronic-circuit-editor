#include "ChainVertex.h"
#include "Editor.h"


ChainVertex::ChainVertex()
	:potential(0)
	, pos({ 0,0 })
{
}

ChainVertex::ChainVertex(coord posT)
	: potential(0)
	, pos(posT)
{

}

ChainVertex::ChainVertex(const ChainVertex & other)
{
	pos.x = other.pos.x;
	pos.y = other.pos.y;
}

void ChainVertex::draw(sf::RenderWindow * window)
{
	float X = pos.x, Y = pos.y;
	X *= editorFieldSizeX / m;
	X += leftMargin - vertexRadius + gridThickness * 0.5;
	Y *= editorFieldSizeY / n;
	Y += topMargin - vertexRadius + gridThickness * 0.5;
	sf::CircleShape temp(vertexRadius);
	temp.setPosition(sf::Vector2f(X, Y));
	temp.setFillColor(sf::Color::Black);
	window->draw(temp);
}

ChainVertex::~ChainVertex()
{
}
