#define _CRT_SECURE_NO_WARNINGS
#include <SFML/Graphics.hpp>
#include <vector>
#include <algorithm>
#include <iostream>
#include <cstdio>
#include <cmath>

float gridThickness = 5;
float gridOutlineThickness = 2;
float n = 30, m = 54;
float editorFieldSizeX = 1820;
float editorFieldSizeY = 1030;
float topMargin = 50;
float leftMargin = 100;
float separatorThickness = -5;
float itemIconSize = 100;
float itemLeftMargin = 10;
float itemTopMargin = 35;
float standartDotLength = 5;
float resistorSizeX = 5;
float resistorSizeY = 2;
float batterySizeX = 5;
float batterySizeY = 4;
float lampSizeX = 5;
float lampSizeY = 4;
bool mat[1000][1000][2];

std::vector<std::pair<std::pair<float, float>, bool > > resistors, batteries, lamps;

bool isColliding(float X1, float Y1, float X2, float Y2) {
	if (X2 > X2) {
		std::swap(X1, X2);
	}
	if (Y1 > Y2) {
		std::swap(Y1, Y2);
	}
	for (int i = 0; i < resistors.size(); i++) {
		float a = resistors[i].first.first, b = resistors[i].first.second;
		float c = a + resistors[i].second * resistorSizeY + (1 - resistors[i].second) * resistorSizeX,
			d = b + resistors[i].second * resistorSizeX + (1 - resistors[i].second) * resistorSizeY;
		if (X2 > a && c > X1 && Y2 > b && d > Y1) {
			return 1;
		}
	}
	for (int i = 0; i < batteries.size(); i++) {
		float a = batteries[i].first.first, b = batteries[i].first.second;
		float c = a + batteries[i].second * batterySizeY + (1 - batteries[i].second) * batterySizeX,
			d = b + batteries[i].second * batterySizeX + (1 - batteries[i].second) * batterySizeY;
		if (X2 > a && c > X1 && Y2 > b && d > Y1) {
			return 1;
		}
	}
	for (int i = 0; i < lamps.size(); i++) {
		float a = lamps[i].first.first, b = lamps[i].first.second;
		float c = a + lamps[i].second * lampSizeY + (1 - lamps[i].second) * lampSizeX,
			d = b + lamps[i].second * lampSizeX + (1 - lamps[i].second) * lampSizeY;
		if (X2 > a && c > X1 && Y2 > b && d > Y1) {
			return 1;
		}
	}
	return 0;
}

void addLine(int x, int y, int l, int ishor) {
	if (ishor) {
		if (l < 0) {
			x = x + l;
			l = -l;
		}
		for (int i = x; i < x + l; i++) {
			mat[i][y][0] = 1;
		}
	}
	else {
		if (l < 0) {
			y = y + l;
			l = -l;
		}
		for (int i = y; i < y + l; i++) {
			mat[x][i][1] = 1;
		}
	}
}

void connectVertexes(float startX, float startY, float endX, float endY) {
	addLine(startX, startY, endY - startY, 0);
	addLine(startX, endY, endX - startX, 1);
}

float startX = 0, startY = 0;
bool isStarted = 0;

void startConnectingVertexes(float mouseX, float mouseY) {
	isStarted = 1;
	startX = mouseX;
	startX -= leftMargin;
	startX = round(startX * m / editorFieldSizeX);
	startY = mouseY;
	startY -= topMargin;
	startY = round(startY * n / editorFieldSizeY);
}

void finishConnectingVertexes(float mouseX, float mouseY) {
	isStarted = 0;
	float endX = mouseX;
	endX -= leftMargin;
	endX = round(endX * m / editorFieldSizeX);
	float endY = mouseY;
	endY -= topMargin;
	endY = round(endY * n / editorFieldSizeY);
	connectVertexes(startX, startY, endX, endY);
}
sf::RenderWindow window(sf::VideoMode(leftMargin + editorFieldSizeX, topMargin + editorFieldSizeY), "Electronic circuit editor", sf::Style::Fullscreen);

void drawWirePreview() {
	sf::RectangleShape temp;
	temp.setOutlineThickness(gridOutlineThickness);
	temp.setOutlineColor(sf::Color::Black);
	float endX = sf::Mouse::getPosition(window).x;
	endX -= leftMargin;
	endX = round(endX * m / editorFieldSizeX);
	float endY = sf::Mouse::getPosition(window).y;
	endY -= topMargin;
	endY = round(endY * n / editorFieldSizeY);
	temp.setFillColor(sf::Color::Green);
	temp.setOutlineColor(sf::Color::Green);

	temp.setSize(sf::Vector2f((endX - startX) / m * editorFieldSizeX, gridThickness));
	temp.setPosition(sf::Vector2f(startX / m * editorFieldSizeX + leftMargin, endY / n * editorFieldSizeY + topMargin));
	window.draw(temp);

	temp.setSize(sf::Vector2f(gridThickness, (endY - startY) / n * editorFieldSizeY));
	temp.setPosition(sf::Vector2f(startX / m * editorFieldSizeX + leftMargin, startY / n * editorFieldSizeY + topMargin));
	window.draw(temp);
}

int currentItem = 0;

void drawDottedLine(float x, float y, float length, float dotLength, int ishor, sf::Color color=sf::Color::Green) {
	sf::RectangleShape temp;
	temp.setFillColor(color);
	if (ishor) {
		for (float curX = x; curX < x + length; curX += 2 * dotLength) {
			temp.setSize(sf::Vector2f(std::min(dotLength, x+length-curX), gridThickness));
			temp.setPosition(sf::Vector2f(curX, y));
			window.draw(temp);
		}
	}
	else {
		for (float curY = y; curY < y + length; curY += 2 * dotLength) {
			temp.setSize(sf::Vector2f(gridThickness, std::min(y + length - curY, dotLength)));
			temp.setPosition(sf::Vector2f(x, curY));
			window.draw(temp);
		}
	}
}

void drawDottedRect(float X1, float Y1, float X2, float Y2, sf::Color color) {
	if (X1 > X2) {
		std::swap(X1, X2);
	}
	if (Y1 > Y2) {
		std::swap(Y1, Y2);
	}
	drawDottedLine(X1, Y1, X2 - X1, standartDotLength, 1, color);
	drawDottedLine(X2, Y1, Y2 - Y1, standartDotLength, 0, color);
	drawDottedLine(X1, Y2, X2 - X1, standartDotLength, 1, color);
	drawDottedLine(X1, Y1, Y2 - Y1, standartDotLength, 0, color);
}

void drawItemPreview(float X1, float Y1, float X2, float Y2) {
	if (X1 > X2) {
		std::swap(X1, X2);
	}
	if (Y1 > Y2) {
		std::swap(Y1, Y2);
	}
	float tX1, tX2, tY1, tY2;
	tX1 = (X1 - leftMargin) / editorFieldSizeX * m;
	tX2 = (X2 - leftMargin) / editorFieldSizeX * m;
	tY1 = (Y1 - topMargin) / editorFieldSizeY * n;
	tY2 = (Y2 - topMargin) / editorFieldSizeY * n;
	sf::Color color = sf::Color::Green;
	if (isColliding(tX1, tY1, tX2, tY2)) {
		color = sf::Color::Red;
	}
	drawDottedRect(X1, Y1, X2, Y2, color);
}
void getCurrentFieldCoords(float &X, float &Y) {
	float curX = sf::Mouse::getPosition(window).x;
	curX -= leftMargin;
	curX = round(curX * m / editorFieldSizeX);
	float curY = sf::Mouse::getPosition(window).y;
	curY -= topMargin;
	curY = round(curY * n / editorFieldSizeY);
	X = curX;
	Y = curY;
}
void getCurrentFlooredFieldCoords(float &X, float &Y) {
	float curX = sf::Mouse::getPosition(window).x;
	curX -= leftMargin;
	curX = floor(curX * m / editorFieldSizeX);
	float curY = sf::Mouse::getPosition(window).y;
	curY -= topMargin;
	curY = floor(curY * n / editorFieldSizeY);
	X = curX;
	Y = curY;
}

void deleteInnerWires(int X1, int Y1, int X2, int Y2) {
	for (int i = X1 + 1; i < X2; i++) {
		for (int j = Y1 + 1; j < Y2; j++) {
			mat[i][j][0] = 0;
			mat[i][j][1] = 0;
		}
	}
	for (int i = X1 + 1; i < X2; i++) {
		mat[i][Y1][1] = 0;
	}
	for (int j = Y1 + 1; j < Y2; j++) {
		mat[X1][j][0] = 0;
	}
}

bool isRotated = 0;

void drawResistorPreview() {
	float curX, curY;
	getCurrentFlooredFieldCoords(curX, curY);
	curX = curX*editorFieldSizeX / m + leftMargin;
	curY = curY*editorFieldSizeY / n + topMargin;
	if(!isRotated)
		drawItemPreview(curX, curY, curX + resistorSizeX * editorFieldSizeX / m, curY + resistorSizeY * editorFieldSizeY / n);
	else
		drawItemPreview(curX, curY, curX + resistorSizeY * editorFieldSizeX / m, curY + resistorSizeX * editorFieldSizeY / n);
}


void putResistor() {
	float curX, curY;
	getCurrentFlooredFieldCoords(curX, curY);
	if (isColliding(curX, curY, curX + isRotated*resistorSizeY + (1 - isRotated)*resistorSizeX, curY + isRotated*resistorSizeX + (1 - isRotated)*resistorSizeY)) {
		return;
	}
	if (!isRotated) {
		deleteInnerWires(curX, curY, curX + resistorSizeX, curY + resistorSizeY);
	}
	else {
		deleteInnerWires(curX, curY, curX + resistorSizeY, curY + resistorSizeX);
	}
	resistors.push_back({ {curX,curY}, isRotated });
}

void drawResistor(float X, float Y, bool isrot) {
	X *= editorFieldSizeX / m;
	X += leftMargin;
	Y *= editorFieldSizeY / n;
	Y += topMargin;
	sf::RectangleShape temp;
	temp.setFillColor(sf::Color::Black);
	temp.setOutlineColor(sf::Color::Black);
	temp.setOutlineThickness(gridOutlineThickness);
	if (!isrot) {
		temp.setSize(sf::Vector2f(editorFieldSizeX / m, gridThickness));
		temp.setPosition(sf::Vector2f(X, Y + editorFieldSizeY / n));
		window.draw(temp);
		temp.setPosition(sf::Vector2f(X + 4 * editorFieldSizeX / m, Y + editorFieldSizeY / n));
		window.draw(temp);
		temp.setSize(sf::Vector2f(3 * editorFieldSizeX / m, gridThickness));
		temp.setPosition(sf::Vector2f(X + editorFieldSizeX / m, Y + 0.5*editorFieldSizeY / n));
		window.draw(temp);
		temp.setPosition(sf::Vector2f(X + editorFieldSizeX / m, Y + 1.5*editorFieldSizeY / n));
		window.draw(temp);
		temp.setSize(sf::Vector2f(gridThickness, editorFieldSizeY / n));
		temp.setPosition(sf::Vector2f(X + editorFieldSizeX / m, Y + 0.5*editorFieldSizeY / n));
		window.draw(temp);
		temp.setPosition(sf::Vector2f(X + 4 * editorFieldSizeX / m, Y + 0.5*editorFieldSizeY / n));
		window.draw(temp);
	}
	else {
		temp.setSize(sf::Vector2f(gridThickness, editorFieldSizeY / n));
		temp.setPosition(sf::Vector2f(X + editorFieldSizeX / m, Y));
		window.draw(temp);
		temp.setPosition(sf::Vector2f(X + editorFieldSizeX / m, Y + 4 * editorFieldSizeY / n));
		window.draw(temp);
		temp.setSize(sf::Vector2f(gridThickness, 3 * editorFieldSizeY / n));
		temp.setPosition(sf::Vector2f(X + 0.5 * editorFieldSizeX / m, Y + editorFieldSizeY / n));
		window.draw(temp);
		temp.setPosition(sf::Vector2f(X + 1.5 * editorFieldSizeX / m, Y + editorFieldSizeY / n));
		window.draw(temp);
		temp.setSize(sf::Vector2f(editorFieldSizeX / m, gridThickness));
		temp.setPosition(sf::Vector2f(X + 0.5 * editorFieldSizeX / m, Y + editorFieldSizeY / n));
		window.draw(temp);
		temp.setPosition(sf::Vector2f(X + 0.5 * editorFieldSizeX / m, Y + 4 * editorFieldSizeY / n));
		window.draw(temp);
	}
}

void drawBatteryPreview() {
	float curX, curY;
	getCurrentFlooredFieldCoords(curX, curY);
	curX = curX*editorFieldSizeX / m + leftMargin;
	curY = curY*editorFieldSizeY / n + topMargin;
	if (!isRotated)
		drawItemPreview(curX, curY, curX + batterySizeX * editorFieldSizeX / m, curY + batterySizeY * editorFieldSizeY / n);
	else
		drawItemPreview(curX, curY, curX + batterySizeY * editorFieldSizeX / m, curY + batterySizeX * editorFieldSizeY / n);
}


void putBattery() {
	float curX, curY;
	getCurrentFlooredFieldCoords(curX, curY);
	if (isColliding(curX, curY, curX + isRotated*batterySizeY + (1 - isRotated)*batterySizeX, curY + isRotated*batterySizeX + (1 - isRotated)*batterySizeY)) {
		return;
	}
	if (!isRotated) {
		deleteInnerWires(curX, curY, curX + batterySizeX, curY + batterySizeY);
	}
	else {
		deleteInnerWires(curX, curY, curX + batterySizeY, curY + batterySizeX);
	}
	batteries.push_back({ { curX,curY }, isRotated });
}

void drawBattery(float X, float Y, bool isrot) {
	X *= editorFieldSizeX / m;
	X += leftMargin;
	Y *= editorFieldSizeY / n;
	Y += topMargin;
	sf::RectangleShape temp;
	temp.setFillColor(sf::Color::Black);
	temp.setOutlineColor(sf::Color::Black);
	temp.setOutlineThickness(gridOutlineThickness);
	if (!isrot) {
		temp.setSize(sf::Vector2f(2 * editorFieldSizeX / m, gridThickness));
		temp.setPosition(sf::Vector2f(X, Y + 2 * editorFieldSizeY / n));
		window.draw(temp);
		temp.setPosition(sf::Vector2f(X + 3 * editorFieldSizeX / m, Y + 2 * editorFieldSizeY / n));
		window.draw(temp);
		temp.setSize(sf::Vector2f(gridThickness, 3 * editorFieldSizeY / n));
		temp.setPosition(sf::Vector2f(X + 2 * editorFieldSizeX / m, Y + 0.5 * editorFieldSizeY / n));
		window.draw(temp);
		temp.setSize(sf::Vector2f(gridThickness, editorFieldSizeY / n));
		temp.setPosition(sf::Vector2f(X + 3 * editorFieldSizeX / m, Y + 1.5 * editorFieldSizeY / n));
		window.draw(temp);
	}
	else {
		temp.setSize(sf::Vector2f(gridThickness, 2 * editorFieldSizeY / n));
		temp.setPosition(sf::Vector2f(X + 2 * editorFieldSizeX / m, Y));
		window.draw(temp);
		temp.setPosition(sf::Vector2f(X + 2 * editorFieldSizeX / m, Y + 3 * editorFieldSizeY / n));
		window.draw(temp);
		temp.setSize(sf::Vector2f(3 * editorFieldSizeX / m, gridThickness));
		temp.setPosition(sf::Vector2f(X + 0.5 * editorFieldSizeX / m, Y + 2 * editorFieldSizeY / n));
		window.draw(temp);
		temp.setSize(sf::Vector2f(editorFieldSizeX / m, gridThickness));
		temp.setPosition(sf::Vector2f(X + 1.5 * editorFieldSizeX / m, Y + 3 * editorFieldSizeY / n));
		window.draw(temp);
	}
}

void drawLampPreview() {
	float curX, curY;
	getCurrentFlooredFieldCoords(curX, curY);
	curX = curX*editorFieldSizeX / m + leftMargin;
	curY = curY*editorFieldSizeY / n + topMargin;
	if (!isRotated)
		drawItemPreview(curX, curY, curX + lampSizeX * editorFieldSizeX / m, curY + lampSizeY * editorFieldSizeY / n);
	else
		drawItemPreview(curX, curY, curX + lampSizeY * editorFieldSizeX / m, curY + lampSizeX * editorFieldSizeY / n);
}


void putLamp() {
	float curX, curY;
	getCurrentFlooredFieldCoords(curX, curY);
	if (isColliding(curX, curY, curX + isRotated*lampSizeY + (1 - isRotated)*lampSizeX, curY + isRotated*lampSizeX + (1 - isRotated)*lampSizeY)) {
		return;
	}
	if (!isRotated) {
		deleteInnerWires(curX, curY, curX + lampSizeX, curY + lampSizeY);
	}
	else {
		deleteInnerWires(curX, curY, curX + lampSizeY, curY + lampSizeX);
	}
	lamps.push_back({ { curX,curY }, isRotated });
}

void drawLamp(float X, float Y, bool isrot) {
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
	if (!isrot) {
		temp.setSize(sf::Vector2f(editorFieldSizeX / m, gridThickness));
		temp.setPosition(sf::Vector2f(X, Y + 2 * editorFieldSizeY / n));
		window.draw(temp);
		temp.setPosition(sf::Vector2f(X + 4 * editorFieldSizeX / m, Y + 2 * editorFieldSizeY / n));
		window.draw(temp);
		tempcir.setPosition(sf::Vector2f(X + editorFieldSizeX / m, Y + 0.5 * editorFieldSizeY / n));
		window.draw(tempcir);
		temp.setSize(sf::Vector2f(3 * editorFieldSizeX / m, gridThickness));
		temp.rotate(45);
		temp.setPosition(sf::Vector2f(X + (2.5 - 1.5 / sqrt(2)) * editorFieldSizeX / m, Y + (2 - 1.5 / sqrt(2)) * editorFieldSizeY / n));
		window.draw(temp);
		temp.rotate(90);
		temp.setPosition(sf::Vector2f(X + (2.5 + 1.5 / sqrt(2)) * editorFieldSizeX / m, Y + (2 - 1.5 / sqrt(2)) * editorFieldSizeY / n));
		window.draw(temp);
	}
	else {
		temp.setSize(sf::Vector2f(gridThickness, editorFieldSizeY / n));
		temp.setPosition(sf::Vector2f(X + 2 * editorFieldSizeX / m, Y));
		window.draw(temp);
		temp.setPosition(sf::Vector2f(X + 2 * editorFieldSizeX / m, Y + 4 * editorFieldSizeY / n));
		window.draw(temp);
		tempcir.setPosition(sf::Vector2f(X + 0.5 * editorFieldSizeX / m, Y + editorFieldSizeY / n));
		window.draw(tempcir);
		temp.setSize(sf::Vector2f(3 * editorFieldSizeX / m, gridThickness));
		temp.rotate(45);
		temp.setPosition(sf::Vector2f(X + (2 - 1.5 / sqrt(2)) * editorFieldSizeX / m, Y + (2.5 - 1.5 / sqrt(2)) * editorFieldSizeY / n));
		window.draw(temp);
		temp.rotate(90);
		temp.setPosition(sf::Vector2f(X + (2 + 1.5 / sqrt(2)) * editorFieldSizeX / m, Y + (2.5 - 1.5 / sqrt(2)) * editorFieldSizeY / n));
		window.draw(temp);
	}
}

int curMode = 0;

bool isSelected = 0;
float selectionEndX = 0, selectionEndY = 0;

void startSelecting() {
	getCurrentFieldCoords(startX, startY);
	isStarted = 1;
	isSelected = 0;
}

void finishSelecting() {
	getCurrentFieldCoords(selectionEndX, selectionEndY);
	isSelected = 1;
	isStarted = 0;
}

void drawSelection() {
	float endX, endY;
	if (!isSelected) {
		getCurrentFieldCoords(endX, endY);
	}
	else {
		endX = selectionEndX;
		endY = selectionEndY;
	}
	drawDottedRect(startX*editorFieldSizeX / m + leftMargin, startY*editorFieldSizeY / n + topMargin, endX*editorFieldSizeX / m + leftMargin, endY*editorFieldSizeY / n + topMargin, sf::Color::Green);
}

bool isInSelection(float x, float y) {
	x -= leftMargin;
	x /= editorFieldSizeX / m;
	y -= topMargin;
	y /= editorFieldSizeY / n;
	return isSelected && (x >= startX && x <= selectionEndX) && (y >= startY && y <= selectionEndY);
}

int launchMainWindow()
{
	//AllocConsole();
	//freopen("CONOUT$", "w", stdout);
	
	sf::Texture toolbarTexture;
	if (!toolbarTexture.loadFromFile("toolbar-sprite.png")) {
		std::cout << "error opening texture\n";
		return 0;
	}
	sf::Texture itemTexture;
	if (!itemTexture.loadFromFile("item-sprite.png")) {
		std::cout << "error opening texture\n";
		return 0;
	}
	toolbarTexture.setSmooth(true);

	sf::Sprite wireItem;
	wireItem.setTexture(itemTexture);
	wireItem.setTextureRect(sf::IntRect(0, 20, 70, 20));
	wireItem.setPosition(sf::Vector2f(itemLeftMargin - separatorThickness, topMargin + itemTopMargin));

	sf::Sprite resistorItem;
	resistorItem.setTexture(itemTexture);
	resistorItem.setTextureRect(sf::IntRect(0, 0, 70, 20));
	resistorItem.setPosition(sf::Vector2f(itemLeftMargin - separatorThickness, topMargin + itemTopMargin + itemIconSize + separatorThickness));

	sf::Sprite batteryItem;
	batteryItem.setTexture(itemTexture);
	batteryItem.setTextureRect(sf::IntRect(0, 40, 70, 20));
	batteryItem.setPosition(sf::Vector2f(itemLeftMargin - separatorThickness, topMargin + itemTopMargin + 2 * (itemIconSize + separatorThickness)));

	sf::Sprite lampItem;
	lampItem.setTexture(itemTexture);
	lampItem.setTextureRect(sf::IntRect(0, 60, 70, 20));
	lampItem.setPosition(sf::Vector2f(itemLeftMargin - separatorThickness, topMargin + itemTopMargin + 3 * (itemIconSize + separatorThickness)));


	sf::RectangleShape vline(sf::Vector2f(editorFieldSizeX, gridThickness));
	sf::RectangleShape hline(sf::Vector2f(gridThickness, editorFieldSizeY));
	sf::RectangleShape background(sf::Vector2f(leftMargin + editorFieldSizeX, topMargin + editorFieldSizeY));
	sf::RectangleShape menuBarBG(sf::Vector2f(leftMargin + editorFieldSizeX, topMargin));
	
	sf::RectangleShape wireIconBG(sf::Vector2f(leftMargin, itemIconSize));
	wireIconBG.setFillColor(sf::Color::White);
	wireIconBG.setOutlineColor(sf::Color::Red);
	wireIconBG.setOutlineThickness(separatorThickness);
	wireIconBG.setPosition(sf::Vector2f(0, topMargin+separatorThickness));

	sf::RectangleShape resistorIconBG(sf::Vector2f(leftMargin, itemIconSize));
	resistorIconBG.setFillColor(sf::Color::White);
	resistorIconBG.setOutlineColor(sf::Color::Red);
	resistorIconBG.setOutlineThickness(separatorThickness);
	resistorIconBG.setPosition(sf::Vector2f(0, topMargin + 2*separatorThickness + itemIconSize));

	sf::RectangleShape batteryIconBG(sf::Vector2f(leftMargin, itemIconSize));
	batteryIconBG.setFillColor(sf::Color::White);
	batteryIconBG.setOutlineColor(sf::Color::Red);
	batteryIconBG.setOutlineThickness(separatorThickness);
	batteryIconBG.setPosition(sf::Vector2f(0, topMargin + 3 * separatorThickness + 2 * itemIconSize));
	
	sf::RectangleShape lampIconBG(sf::Vector2f(leftMargin, itemIconSize));
	lampIconBG.setFillColor(sf::Color::White);
	lampIconBG.setOutlineColor(sf::Color::Red);
	lampIconBG.setOutlineThickness(separatorThickness);
	lampIconBG.setPosition(sf::Vector2f(0, topMargin + 4 * separatorThickness + 3 * itemIconSize));

	menuBarBG.setFillColor(sf::Color::White);
	menuBarBG.setOutlineThickness(separatorThickness);
	menuBarBG.setOutlineColor(sf::Color::Red);
	background.setFillColor(sf::Color::White);
	vline.setFillColor(sf::Color(0,0,0,120));
	hline.setFillColor(sf::Color(0,0,0,120));

	sf::RectangleShape selectedItemBG(sf::Vector2f(itemIconSize + 2 * separatorThickness, itemIconSize + 2 * separatorThickness));
	selectedItemBG.setFillColor(sf::Color::Transparent);
	selectedItemBG.setOutlineThickness(separatorThickness);
	selectedItemBG.setOutlineColor(sf::Color(120, 120, 120, 255));

	sf::RectangleShape drawButtonBG(sf::Vector2f(topMargin, topMargin));
	drawButtonBG.setFillColor(sf::Color::White);
	drawButtonBG.setOutlineColor(sf::Color::Red);
	drawButtonBG.setOutlineThickness(separatorThickness);
	drawButtonBG.setPosition(sf::Vector2f(0, 0));

	sf::RectangleShape selectButtonBG(sf::Vector2f(topMargin, topMargin));
	selectButtonBG.setFillColor(sf::Color::White);
	selectButtonBG.setOutlineColor(sf::Color::Red);
	selectButtonBG.setOutlineThickness(separatorThickness);
	selectButtonBG.setPosition(sf::Vector2f(topMargin + separatorThickness, 0));

	sf::RectangleShape rotateButtonBG(sf::Vector2f(topMargin, topMargin));
	rotateButtonBG.setFillColor(sf::Color::White);
	rotateButtonBG.setOutlineColor(sf::Color::Red);
	rotateButtonBG.setOutlineThickness(separatorThickness);
	rotateButtonBG.setPosition(sf::Vector2f(2*topMargin + 2*separatorThickness, 0));

	sf::Sprite drawButton;
	drawButton.setTexture(toolbarTexture);
	drawButton.setTextureRect(sf::IntRect(0, 0, 30, 30));
	drawButton.setPosition(sf::Vector2f(-2 * separatorThickness, -2 * separatorThickness));

	sf::Sprite selectButton;
	selectButton.setTexture(toolbarTexture);
	selectButton.setTextureRect(sf::IntRect(30, 0, 30, 30));
	selectButton.setPosition(sf::Vector2f(topMargin - separatorThickness, -2 * separatorThickness));
	
	sf::Sprite rotateButton;
	rotateButton.setTexture(toolbarTexture);
	rotateButton.setTextureRect(sf::IntRect(60, 0, 30, 30));
	rotateButton.setPosition(sf::Vector2f(2*topMargin, -2 * separatorThickness));

	sf::RectangleShape selectedModeBG(sf::Vector2f(topMargin + 2 * separatorThickness, topMargin + 2 * separatorThickness));
	selectedModeBG.setFillColor(sf::Color::Transparent);
	selectedModeBG.setOutlineColor(sf::Color(120, 120, 120, 255));
	selectedModeBG.setOutlineThickness(separatorThickness);
	selectedModeBG.setPosition(sf::Vector2f(-separatorThickness, -separatorThickness));

	while (window.isOpen())
	{
		sf::Event event;
		while (window.pollEvent(event))
		{
			if (event.type == sf::Event::Closed)
				window.close();
			else if (event.type == sf::Event::MouseWheelScrolled) {
				float delta = event.mouseWheelScroll.delta;
				n *= (100.f + delta) / 100.f;
				m *= (100.f + delta) / 100.f;
				gridThickness = round(editorFieldSizeX / m * 0.1);
				gridThickness = std::max(gridThickness, 1.f);
				gridOutlineThickness = gridThickness * 0.4;
				gridOutlineThickness = std::max(gridOutlineThickness, 1.f);
				vline.setSize(sf::Vector2f(editorFieldSizeX, gridThickness));
				hline.setSize(sf::Vector2f(gridThickness, editorFieldSizeY));
			}
			else if (event.type == sf::Event::MouseButtonPressed) {
				if (event.mouseButton.button == sf::Mouse::Left) {
					if (event.mouseButton.x >= leftMargin && event.mouseButton.y >= topMargin) {
						if (curMode == 0) {
							if (currentItem == 0) {
								startConnectingVertexes(event.mouseButton.x, event.mouseButton.y);
							}
							else if (currentItem == 1) {
								putResistor();
							}
							else if (currentItem == 2) {
								putBattery();
							}
							else if (currentItem == 3) {
								putLamp();
							}
						}
						else if (curMode == 1) {
							if (!isInSelection(event.mouseButton.x, event.mouseButton.y)) {
								startSelecting();
							}
						}
					}
					else if (event.mouseButton.y >= topMargin && event.mouseButton.x < leftMargin) {
						currentItem = int(event.mouseButton.y - topMargin) / int(itemIconSize + separatorThickness);
						isRotated = 0;
					}
					else if (event.mouseButton.y < topMargin) {
						int curButton = floor(event.mouseButton.x / (topMargin + separatorThickness));
						if (curButton == 0) {
							curMode = 0;
							isStarted = 0;
							isSelected = 0;
						}
						else if(curButton == 1) {
							curMode = 1;
							isStarted = 0;
							isSelected = 0;
						}
						if (curButton == 2) {
							isRotated ^= 1;
						}
					}
				}
			}
			else if (event.type == sf::Event::MouseButtonReleased) {
				if (event.mouseButton.button == sf::Mouse::Left) {
					if (event.mouseButton.x >= leftMargin && event.mouseButton.y >= topMargin && isStarted) {
						if (curMode == 0 && currentItem == 0) { 
							finishConnectingVertexes(event.mouseButton.x, event.mouseButton.y); 
						}
						else if (curMode == 1) {
							finishSelecting();
						}
					}
				}
			}
		}

		window.clear();
		window.draw(background);
		window.draw(menuBarBG);
		window.draw(wireIconBG);
		window.draw(resistorIconBG);
		window.draw(batteryIconBG);
		window.draw(lampIconBG);
		window.draw(drawButtonBG);
		window.draw(selectButtonBG);
		window.draw(rotateButtonBG);
		for (int i = 1; i < n; i++) {
			vline.setPosition(sf::Vector2f(leftMargin, topMargin + editorFieldSizeY / n*i));
			window.draw(vline);
		}
		for (int i = 1; i < m; i++) {
			hline.setPosition(sf::Vector2f(leftMargin + editorFieldSizeX / m*i, topMargin));
			window.draw(hline);
		}
		sf::RectangleShape temp;
		temp.setFillColor(sf::Color::Black);
		temp.setOutlineThickness(gridOutlineThickness);
		temp.setOutlineColor(sf::Color::Black);
		for (int i = 0; i < 1000; i++) {
			for (int j = 0; j < 1000; j++) {
				if (mat[i][j][0]) {
					temp.setSize(sf::Vector2f(editorFieldSizeX / m, round(gridThickness)));
					temp.setPosition(sf::Vector2f(leftMargin + i*editorFieldSizeX / m, topMargin + j*editorFieldSizeY / n));
					window.draw(temp);
				}
				if (mat[i][j][1]) {
					temp.setSize(sf::Vector2f(round(gridThickness), editorFieldSizeX / m));
					temp.setPosition(sf::Vector2f(leftMargin + i*editorFieldSizeX / m, topMargin + j*editorFieldSizeY / n));
					window.draw(temp);
				}
			}
		}
		for (int i = 0; i < resistors.size(); i++) {
			drawResistor(resistors[i].first.first, resistors[i].first.second, resistors[i].second);
		}
		for (int i = 0; i < batteries.size(); i++) {
			drawBattery(batteries[i].first.first, batteries[i].first.second, batteries[i].second);
		}
		for (int i = 0; i < lamps.size(); i++) {
			drawLamp(lamps[i].first.first, lamps[i].first.second, lamps[i].second);
		}
		if (curMode == 0) {
			if (isStarted) {
				if (sf::Mouse::getPosition(window).x >= leftMargin && sf::Mouse::getPosition(window).y >= topMargin) {
					if (currentItem == 0) {
						drawWirePreview();
					}
				}
				else {
					isStarted = 0;
				}
			}
			if (sf::Mouse::getPosition(window).x >= leftMargin && sf::Mouse::getPosition(window).y >= topMargin) {
				if (currentItem == 1) {
					drawResistorPreview();
				}
				else if (currentItem == 2) {
					drawBatteryPreview();
				}
				else if (currentItem == 3) {
					drawLampPreview();
				}
			}
		}
		else if (curMode == 1) {
			if (isStarted) {
				if (sf::Mouse::getPosition(window).x >= leftMargin && sf::Mouse::getPosition(window).y >= topMargin) {
					drawSelection();
				}
				else {
					isStarted = 0;
				}
			}
			if (isSelected) {
				drawSelection();
			}
		}
		window.draw(wireItem);
		window.draw(resistorItem);
		window.draw(batteryItem);
		window.draw(lampItem);
		selectedItemBG.setPosition(sf::Vector2f(-separatorThickness, topMargin + currentItem*(itemIconSize + separatorThickness)));
		window.draw(selectedItemBG);
		selectedModeBG.setPosition(sf::Vector2f(-separatorThickness + curMode*(topMargin + separatorThickness), -separatorThickness));
		window.draw(selectedModeBG);
		window.draw(drawButton);
		window.draw(selectButton);
		window.draw(rotateButton);
		window.display();
	}

	return 0;
}

void launchStartingWindow() {
	sf::RenderWindow startWindow(sf::VideoMode(800,600), "Electronic circuit editor");
	sf::Texture startWindowTexture;
	if (!startWindowTexture.loadFromFile("start-window-texture.png")) {
		return;
	}
	sf::Sprite startWindowSprite;
	startWindowSprite.setTexture(startWindowTexture);
	while (startWindow.isOpen())
	{
		sf::Event event;
		while (startWindow.pollEvent(event))
		{
			if (event.type == sf::Event::Closed)
				startWindow.close();
			else if (event.type == sf::Event::KeyPressed) {
				startWindow.close();
				return;
			}
		}

		startWindow.clear();
		startWindow.draw(startWindowSprite);
		startWindow.display();
	}
}

int main() {
	window.setVisible(0);
	launchStartingWindow();
	window.setVisible(1);
	launchMainWindow();
}