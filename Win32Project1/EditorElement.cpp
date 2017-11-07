#include "EditorElement.h"
#include "Editor.h"


void EditorElement::draw(sf::RenderWindow * window)
{
	float X = pos.x, Y = pos.y;
	if (isRotated % 4 == 1) {
		X += sizeY;
	}
	X *= editorFieldSizeX / m;
	X += leftMargin + gridThickness / 2;
	Y *= editorFieldSizeY / n;
	Y += topMargin + gridThickness / 2;
	sf::Sprite temp;
	temp.setTexture(*texture);
	temp.setPosition(X, Y);
	temp.setRotation(90 * isRotated);
	temp.setScale(sf::Vector2f(cellSize / standartCellSize, cellSize / standartCellSize));
	window->draw(temp);
}

EditorElement::EditorElement()
	: pos({ 0,0 })
	, isRotated(0)
	, resistance(0)
{
}

EditorElement::EditorElement(coord posT, int isRotatedT)
	: pos(posT)
	, isRotated(isRotatedT)
	, resistance(0)
{
}


EditorElement::~EditorElement()
{
}


Resistor::Resistor()
	: EditorElement()
{
	sizeX = resistorSizeX;
	sizeY = resistorSizeY;
	texture = resistorTexture;
}

Resistor::Resistor(coord posT, int isRotatedT)
	: EditorElement(posT, isRotatedT)
{
	sizeX = resistorSizeX;
	sizeY = resistorSizeY;
	texture = resistorTexture;
}


Battery::Battery()
	: EditorElement()
{
	sizeX = batterySizeX;
	sizeY = batterySizeY;
	texture = batteryTexture;
}

Battery::Battery(coord posT, int isRotatedT)
	: EditorElement(posT, isRotatedT)
{
	sizeX = batterySizeX;
	sizeY = batterySizeY;
	texture = batteryTexture;
}

Lamp::Lamp()
	: EditorElement()
{
	sizeX = lampSizeX;
	sizeY = lampSizeY;
	texture = lampTexture;
}

Lamp::Lamp(coord posT, int isRotatedT)
	: EditorElement(posT, isRotatedT)
{
	sizeX = lampSizeX;
	sizeY = lampSizeY;
	texture = lampTexture;
}