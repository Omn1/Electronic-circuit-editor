#include "EditorElement.h"
#include "Editor.h"


void EditorElement::draw(sf::RenderWindow * window)
{
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

void Resistor::draw(sf::RenderWindow * window)
{
	float X = pos.x, Y = pos.y;
	X *= editorFieldSizeX / m;
	X += leftMargin;
	Y *= editorFieldSizeY / n;
	Y += topMargin;
	sf::RectangleShape temp;
	temp.setFillColor(sf::Color::Black);
	temp.setOutlineColor(sf::Color::Black);
	temp.setOutlineThickness(gridOutlineThickness);
	if (!this->isRotated) {
		temp.setSize(sf::Vector2f(editorFieldSizeX / m, gridThickness));
		temp.setPosition(sf::Vector2f(X, Y + editorFieldSizeY / n));
		window->draw(temp);
		temp.setPosition(sf::Vector2f(X + 4 * editorFieldSizeX / m, Y + editorFieldSizeY / n));
		window->draw(temp);
		temp.setSize(sf::Vector2f(3 * editorFieldSizeX / m, gridThickness));
		temp.setPosition(sf::Vector2f(X + editorFieldSizeX / m, Y + 0.5*editorFieldSizeY / n));
		window->draw(temp);
		temp.setPosition(sf::Vector2f(X + editorFieldSizeX / m, Y + 1.5*editorFieldSizeY / n));
		window->draw(temp);
		temp.setSize(sf::Vector2f(gridThickness, editorFieldSizeY / n));
		temp.setPosition(sf::Vector2f(X + editorFieldSizeX / m, Y + 0.5*editorFieldSizeY / n));
		window->draw(temp);
		temp.setPosition(sf::Vector2f(X + 4 * editorFieldSizeX / m, Y + 0.5*editorFieldSizeY / n));
		window->draw(temp);
	}
	else {
		temp.setSize(sf::Vector2f(gridThickness, editorFieldSizeY / n));
		temp.setPosition(sf::Vector2f(X + editorFieldSizeX / m, Y));
		window->draw(temp);
		temp.setPosition(sf::Vector2f(X + editorFieldSizeX / m, Y + 4 * editorFieldSizeY / n));
		window->draw(temp);
		temp.setSize(sf::Vector2f(gridThickness, 3 * editorFieldSizeY / n));
		temp.setPosition(sf::Vector2f(X + 0.5 * editorFieldSizeX / m, Y + editorFieldSizeY / n));
		window->draw(temp);
		temp.setPosition(sf::Vector2f(X + 1.5 * editorFieldSizeX / m, Y + editorFieldSizeY / n));
		window->draw(temp);
		temp.setSize(sf::Vector2f(editorFieldSizeX / m, gridThickness));
		temp.setPosition(sf::Vector2f(X + 0.5 * editorFieldSizeX / m, Y + editorFieldSizeY / n));
		window->draw(temp);
		temp.setPosition(sf::Vector2f(X + 0.5 * editorFieldSizeX / m, Y + 4 * editorFieldSizeY / n));
		window->draw(temp);
	}
}

Resistor::Resistor()
	:EditorElement()
{
}

Resistor::Resistor(coord posT, int isRotatedT)
	: EditorElement(posT, isRotatedT)
{
}

void Battery::draw(sf::RenderWindow * window) {
	float X = pos.x, Y = pos.y;
	X *= editorFieldSizeX / m;
	X += leftMargin;
	Y *= editorFieldSizeY / n;
	Y += topMargin;
	sf::RectangleShape temp;
	temp.setFillColor(sf::Color::Black);
	temp.setOutlineColor(sf::Color::Black);
	temp.setOutlineThickness(gridOutlineThickness);
	if (!this->isRotated) {
		temp.setSize(sf::Vector2f(2 * editorFieldSizeX / m, gridThickness));
		temp.setPosition(sf::Vector2f(X, Y + 2 * editorFieldSizeY / n));
		window->draw(temp);
		temp.setPosition(sf::Vector2f(X + 3 * editorFieldSizeX / m, Y + 2 * editorFieldSizeY / n));
		window->draw(temp);
		temp.setSize(sf::Vector2f(gridThickness, 3 * editorFieldSizeY / n));
		temp.setPosition(sf::Vector2f(X + 2 * editorFieldSizeX / m, Y + 0.5 * editorFieldSizeY / n));
		window->draw(temp);
		temp.setSize(sf::Vector2f(gridThickness, editorFieldSizeY / n));
		temp.setPosition(sf::Vector2f(X + 3 * editorFieldSizeX / m, Y + 1.5 * editorFieldSizeY / n));
		window->draw(temp);
	}
	else {
		temp.setSize(sf::Vector2f(gridThickness, 2 * editorFieldSizeY / n));
		temp.setPosition(sf::Vector2f(X + 2 * editorFieldSizeX / m, Y));
		window->draw(temp);
		temp.setPosition(sf::Vector2f(X + 2 * editorFieldSizeX / m, Y + 3 * editorFieldSizeY / n));
		window->draw(temp);
		temp.setSize(sf::Vector2f(3 * editorFieldSizeX / m, gridThickness));
		temp.setPosition(sf::Vector2f(X + 0.5 * editorFieldSizeX / m, Y + 2 * editorFieldSizeY / n));
		window->draw(temp);
		temp.setSize(sf::Vector2f(editorFieldSizeX / m, gridThickness));
		temp.setPosition(sf::Vector2f(X + 1.5 * editorFieldSizeX / m, Y + 3 * editorFieldSizeY / n));
		window->draw(temp);
	}
}

Battery::Battery()
	: EditorElement()
{
}

Battery::Battery(coord posT, int isRotatedT)
	: EditorElement(posT, isRotatedT)
{
}

void Lamp::draw(sf::RenderWindow * window) {
	float X = pos.x, Y = pos.y;
	X *= editorFieldSizeX / m;
	X += leftMargin;
	Y *= editorFieldSizeY / n;
	Y += topMargin;
	sf::RectangleShape temp;
	temp.setFillColor(sf::Color::Black);
	temp.setOutlineColor(sf::Color::Black);
	temp.setOutlineThickness(gridOutlineThickness);
	sf::CircleShape tempcir(1.5 * editorFieldSizeX / m);
	tempcir.setFillColor(sf::Color::Transparent);
	tempcir.setOutlineColor(sf::Color::Black);
	tempcir.setOutlineThickness(gridThickness + gridOutlineThickness);
	if (!this->isRotated) {
		temp.setSize(sf::Vector2f(editorFieldSizeX / m, gridThickness));
		temp.setPosition(sf::Vector2f(X, Y + 2 * editorFieldSizeY / n));
		window->draw(temp);
		temp.setPosition(sf::Vector2f(X + 4 * editorFieldSizeX / m, Y + 2 * editorFieldSizeY / n));
		window->draw(temp);
		tempcir.setPosition(sf::Vector2f(X + editorFieldSizeX / m, Y + 0.5 * editorFieldSizeY / n));
		window->draw(tempcir);
		temp.setSize(sf::Vector2f(3 * editorFieldSizeX / m, gridThickness));
		temp.rotate(45);
		temp.setPosition(sf::Vector2f(X + (2.5 - 1.5 / sqrt(2)) * editorFieldSizeX / m, Y + (2 - 1.5 / sqrt(2)) * editorFieldSizeY / n));
		window->draw(temp);
		temp.rotate(90);
		temp.setPosition(sf::Vector2f(X + (2.5 + 1.5 / sqrt(2)) * editorFieldSizeX / m, Y + (2 - 1.5 / sqrt(2)) * editorFieldSizeY / n));
		window->draw(temp);
	}
	else {
		temp.setSize(sf::Vector2f(gridThickness, editorFieldSizeY / n));
		temp.setPosition(sf::Vector2f(X + 2 * editorFieldSizeX / m, Y));
		window->draw(temp);
		temp.setPosition(sf::Vector2f(X + 2 * editorFieldSizeX / m, Y + 4 * editorFieldSizeY / n));
		window->draw(temp);
		tempcir.setPosition(sf::Vector2f(X + 0.5 * editorFieldSizeX / m, Y + editorFieldSizeY / n));
		window->draw(tempcir);
		temp.setSize(sf::Vector2f(3 * editorFieldSizeX / m, gridThickness));
		temp.rotate(45);
		temp.setPosition(sf::Vector2f(X + (2 - 1.5 / sqrt(2)) * editorFieldSizeX / m, Y + (2.5 - 1.5 / sqrt(2)) * editorFieldSizeY / n));
		window->draw(temp);
		temp.rotate(90);
		temp.setPosition(sf::Vector2f(X + (2 + 1.5 / sqrt(2)) * editorFieldSizeX / m, Y + (2.5 - 1.5 / sqrt(2)) * editorFieldSizeY / n));
		window->draw(temp);
	}
}

Lamp::Lamp()
	: EditorElement()
{
}

Lamp::Lamp(coord posT, int isRotatedT)
	: EditorElement(posT, isRotatedT)
{
}