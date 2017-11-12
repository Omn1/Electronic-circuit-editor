#include "EditorElement.h"
#include "Editor.h"
#include <string>

double EditorElement::getVoltage()
{
	if (v1 && v2) {
		return abs(v1->potential - v2->potential);
	}
	return 0.0;
}

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
	updateRotation();
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

std::vector<std::pair<std::string, std::string>> EditorElement::getInspectorElements()
{
	return std::vector<std::pair<std::string, std::string>>();
}

EditorElement::EditorElement()
	: pos({ 0,0 })
	, isRotated(0)
	, resistance(1)
	, realVoltage(0)
	, deltaX(0)
	, deltaY(0)
	, v1(0)
	, v2(0)
	, sizeX(1)
	, sizeY(0)
{
	updateRotation();
}

EditorElement::EditorElement(coord posT, int isRotatedT)
	: pos(posT)
	, isRotated(isRotatedT)
	, resistance(1)
	, realVoltage(0)
	, deltaX(0)
	, deltaY(0)
	, v1(0)
	, v2(0)
	, sizeX(1)
	, sizeY(0)
{
	updateRotation();
}

EditorElement::EditorElement(EditorElement * elem)
{
	pos.x = elem->pos.x;
	pos.y = elem->pos.y;
	isRotated = elem->isRotated;
	resistance = elem->resistance;
	realVoltage = elem->realVoltage;
	sizeX = elem->sizeX;
	sizeY = elem->sizeY;
	deltaX = elem->deltaX;
	deltaY = elem->deltaY;
	v1 = elem->v1;
	v2 = elem->v2;
	texture = elem->texture;
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

Resistor::Resistor(Resistor * elem)
{
	pos.x = elem->pos.x;
	pos.y = elem->pos.y;
	isRotated = elem->isRotated;
	resistance = elem->resistance;
	realVoltage = elem->realVoltage;
	sizeX = elem->sizeX;
	sizeY = elem->sizeY;
	deltaX = elem->deltaX;
	deltaY = elem->deltaY;
	v1 = elem->v1;
	v2 = elem->v2;
	texture = elem->texture;
}

std::vector<std::pair<std::string,std::string>> Resistor::getInspectorElements() 
{
	std::vector<std::pair<std::string, std::string>> temp;
	temp.push_back(make_pair(std::string("Item type:"), std::string("Resistor")));
	temp.push_back(make_pair("Voltage:", std::to_string(getVoltage()).substr(0, 10)));
	temp.push_back(make_pair("Current:", std::to_string(getVoltage() / resistance).substr(0, 10)));
	temp.push_back(make_pair("Resistance:", std::to_string(resistance).substr(0, 10)));
	return temp;
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

Battery::Battery(Battery * elem)
{
	pos.x = elem->pos.x;
	pos.y = elem->pos.y;
	isRotated = elem->isRotated;
	resistance = elem->resistance;
	realVoltage = elem->realVoltage;
	sizeX = elem->sizeX;
	sizeY = elem->sizeY;
	deltaX = elem->deltaX;
	deltaY = elem->deltaY;
	v1 = elem->v1;
	v2 = elem->v2;
	texture = elem->texture;
}

std::vector<std::pair<std::string, std::string>> Battery::getInspectorElements()
{
	std::vector<std::pair<std::string, std::string>> temp;
	temp.push_back(make_pair(std::string("Item type:"), std::string("Battery")));
	temp.push_back(make_pair("Voltage:", std::to_string(realVoltage).substr(0, 10)));
	temp.push_back(make_pair("Current:", std::to_string(getVoltage() / resistance).substr(0, 10)));
	temp.push_back(make_pair("Resistance:", std::to_string(resistance).substr(0, 10)));
	return temp;
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

Lamp::Lamp(Lamp * elem)
{
	pos.x = elem->pos.x;
	pos.y = elem->pos.y;
	isRotated = elem->isRotated;
	resistance = elem->resistance;
	realVoltage = elem->realVoltage;
	sizeX = elem->sizeX;
	sizeY = elem->sizeY;
	deltaX = elem->deltaX;
	deltaY = elem->deltaY;
	v1 = elem->v1;
	v2 = elem->v2;
	texture = elem->texture;
}

std::vector<std::pair<std::string, std::string>> Lamp::getInspectorElements()
{
	std::vector<std::pair<std::string, std::string>> temp;
	temp.push_back(make_pair(std::string("Item type:"), std::string("Lamp")));
	temp.push_back(make_pair("Voltage:", std::to_string(getVoltage()).substr(0,10)));
	temp.push_back(make_pair("Current:", std::to_string(getVoltage() / resistance).substr(0, 10)));
	temp.push_back(make_pair("Resistance:", std::to_string(resistance).substr(0, 10)));
	return temp;
}