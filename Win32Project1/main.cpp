#define _CRT_SECURE_NO_WARNINGS
#include <SFML/Graphics.hpp>
#include <vector>
#include <algorithm>
#include <iostream>
#include <cstdio>
#include <cmath>
#include "InspectorSection.h"
#include "Editor.h"
#include "EditorElement.h"

float gridThickness = 3;
float gridOutlineThickness = 1.2;
float n, m;
float cellSize = 60;
float editorFieldSizeX = 1280;
float editorFieldSizeY = 720;
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
float inspectorWidth = 200;
float inspectorLeftTextMargin = 10;
float inspectorLineSize = 50;
sf::Font arial;
sf::Color mainColor = sf::Color(0, 120, 120);
sf::Vector2f sectionSize = sf::Vector2f(inspectorWidth, 2 * inspectorLineSize);

std::vector<Resistor> resistors;
std::vector<Battery> batteries;
std::vector<Lamp> lamps;
std::vector<EditorElement> wires;
sf::Texture toolbarTexture, itemTexture;


bool isColliding(float X1, float Y1, float X2, float Y2) {
	if (X2 > X2) {
		std::swap(X1, X2);
	}
	if (Y1 > Y2) {
		std::swap(Y1, Y2);
	}
	for (int i = 0; i < resistors.size(); i++) {
		float a = resistors[i].pos.x, b = resistors[i].pos.y;
		float c = a + resistors[i].isRotated * resistorSizeY + (1 - resistors[i].isRotated) * resistorSizeX,
			d = b + resistors[i].isRotated * resistorSizeX + (1 - resistors[i].isRotated) * resistorSizeY;
		if (std::min(X2, c)>std::max(X1, a) && std::min(Y2, d)>std::max(Y1, b)) {
			return 1;
		}
	}
	for (int i = 0; i < batteries.size(); i++) {
		float a = batteries[i].pos.x, b = batteries[i].pos.y;
		float c = a + batteries[i].isRotated * batterySizeY + (1 - batteries[i].isRotated) * batterySizeX,
			d = b + batteries[i].isRotated * batterySizeX + (1 - batteries[i].isRotated) * batterySizeY;
		if (std::min(X2, c)>std::max(X1, a) && std::min(Y2, d)>std::max(Y1, b)) {
			return 1;
		}
	}
	for (int i = 0; i < lamps.size(); i++) {
		float a = lamps[i].pos.x, b = lamps[i].pos.y;
		float c = a + lamps[i].isRotated * lampSizeY + (1 - lamps[i].isRotated) * lampSizeX,
			d = b + lamps[i].isRotated * lampSizeX + (1 - lamps[i].isRotated) * lampSizeY;
		if (std::min(X2, c)>std::max(X1, a) && std::min(Y2, d)>std::max(Y1, b)) {
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
			wires.push_back(EditorElement({ (float)i,(float)y }, 0));
		}
	}
	else {
		if (l < 0) {
			y = y + l;
			l = -l;
		}
		for (int i = y; i < y + l; i++) {
			wires.push_back(EditorElement({ (float)x,(float)i }, 1));
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
sf::RenderWindow window(sf::VideoMode(leftMargin + editorFieldSizeX + inspectorWidth, topMargin + editorFieldSizeY), "Electronic circuit editor");
sf::View view(window.getDefaultView());
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

void drawDottedLine(float x, float y, float length, float dotLength, int ishor, sf::Color color = sf::Color::Green) {
	sf::RectangleShape temp;
	temp.setFillColor(color);
	if (ishor) {
		for (float curX = x; curX < x + length; curX += 2 * dotLength) {
			temp.setSize(sf::Vector2f(std::min(dotLength, x + length - curX), gridThickness));
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
	tX1 = round(tX1);
	tX2 = (X2 - leftMargin) / editorFieldSizeX * m;
	tX2 = round(tX2);
	tY1 = (Y1 - topMargin) / editorFieldSizeY * n;
	tY1 = round(tY1);
	tY2 = (Y2 - topMargin) / editorFieldSizeY * n;
	tY2 = round(tY2);
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
	std::vector<EditorElement> twires;
	for (int i = 0; i < wires.size(); i++) {
		float a = wires[i].pos.x, b = wires[i].pos.y;
		float c = a + (1 - wires[i].isRotated),
			d = b + wires[i].isRotated;
		if (!(X2 > a && c > X1 && Y2 > b && d > Y1)) {
			twires.push_back(wires[i]);
		}
	}
	wires = twires;
}

bool isRotated = 0;

void drawResistorPreview() {
	float curX, curY;
	getCurrentFlooredFieldCoords(curX, curY);
	curX = curX*editorFieldSizeX / m + leftMargin;
	curY = curY*editorFieldSizeY / n + topMargin;
	if (!isRotated)
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
	resistors.push_back(Resistor({ curX, curY }, isRotated));
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
	lamps.push_back(Lamp({ curX,curY }, isRotated));
}

int curMode = 0;

bool isSelected = 0;
float selectionEndX = 0, selectionEndY = 0;

void startSelecting() {
	getCurrentFieldCoords(startX, startY);
	isStarted = 1;
	isSelected = 0;
}

bool isItemSelected = 0;
int selectedItemType = 0, selectedItemI = 0;

void selectItem() {
	float curX, curY;
	getCurrentFlooredFieldCoords(curX, curY);
	isItemSelected = 0;
	for (int i = 0; i < resistors.size(); i++) {
		float a = resistors[i].pos.x, b = resistors[i].pos.y;
		float c = a + resistors[i].isRotated * resistorSizeY + (1 - resistors[i].isRotated) * resistorSizeX,
			d = b + resistors[i].isRotated * resistorSizeX + (1 - resistors[i].isRotated) * resistorSizeY;
		if (curX >= a && curX <= c && curY >= b && curY <= d) {
			startX = a;
			startY = b;
			selectionEndX = c;
			selectionEndY = d;
			isItemSelected = 1;
			selectedItemType = 0;
			selectedItemI = i;
		}
	}
	for (int i = 0; i < batteries.size(); i++) {
		float a = batteries[i].pos.x, b = batteries[i].pos.y;
		float c = a + batteries[i].isRotated * batterySizeY + (1 - batteries[i].isRotated) * batterySizeX,
			d = b + batteries[i].isRotated * batterySizeX + (1 - batteries[i].isRotated) * batterySizeY;
		if (curX >= a && curX <= c && curY >= b && curY <= d) {
			startX = a;
			startY = b;
			selectionEndX = c;
			selectionEndY = d;
			isItemSelected = 1;
			selectedItemType = 1;
			selectedItemI = i;
		}
	}
	for (int i = 0; i < lamps.size(); i++) {
		float a = lamps[i].pos.x, b = lamps[i].pos.y;
		float c = a + lamps[i].isRotated * lampSizeY + (1 - lamps[i].isRotated) * lampSizeX,
			d = b + lamps[i].isRotated * lampSizeX + (1 - lamps[i].isRotated) * lampSizeY;
		if (curX >= a && curX <= c && curY >= b && curY <= d) {
			startX = a;
			startY = b;
			selectionEndX = c;
			selectionEndY = d;
			isItemSelected = 1;
			selectedItemType = 2;
			selectedItemI = i;
		}
	}
}

void finishSelecting() {
	getCurrentFieldCoords(selectionEndX, selectionEndY);
	if (selectionEndX == startX && selectionEndY == startY) {
		selectItem();
	}
	else {
		isItemSelected = 0;
	}
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

void deleteSelection() {
	if (!isSelected)
		return;
	float X1 = startX, X2 = selectionEndX, Y1 = startY, Y2 = selectionEndY;
	if (X1 > X2) {
		std::swap(X1, X2);
	}
	if (Y1 > Y2) {
		std::swap(Y1, Y2);
	}
	std::vector<Resistor> tresistors;
	std::vector<Battery> tbatteries;
	std::vector<Lamp> tlamps;
	deleteInnerWires(X1, Y1, X2, Y2);
	for (int i = 0; i < resistors.size(); i++) {
		float a = resistors[i].pos.x, b = resistors[i].pos.y;
		float c = a + resistors[i].isRotated * resistorSizeY + (1 - resistors[i].isRotated) * resistorSizeX,
			d = b + resistors[i].isRotated * resistorSizeX + (1 - resistors[i].isRotated) * resistorSizeY;
		if (!(X2 > a && c > X1 && Y2 > b && d > Y1)) {
			tresistors.push_back(resistors[i]);
		}
	}
	for (int i = 0; i < batteries.size(); i++) {
		float a = batteries[i].pos.x, b = batteries[i].pos.y;
		float c = a + batteries[i].isRotated * batterySizeY + (1 - batteries[i].isRotated) * batterySizeX,
			d = b + batteries[i].isRotated * batterySizeX + (1 - batteries[i].isRotated) * batterySizeY;
		if (!(X2 > a && c > X1 && Y2 > b && d > Y1)) {
			tbatteries.push_back(batteries[i]);
		}
	}
	for (int i = 0; i < lamps.size(); i++) {
		float a = lamps[i].pos.x, b = lamps[i].pos.y;
		float c = a + lamps[i].isRotated * lampSizeY + (1 - lamps[i].isRotated) * lampSizeX,
			d = b + lamps[i].isRotated * lampSizeX + (1 - lamps[i].isRotated) * lampSizeY;
		if (!(X2 > a && c > X1 && Y2 > b && d > Y1)) {
			tlamps.push_back(lamps[i]);
		}
	}
	resistors = tresistors;
	batteries = tbatteries;
	lamps = tlamps;
}

float moveStartX = 0, moveStartY = 0, moveEndX = 0, moveEndY = 0;
bool isMoving = 0;


void startMoving() {
	if (!isSelected)
		return;
	getCurrentFlooredFieldCoords(moveStartX, moveStartY);
	isMoving = 1;
}

void moveSelection(float deltaX, float deltaY) {
	float X1 = startX, X2 = selectionEndX, Y1 = startY, Y2 = selectionEndY;
	if (X1 > X2) {
		std::swap(X1, X2);
	}
	if (Y1 > Y2) {
		std::swap(Y1, Y2);
	}
	for (int i = 0; i < wires.size(); i++) {
		float a = wires[i].pos.x, b = wires[i].pos.y;
		float c = a + (1 - wires[i].isRotated),
			d = b + wires[i].isRotated;
		if (X2 > a && c > X1 && Y2 > b && d > Y1) {
			wires[i].pos.x += deltaX;
			wires[i].pos.y += deltaY;
		}
	}
	for (int i = 0; i < resistors.size(); i++) {
		float a = resistors[i].pos.x, b = resistors[i].pos.y;
		float c = a + resistors[i].isRotated * resistorSizeY + (1 - resistors[i].isRotated) * resistorSizeX,
			d = b + resistors[i].isRotated * resistorSizeX + (1 - resistors[i].isRotated) * resistorSizeY;
		if (X2 > a && c > X1 && Y2 > b && d > Y1) {
			resistors[i].pos.x += deltaX;
			resistors[i].pos.y += deltaY;
		}
	}
	for (int i = 0; i < batteries.size(); i++) {
		float a = batteries[i].pos.x, b = batteries[i].pos.y;
		float c = a + batteries[i].isRotated * batterySizeY + (1 - batteries[i].isRotated) * batterySizeX,
			d = b + batteries[i].isRotated * batterySizeX + (1 - batteries[i].isRotated) * batterySizeY;
		if (X2 > a && c > X1 && Y2 > b && d > Y1) {
			batteries[i].pos.x += deltaX;
			batteries[i].pos.y += deltaY;
		}
	}
	for (int i = 0; i < lamps.size(); i++) {
		float a = lamps[i].pos.x, b = lamps[i].pos.y;
		float c = a + lamps[i].isRotated * lampSizeY + (1 - lamps[i].isRotated) * lampSizeX,
			d = b + lamps[i].isRotated * lampSizeX + (1 - lamps[i].isRotated) * lampSizeY;
		if (X2 > a && c > X1 && Y2 > b && d > Y1) {
			lamps[i].pos.x += deltaX;
			lamps[i].pos.y += deltaY;
		}
	}
}

bool checkMoveInField() {
	float tstartX, tstartY, curX, curY;
	getCurrentFlooredFieldCoords(curX, curY);
	tstartX = startX - moveStartX + curX;
	tstartY = startY - moveStartY + curY;
	return (tstartX >= 0 && tstartY >= 0);
}

void finishMoving() {
	if (!checkMoveInField()) {
		isMoving = 0;
		curMode = 1;
	}
	if (!isMoving)
		return;
	getCurrentFlooredFieldCoords(moveEndX, moveEndY);
	moveSelection(moveEndX - moveStartX, moveEndY - moveStartY);
	startX += moveEndX - moveStartX;
	startY += moveEndY - moveStartY;
	selectionEndX += moveEndX - moveStartX;
	selectionEndY += moveEndY - moveStartY;
	isMoving = 0;
	curMode = 1;
}

void drawMovePreview() {
	float endX, endY, tstartX, tstartY, curX, curY;
	getCurrentFlooredFieldCoords(curX, curY);
	endX = selectionEndX - moveStartX + curX;
	endY = selectionEndY - moveStartY + curY;
	tstartX = startX - moveStartX + curX;
	tstartY = startY - moveStartY + curY;
	drawDottedRect(tstartX*editorFieldSizeX / m + leftMargin, tstartY*editorFieldSizeY / n + topMargin, endX*editorFieldSizeX / m + leftMargin, endY*editorFieldSizeY / n + topMargin, sf::Color::Green);
}

void rotateItem() {
	if (selectedItemType == 0) {
		resistors[selectedItemI].isRotated ^= 1;
		selectionEndX = startX + resistors[selectedItemI].isRotated * resistorSizeY + (1 - resistors[selectedItemI].isRotated) * resistorSizeX;
		selectionEndY = startY + resistors[selectedItemI].isRotated * resistorSizeX + (1 - resistors[selectedItemI].isRotated) * resistorSizeY;
	}
	else if (selectedItemType == 1) {
		batteries[selectedItemI].isRotated ^= 1;
		selectionEndX = startX + batteries[selectedItemI].isRotated * batterySizeY + (1 - batteries[selectedItemI].isRotated) * batterySizeX;
		selectionEndY = startY + batteries[selectedItemI].isRotated * batterySizeX + (1 - batteries[selectedItemI].isRotated) * batterySizeY;
	}
	else if (selectedItemType == 2) {
		lamps[selectedItemI].isRotated ^= 1;
		selectionEndX = startX + lamps[selectedItemI].isRotated * lampSizeY + (1 - lamps[selectedItemI].isRotated) * lampSizeX;
		selectionEndY = startY + lamps[selectedItemI].isRotated * lampSizeX + (1 - lamps[selectedItemI].isRotated) * lampSizeY;
	}
}

void drawItemInspector() {
	sf::RectangleShape inspectorBG(sf::Vector2f(inspectorWidth, topMargin + editorFieldSizeY));
	inspectorBG.setFillColor(sf::Color::White);
	inspectorBG.setOutlineColor(mainColor);
	inspectorBG.setOutlineThickness(separatorThickness);
	inspectorBG.setPosition(sf::Vector2f(leftMargin + editorFieldSizeX, 0));
	window.draw(inspectorBG);
	sf::RectangleShape nameTextBG(sf::Vector2f(inspectorWidth, inspectorLineSize));
	nameTextBG.setFillColor(sf::Color::White);
	nameTextBG.setOutlineColor(mainColor);
	nameTextBG.setOutlineThickness(separatorThickness);
	nameTextBG.setPosition(leftMargin + editorFieldSizeX, 0);
	window.draw(nameTextBG);
	sf::Text inspectorNameText("Properties:", arial);
	inspectorNameText.setPosition(sf::Vector2f(leftMargin + editorFieldSizeX - separatorThickness + inspectorLeftTextMargin, -separatorThickness));
	inspectorNameText.setFillColor(sf::Color::Black);
	window.draw(inspectorNameText);
	std::string itemType = "";
	if (isItemSelected) {
		if (selectedItemType == 0) {
			itemType = "Resistor";
		}
		else if (selectedItemType == 1) {
			itemType = "Battery";
		}
		else if (selectedItemType == 2) {
			itemType = "Lamp";
		}
	}
	InspectorSection elementType("Item type:", itemType, sf::Vector2f(leftMargin + editorFieldSizeX, inspectorLineSize + separatorThickness), sectionSize);
	elementType.draw(&window);
	InspectorSection elementVoltage("Voltage (V):", "", sf::Vector2f(leftMargin + editorFieldSizeX, 3 * (inspectorLineSize + separatorThickness)), sectionSize);
	elementVoltage.draw(&window);
	InspectorSection elementCurrent("Current (A):", "", sf::Vector2f(leftMargin + editorFieldSizeX, 5 * (inspectorLineSize + separatorThickness)), sectionSize);
	elementCurrent.draw(&window);
}

void resetFieldSize() {
	editorFieldSizeX = window.getSize().x - leftMargin - inspectorWidth;
	editorFieldSizeY = window.getSize().y - topMargin;
	n = editorFieldSizeY / cellSize;
	m = editorFieldSizeX / cellSize;
}

void drawDynamicBG() {
	sf::RectangleShape vline(sf::Vector2f(editorFieldSizeX, gridThickness));
	sf::RectangleShape hline(sf::Vector2f(gridThickness, editorFieldSizeY));
	sf::RectangleShape background(sf::Vector2f(leftMargin + editorFieldSizeX, topMargin + editorFieldSizeY));
	sf::RectangleShape menuBarBG(sf::Vector2f(leftMargin + editorFieldSizeX - separatorThickness, topMargin));
	menuBarBG.setFillColor(sf::Color::White);
	menuBarBG.setOutlineThickness(separatorThickness);
	menuBarBG.setOutlineColor(mainColor);
	background.setFillColor(sf::Color(240, 240, 240));
	vline.setFillColor(sf::Color(120, 120, 120));
	hline.setFillColor(sf::Color(120, 120, 120));
	vline.setSize(sf::Vector2f(editorFieldSizeX, gridThickness));
	hline.setSize(sf::Vector2f(gridThickness, editorFieldSizeY));
	sf::RectangleShape itemBarBG(sf::Vector2f(leftMargin, editorFieldSizeY));
	itemBarBG.setFillColor(sf::Color::White);
	itemBarBG.setOutlineColor(mainColor);
	itemBarBG.setOutlineThickness(separatorThickness);
	itemBarBG.setPosition(sf::Vector2f(0, topMargin));
	window.draw(background);
	window.draw(menuBarBG);
	window.draw(itemBarBG);
	for (int i = 1; i < n; i++) {
		vline.setPosition(sf::Vector2f(leftMargin, topMargin + editorFieldSizeY / n*i));
		window.draw(vline);
	}
	for (int i = 1; i < m; i++) {
		hline.setPosition(sf::Vector2f(leftMargin + editorFieldSizeX / m*i, topMargin));
		window.draw(hline);
	}
}

int launchMainWindow()
{
	//AllocConsole();
	//freopen("CONOUT$", "w", stdout);

	resetFieldSize();

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


	sf::RectangleShape wireIconBG(sf::Vector2f(leftMargin, itemIconSize));
	wireIconBG.setFillColor(sf::Color::White);
	wireIconBG.setOutlineColor(mainColor);
	wireIconBG.setOutlineThickness(separatorThickness);
	wireIconBG.setPosition(sf::Vector2f(0, topMargin + separatorThickness));

	sf::RectangleShape resistorIconBG(sf::Vector2f(leftMargin, itemIconSize));
	resistorIconBG.setFillColor(sf::Color::White);
	resistorIconBG.setOutlineColor(mainColor);
	resistorIconBG.setOutlineThickness(separatorThickness);
	resistorIconBG.setPosition(sf::Vector2f(0, topMargin + 2 * separatorThickness + itemIconSize));

	sf::RectangleShape batteryIconBG(sf::Vector2f(leftMargin, itemIconSize));
	batteryIconBG.setFillColor(sf::Color::White);
	batteryIconBG.setOutlineColor(mainColor);
	batteryIconBG.setOutlineThickness(separatorThickness);
	batteryIconBG.setPosition(sf::Vector2f(0, topMargin + 3 * separatorThickness + 2 * itemIconSize));

	sf::RectangleShape lampIconBG(sf::Vector2f(leftMargin, itemIconSize));
	lampIconBG.setFillColor(sf::Color::White);
	lampIconBG.setOutlineColor(mainColor);
	lampIconBG.setOutlineThickness(separatorThickness);
	lampIconBG.setPosition(sf::Vector2f(0, topMargin + 4 * separatorThickness + 3 * itemIconSize));


	sf::RectangleShape selectedItemBG(sf::Vector2f(itemIconSize + 2 * separatorThickness, itemIconSize + 2 * separatorThickness));
	selectedItemBG.setFillColor(sf::Color::Transparent);
	selectedItemBG.setOutlineThickness(separatorThickness);
	selectedItemBG.setOutlineColor(sf::Color(120, 120, 120, 255));

	sf::RectangleShape drawButtonBG(sf::Vector2f(topMargin, topMargin));
	drawButtonBG.setFillColor(sf::Color::White);
	drawButtonBG.setOutlineColor(mainColor);
	drawButtonBG.setOutlineThickness(separatorThickness);
	drawButtonBG.setPosition(sf::Vector2f(0, 0));

	sf::RectangleShape selectButtonBG(sf::Vector2f(topMargin, topMargin));
	selectButtonBG.setFillColor(sf::Color::White);
	selectButtonBG.setOutlineColor(mainColor);
	selectButtonBG.setOutlineThickness(separatorThickness);
	selectButtonBG.setPosition(sf::Vector2f(topMargin + separatorThickness, 0));

	sf::RectangleShape rotateButtonBG(sf::Vector2f(topMargin, topMargin));
	rotateButtonBG.setFillColor(sf::Color::White);
	rotateButtonBG.setOutlineColor(mainColor);
	rotateButtonBG.setOutlineThickness(separatorThickness);
	rotateButtonBG.setPosition(sf::Vector2f(3 * topMargin + 3 * separatorThickness, 0));

	sf::RectangleShape moveButtonBG(sf::Vector2f(topMargin, topMargin));
	moveButtonBG.setFillColor(sf::Color::White);
	moveButtonBG.setOutlineColor(mainColor);
	moveButtonBG.setOutlineThickness(separatorThickness);
	moveButtonBG.setPosition(sf::Vector2f(2 * topMargin + 2 * separatorThickness, 0));

	sf::RectangleShape deleteButtonBG(sf::Vector2f(topMargin, topMargin));
	deleteButtonBG.setFillColor(sf::Color::White);
	deleteButtonBG.setOutlineColor(mainColor);
	deleteButtonBG.setOutlineThickness(separatorThickness);
	deleteButtonBG.setPosition(sf::Vector2f(4 * topMargin + 4 * separatorThickness, 0));

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
	rotateButton.setPosition(sf::Vector2f(3 * topMargin + separatorThickness, -2 * separatorThickness));

	sf::Sprite moveButton;
	moveButton.setTexture(toolbarTexture);
	moveButton.setTextureRect(sf::IntRect(120, 0, 30, 30));
	moveButton.setPosition(sf::Vector2f(2 * topMargin, -2 * separatorThickness));

	sf::Sprite deleteButton;
	deleteButton.setTexture(toolbarTexture);
	deleteButton.setTextureRect(sf::IntRect(90, 0, 30, 30));
	deleteButton.setPosition(sf::Vector2f(4 * topMargin + 2 * separatorThickness, -2 * separatorThickness));

	sf::RectangleShape selectedModeBG(sf::Vector2f(topMargin + 2 * separatorThickness, topMargin + 2 * separatorThickness));
	selectedModeBG.setFillColor(sf::Color::Transparent);
	selectedModeBG.setOutlineColor(sf::Color(120, 120, 120, 255));
	selectedModeBG.setOutlineThickness(separatorThickness);
	selectedModeBG.setPosition(sf::Vector2f(-separatorThickness, -separatorThickness));

	while (window.isOpen())
	{
		sf::Event event;
		resetFieldSize();
		while (window.pollEvent(event))
		{
			if (event.type == sf::Event::Closed)
				window.close();
			else if (event.type == sf::Event::Resized)
				window.setView(view = sf::View(sf::FloatRect(0.f, 0.f,
					static_cast<float>(window.getSize().x),
					static_cast<float>(window.getSize().y))));
			else if (event.type == sf::Event::MouseWheelScrolled) {
				float delta = event.mouseWheelScroll.delta;
				cellSize *= (100.f - delta) / 100.f;
				gridThickness = round(editorFieldSizeX / m * 0.05);
				gridThickness = std::max(gridThickness, 1.f);
				gridOutlineThickness = gridThickness * 0.4;
				gridOutlineThickness = std::max(gridOutlineThickness, 1.f);
				resetFieldSize();
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
							else {
								curMode = 2;
								isStarted = 0;
								startMoving();
							}
						}
						else if (curMode == 2) {
							startMoving();
						}
					}
					else if (event.mouseButton.y >= topMargin && event.mouseButton.x < leftMargin) {
						currentItem = int(event.mouseButton.y - topMargin) / int(itemIconSize + separatorThickness);
						isRotated = 0;
						curMode = 0;
					}
					else if (event.mouseButton.y < topMargin) {
						int curButton = floor(event.mouseButton.x / (topMargin + separatorThickness));
						if (curButton == 0) {
							curMode = 0;
							isStarted = 0;
							isSelected = 0;
							isItemSelected = 0;
						}
						else if (curButton == 1) {
							curMode = 1;
							isStarted = 0;
							isSelected = 0;
							isItemSelected = 0;
						}
						else if (curButton == 2) {
							curMode = 2;
							isStarted = 0;
						}
						else if (curButton == 3) {
							if (isItemSelected) {
								rotateItem();
							}
							else {
								isRotated ^= 1;
							}
						}
						else if (curButton == 4) {
							deleteSelection();
							isSelected = 0;
							isItemSelected = 0;
						}
					}
				}
			}
			else if (event.type == sf::Event::MouseButtonReleased) {
				if (event.mouseButton.button == sf::Mouse::Left) {
					if (event.mouseButton.x >= leftMargin && event.mouseButton.y >= topMargin) {
						if (curMode == 0 && currentItem == 0 && isStarted) {
							finishConnectingVertexes(event.mouseButton.x, event.mouseButton.y);
						}
						else if (curMode == 1 && isStarted) {
							finishSelecting();
						}
						else if (curMode == 2 && isMoving) {
							finishMoving();
						}
					}
				}
			}
		}

		window.clear();
		drawDynamicBG();
		window.draw(wireIconBG);
		window.draw(resistorIconBG);
		window.draw(batteryIconBG);
		window.draw(lampIconBG);
		window.draw(drawButtonBG);
		window.draw(selectButtonBG);
		window.draw(rotateButtonBG);
		window.draw(moveButtonBG);
		window.draw(deleteButtonBG);
		sf::RectangleShape temp;
		temp.setFillColor(sf::Color::Black);
		temp.setOutlineThickness(gridOutlineThickness);
		temp.setOutlineColor(sf::Color::Black);
		for (int i = 0; i < wires.size(); i++) {
			if (wires[i].isRotated == 0) {
				temp.setSize(sf::Vector2f(editorFieldSizeX / m, roundf(gridThickness)));
				temp.setPosition(sf::Vector2f(leftMargin + wires[i].pos.x*editorFieldSizeX / m, topMargin + wires[i].pos.y*editorFieldSizeY / n));					window.draw(temp);
			}
			else {
				temp.setSize(sf::Vector2f(round(gridThickness), editorFieldSizeX / m));
				temp.setPosition(sf::Vector2f(leftMargin + wires[i].pos.x*editorFieldSizeX / m, topMargin + wires[i].pos.y*editorFieldSizeY / n));					window.draw(temp);
			}
		}
		for (int i = 0; i < resistors.size(); i++) {
			//drawResistor(resistors[i].pos.x, resistors[i].pos.y, resistors[i].isRotated);
			resistors[i].draw(&window);
		}
		for (int i = 0; i < batteries.size(); i++) {
			//drawBattery(batteries[i].pos.x, batteries[i].pos.y, batteries[i].isRotated);
			batteries[i].draw(&window);
		}
		for (int i = 0; i < lamps.size(); i++) {
			//drawLamp(lamps[i].pos.x, lamps[i].pos.y, lamps[i].isRotated);
			lamps[i].draw(&window);
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
		else if (curMode == 2) {
			if (isSelected) {
				drawSelection();
			}
			if (isMoving && checkMoveInField()) {
				drawMovePreview();
			}
			else {
				isMoving = 0;
				curMode = 1;
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
		window.draw(moveButton);
		window.draw(deleteButton);
		drawItemInspector();
		window.display();
	}

	return 0;
}

void launchStartingWindow() {
	sf::RenderWindow startWindow(sf::VideoMode(800, 600), "Electronic circuit editor");
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
	bool isError = 0;
	if (!toolbarTexture.loadFromFile("toolbar-sprite.png")) {
		isError = 1;
	}
	if (!itemTexture.loadFromFile("item-sprite.png")) {
		isError = 1;
	}
	if (!arial.loadFromFile("arial.ttf")) {
		isError = 1;
	}
	if (isError) {
		return 0;
	}
	window.setVisible(0);
	launchStartingWindow();
	window.setVisible(1);
	launchMainWindow();
}