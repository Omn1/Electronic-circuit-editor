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
#include "ChainVertex.h"
#include "ItemInspector.h"

float gridThickness = 2;
float gridOutlineThickness = 4;
float n, m;
float cellSize = 60;
float standartCellSize = 60;
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
float vertexRadius = 12;
sf::Font arial;
sf::Color mainColor = sf::Color(0, 120, 120);
sf::Vector2f sectionSize = sf::Vector2f(inspectorWidth, 2 * inspectorLineSize);
sf::Texture *resistorTexture = new sf::Texture();
sf::Texture *batteryTexture = new sf::Texture();
sf::Texture *lampTexture = new sf::Texture();

std::vector<Resistor*> resistors;
std::vector<Battery*> batteries;
std::vector<Lamp*> lamps;
std::vector<EditorElement*> wires;
std::vector<ChainVertex*> vertexes;
sf::Texture toolbarTexture, itemTexture;


bool checkStrictCollision(ElementRect a, ElementRect b) {
	return (a.x2 > b.x1 && b.x2 > a.x1 && a.y2 > b.y1 && b.y2 > a.y1);
}
bool checkUntrictCollision(ElementRect a, ElementRect b) {
	return (a.x2 >= b.x1 && b.x2 >= a.x1 && a.y2 >= b.y1 && b.y2 >= a.y1);
}


bool isColliding(ElementRect rect) {
	if (rect.x1 > rect.x2) {
		std::swap(rect.x1, rect.x2);
	}
	if (rect.y1 > rect.y2) {
		std::swap(rect.y1, rect.y2);
	}
	for (int i = 0; i < resistors.size(); i++) {
		if (checkStrictCollision(rect, resistors[i]->getElementRect())) {
			return 1;
		}
	}
	for (int i = 0; i < batteries.size(); i++) {
		if (checkStrictCollision(rect, batteries[i]->getElementRect())) {
			return 1;
		}
	}
	for (int i = 0; i < lamps.size(); i++) {
		if (checkStrictCollision(rect, lamps[i]->getElementRect())) {
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
			wires.push_back(new EditorElement({ (float)i,(float)y }, 0));
		}
	}
	else {
		if (l < 0) {
			y = y + l;
			l = -l;
		}
		for (int i = y; i < y + l; i++) {
			wires.push_back(new EditorElement({ (float)x,(float)i }, 1));
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
	if (isColliding({ tX1, tX2, tY1, tY2 })) {
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
	std::vector<EditorElement*> twires;
	for (int i = 0; i < wires.size(); i++) {
		float a = wires[i]->pos.x, b = wires[i]->pos.y;
		float c = a + (1 - wires[i]->isRotated),
			d = b + wires[i]->isRotated;
		if (!(X2 > a && c > X1 && Y2 > b && d > Y1)) {
			twires.push_back(wires[i]);
		}
	}
	wires = twires;
}

int isRotated = 0;

void drawItemPreview(float sizeX, float sizeY) {
	float curX, curY;
	getCurrentFlooredFieldCoords(curX, curY);
	curX = curX*cellSize + leftMargin;
	curY = curY*cellSize + topMargin;
	if (isRotated % 2 == 0)
		drawItemPreview(curX, curY, curX + sizeX * cellSize, curY + sizeY * cellSize);
	else
		drawItemPreview(curX, curY, curX + sizeY * cellSize, curY + sizeX * cellSize);
}

void putChainVertex(float curX = -1, float curY = -1)
{
	if (curX == -1 && curY == -1) {
		getCurrentFieldCoords(curX, curY);
	}
	if (isColliding({ curX, curX, curY, curY })) {
		return;
	}
	vertexes.push_back(new ChainVertex({ curX, curY }));
}

void drawResistorPreview() {
	drawItemPreview(resistorSizeX, resistorSizeY);
}

void putResistor() {
	float curX, curY;
	getCurrentFlooredFieldCoords(curX, curY);
	Resistor temp({curX, curY}, isRotated);
	if (isColliding(temp.getElementRect())) {
		return;
	}
	if (isRotated % 2 == 0) {
		deleteInnerWires(curX, curY, curX + resistorSizeX, curY + resistorSizeY);
	}
	else {
		deleteInnerWires(curX, curY, curX + resistorSizeY, curY + resistorSizeX);
	}
	resistors.push_back(new Resistor({ curX, curY }, isRotated));
	if (isRotated % 2 == 0) {
		putChainVertex(curX, curY + resistorSizeY / 2);
		putChainVertex(curX + resistorSizeX, curY + resistorSizeY / 2);
	}
	else {
		putChainVertex(curX + resistorSizeY / 2, curY);
		putChainVertex(curX + resistorSizeY / 2, curY + resistorSizeX);
	}
	resistors.back()->v1 = vertexes[vertexes.size() - 1];
	resistors.back()->v2 = vertexes[vertexes.size() - 2];
}

void drawBatteryPreview() {
	drawItemPreview(batterySizeX, batterySizeY);
}


void putBattery() {
	float curX, curY;
	getCurrentFlooredFieldCoords(curX, curY);
	Battery temp({ curX, curY }, isRotated);
	if (isColliding(temp.getElementRect())) {
		return;
	}
	if (isRotated % 2 == 0) {
		deleteInnerWires(curX, curY, curX + batterySizeX, curY + batterySizeY);
	}
	else {
		deleteInnerWires(curX, curY, curX + batterySizeY, curY + batterySizeX);
	}
	batteries.push_back(new Battery({ curX,curY }, isRotated ));
	if (isRotated % 2 == 0) {
		putChainVertex(curX, curY + batterySizeY / 2);
		putChainVertex(curX + batterySizeX, curY + batterySizeY / 2);
	}
	else {
		putChainVertex(curX + batterySizeY / 2, curY);
		putChainVertex(curX + batterySizeY / 2, curY + batterySizeX);
	}
	batteries.back()->v1 = vertexes[vertexes.size() - 1];
	batteries.back()->v2 = vertexes[vertexes.size() - 2];
}

void drawLampPreview() {
	drawItemPreview(lampSizeX, lampSizeY);
}


void putLamp() {
	float curX, curY;
	getCurrentFlooredFieldCoords(curX, curY);
	Lamp temp({ curX, curY }, isRotated);
	if (isColliding(temp.getElementRect())) {
		return;
	}
	if (isRotated % 2 == 0) {
		deleteInnerWires(curX, curY, curX + lampSizeX, curY + lampSizeY);
	}
	else {
		deleteInnerWires(curX, curY, curX + lampSizeY, curY + lampSizeX);
	}
	lamps.push_back(new Lamp({ curX,curY }, isRotated));
	if (isRotated % 2 == 0) {
		putChainVertex(curX, curY + lampSizeY / 2);
		putChainVertex(curX + lampSizeX, curY + lampSizeY / 2);
	}
	else {
		putChainVertex(curX + lampSizeY / 2, curY);
		putChainVertex(curX + lampSizeY / 2, curY + lampSizeX);
	}
	lamps.back()->v1 = vertexes[vertexes.size() - 1];
	lamps.back()->v2 = vertexes[vertexes.size() - 2];
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

void setSelection(ElementRect newSel) {
	startX = newSel.x1;
	startY = newSel.y1;
	selectionEndX = newSel.x2;
	selectionEndY = newSel.y2;
}

void selectItem() {
	float curX, curY;
	getCurrentFlooredFieldCoords(curX, curY);
	isItemSelected = 0;
	isSelected = 0;
	ElementRect temp = { curX, curX, curY, curY };
	for (int i = 0; i < resistors.size(); i++) {
		if (checkUntrictCollision(temp, resistors[i]->getElementRect())) {
			setSelection(resistors[i]->getElementRect());
			isSelected = 1;
			isItemSelected = 1;
			selectedItemType = 0;
			selectedItemI = i;
		}
	}
	for (int i = 0; i < batteries.size(); i++) {
		if (checkUntrictCollision(temp, batteries[i]->getElementRect())) {
			setSelection(batteries[i]->getElementRect());
			isItemSelected = 1;
			isSelected = 1;
			selectedItemType = 1;
			selectedItemI = i;
		}
	}
	for (int i = 0; i < lamps.size(); i++) {
		if (checkUntrictCollision(temp, lamps[i]->getElementRect())) {
			setSelection(lamps[i]->getElementRect());
			isItemSelected = 1;
			isSelected = 1;
			selectedItemType = 2;
			selectedItemI = i;
		}
	}
}

void finishSelecting() {
	getCurrentFieldCoords(selectionEndX, selectionEndY);
	if (selectionEndX == startX && selectionEndY == startY) {
		selectItem();
		isStarted = 0;
		return;
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
	drawDottedRect(startX*cellSize + leftMargin, startY*cellSize + topMargin, endX*cellSize + leftMargin, endY*cellSize + topMargin, sf::Color::Green);
}

bool isInSelection(float x, float y) {
	x -= leftMargin;
	x /= cellSize;
	y -= topMargin;
	y /= cellSize;
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
	std::vector<Resistor*> tresistors;
	std::vector<Battery*> tbatteries;
	std::vector<Lamp*> tlamps;
	std::vector<ChainVertex*> tvertexes;
	std::vector<int> isVertexSelected(vertexes.size(),1);
	deleteInnerWires(X1, Y1, X2, Y2);
	ElementRect temp = { X1,X2,Y1,Y2 };
	for (int i = 0; i < resistors.size(); i++) {
		if (!(checkStrictCollision(temp,resistors[i]->getElementRect()))) {
			tresistors.push_back(resistors[i]);
		}
		else {
			tvertexes.push_back(resistors[i]->v1);
			tvertexes.push_back(resistors[i]->v2);
		}
	}
	for (int i = 0; i < batteries.size(); i++) {
		if (!(checkStrictCollision(temp, batteries[i]->getElementRect()))) {
			tbatteries.push_back(batteries[i]);
		}
		else {
			tvertexes.push_back(batteries[i]->v1);
			tvertexes.push_back(batteries[i]->v2);
		}
	}
	for (int i = 0; i < lamps.size(); i++) {
		if (!(checkStrictCollision(temp, lamps[i]->getElementRect()))) {
			tlamps.push_back(lamps[i]);
		}
		else {
			tvertexes.push_back(lamps[i]->v1);
			tvertexes.push_back(lamps[i]->v2);
		}
	}
	std::sort(tvertexes.begin(), tvertexes.end());
	for (int i = 0; i < vertexes.size(); i++) {
		if (checkStrictCollision(temp, { vertexes[i]->pos.x,vertexes[i]->pos.x,vertexes[i]->pos.y,vertexes[i]->pos.y }) || (std::binary_search(tvertexes.begin(), tvertexes.end(), vertexes[i]))) {
			isVertexSelected[i] = 0;
		}
	}
	tvertexes.clear(); 
	for (int i = 0; i < vertexes.size(); i++) {
		if (isVertexSelected[i]) {
			tvertexes.push_back(vertexes[i]);
		}
	}
	resistors = tresistors;
	batteries = tbatteries;
	lamps = tlamps;
	vertexes = tvertexes;
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
	ElementRect temp = { X1,X2,Y1,Y2 };
	for (int i = 0; i < vertexes.size(); i++) {
		if (checkStrictCollision(temp, { vertexes[i]->pos.x,vertexes[i]->pos.x,vertexes[i]->pos.y,vertexes[i]->pos.y })) {
			vertexes[i]->pos.x += deltaX;
			vertexes[i]->pos.y += deltaY;
		}
	}
	for (int i = 0; i < wires.size(); i++) {
		float a = wires[i]->pos.x, b = wires[i]->pos.y;
		float c = a + (1 - wires[i]->isRotated),
			d = b + wires[i]->isRotated;
		if (X2 > a && c > X1 && Y2 > b && d > Y1) {
			wires[i]->move(deltaX, deltaY);
		}
	}
	for (int i = 0; i < resistors.size(); i++) {
		if (checkStrictCollision(temp, resistors[i]->getElementRect())) {
			resistors[i]->move(deltaX, deltaY);
		}
	}
	for (int i = 0; i < batteries.size(); i++) {
		if (checkStrictCollision(temp, batteries[i]->getElementRect())) {
			batteries[i]->move(deltaX, deltaY);
		}
	}
	for (int i = 0; i < lamps.size(); i++) {
		if (checkStrictCollision(temp, lamps[i]->getElementRect())) {
			lamps[i]->move(deltaX, deltaY);
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
	drawDottedRect(tstartX*cellSize + leftMargin, tstartY*cellSize + topMargin, endX*cellSize + leftMargin, endY*cellSize + topMargin, sf::Color::Green);
}

void rotateItem() {
	if (selectedItemType == 0) {
		resistors[selectedItemI]->rotate();
		setSelection(resistors[selectedItemI]->getElementRect());
	}
	else if (selectedItemType == 1) {
		batteries[selectedItemI]->rotate();
		setSelection(batteries[selectedItemI]->getElementRect());
	}
	else if (selectedItemType == 2) {
		lamps[selectedItemI]->rotate();
		setSelection(lamps[selectedItemI]->getElementRect());
	}
}

ItemInspector inspector;

void drawItemInspector() {
	if (isItemSelected) {
		if (selectedItemType == 0) {
			inspector.sections = resistors[selectedItemI]->getInspectorElements();
		}
		else if (selectedItemType == 1) {
			inspector.sections = batteries[selectedItemI]->getInspectorElements();
		}
		else if (selectedItemType == 2) {
			inspector.sections = lamps[selectedItemI]->getInspectorElements();
		}
	}
	else {
		inspector.sections = std::vector<std::pair<std::string, std::string> >();
	}
	inspector.draw(&window);
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
		vline.setPosition(sf::Vector2f(leftMargin, topMargin + cellSize*i));
		window.draw(vline);
	}
	for (int i = 1; i < m; i++) {
		hline.setPosition(sf::Vector2f(leftMargin + cellSize*i, topMargin));
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
				vertexRadius *= (100.f - delta) / 100.f;
				gridThickness = round(cellSize * 2 / 60);
				gridThickness = std::max(gridThickness, 1.f);
				gridOutlineThickness = gridThickness * 2;
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
							else if (currentItem == 4) {
								putChainVertex();
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
								isRotated++;
								isRotated %= 4;
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
			if (wires[i]->isRotated == 0) {
				temp.setSize(sf::Vector2f(cellSize, roundf(gridThickness)));
				temp.setPosition(sf::Vector2f(leftMargin + wires[i]->pos.x*cellSize, topMargin + wires[i]->pos.y*cellSize));
				window.draw(temp);
			}
			else {
				temp.setSize(sf::Vector2f(round(gridThickness), cellSize));
				temp.setPosition(sf::Vector2f(leftMargin + wires[i]->pos.x*cellSize, topMargin + wires[i]->pos.y*cellSize));
				window.draw(temp);
			}
		}
		for (int i = 0; i < resistors.size(); i++) {
			//drawResistor(resistors[i].pos.x, resistors[i].pos.y, resistors[i].isRotated);
			resistors[i]->draw(&window);
		}
		for (int i = 0; i < batteries.size(); i++) {
			//drawBattery(batteries[i].pos.x, batteries[i].pos.y, batteries[i].isRotated);
			batteries[i]->draw(&window);
		}
		for (int i = 0; i < lamps.size(); i++) {
			//drawLamp(lamps[i].pos.x, lamps[i].pos.y, lamps[i].isRotated);
			lamps[i]->draw(&window);
		}
		for (int i = 0; i < vertexes.size(); i++) {
			vertexes[i]->draw(&window);
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
	if (!resistorTexture->loadFromFile("resistor.png")) {
		isError = 1;
	}
	if (!batteryTexture->loadFromFile("battery.png")) {
		isError = 1;
	}
	if (!lampTexture->loadFromFile("lamp.png")) {
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