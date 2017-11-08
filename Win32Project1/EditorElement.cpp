#include "EditorElement.h"
#include "Editor.h"


ElementRect EditorElement::getElementRect()
{
	if (isRotated % 2 == 0) {
		return{ pos.x, pos.x + sizeX, pos.y, pos.y + sizeY};
	}
	else {
		return{ pos.x, pos.x + sizeY, pos.y, pos.y + sizeX};
	}
}

void EditorElement::draw(sf::RenderWindow * window)
{
	float X = pos.x, Y = pos.y;
	X += deltaX;
	Y += deltaY;
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

void EditorElement::rotate()
{
	isRotated++;
	isRotated %= 4;
	updateRotation();
}

void EditorElement::move(float dX, float dY)
{
	pos.x += dX;
	pos.y += dY;
	if (v1) {
		v1->pos.x += dX;
		v1->pos.y += dY;
	}
	if (v2) {
		v2->pos.x += dX;
		v2->pos.y += dY;
	}
}

void EditorElement::updateRotation()
{
	if (isRotated == 0) {
		deltaX = 0;
		deltaY = 0;
		if (v1) {
			v1->pos.x = pos.x;
			v1->pos.y = pos.y + sizeY / 2;
		}
		if (v2) {
			v2->pos.x = pos.x + sizeX;
			v2->pos.y = pos.y + sizeY / 2;
		}
	}
	if (isRotated == 1) {
		deltaX = sizeY;
		deltaY = 0;
		if (v1) {
			v1->pos.x = pos.x + sizeY / 2;
			v1->pos.y = pos.y;
		}
		if (v2) {
			v2->pos.x = pos.x + sizeY / 2;
			v2->pos.y = pos.y + sizeX;
		}
	}
	if (isRotated == 2) {
		deltaX = sizeX;
		deltaY = sizeY;
		if (v1) {
			v1->pos.x = pos.x + sizeX;
			v1->pos.y = pos.y + sizeY / 2;
		}
		if (v2) {
			v2->pos.x = pos.x;
			v2->pos.y = pos.y + sizeY / 2;
		}
	}
	if (isRotated == 3) {
		deltaX = 0;
		deltaY = sizeX;
		if (v1) {
			v1->pos.x = pos.x + sizeY / 2;
			v1->pos.y = pos.y + sizeX;
		}
		if (v2) {
			v2->pos.x = pos.x + sizeY / 2;
			v2->pos.y = pos.y;
		}
	}
}

EditorElement::EditorElement()
	: pos({ 0,0 })
	, isRotated(0)
	, resistance(0)
	, deltaX(0)
	, deltaY(0)
	, v1(0)
	, v2(0)
{
	updateRotation();
}

EditorElement::EditorElement(coord posT, int isRotatedT)
	: pos(posT)
	, isRotated(isRotatedT)
	, resistance(0)
	, deltaX(0)
	, deltaY(0)
	, v1(0)
	, v2(0)
{
	updateRotation();
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
	updateRotation();
}

Resistor::Resistor(coord posT, int isRotatedT)
	: EditorElement(posT, isRotatedT)
{
	sizeX = resistorSizeX;
	sizeY = resistorSizeY;
	texture = resistorTexture;
	updateRotation();
}


Battery::Battery()
	: EditorElement()
{
	sizeX = batterySizeX;
	sizeY = batterySizeY;
	texture = batteryTexture;
	updateRotation();
}

Battery::Battery(coord posT, int isRotatedT)
	: EditorElement(posT, isRotatedT)
{
	sizeX = batterySizeX;
	sizeY = batterySizeY;
	texture = batteryTexture;
	updateRotation();
}

Lamp::Lamp()
	: EditorElement()
{
	sizeX = lampSizeX;
	sizeY = lampSizeY;
	texture = lampTexture;
	updateRotation();
}

Lamp::Lamp(coord posT, int isRotatedT)
	: EditorElement(posT, isRotatedT)
{
	sizeX = lampSizeX;
	sizeY = lampSizeY;
	texture = lampTexture;
	updateRotation();
}