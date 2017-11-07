#pragma once
#include <SFML/Graphics.hpp>
#define EDITOR_H
extern float gridThickness;
extern float gridOutlineThickness;
extern float n, m;
extern float cellSize;
extern float standartCellSize;
extern float editorFieldSizeX;
extern float editorFieldSizeY;
extern float topMargin;
extern float leftMargin;
extern float separatorThickness;
extern float itemIconSize;
extern float itemLeftMargin;
extern float itemTopMargin;
extern float standartDotLength;
extern float resistorSizeX;
extern float resistorSizeY;
extern float batterySizeX;
extern float batterySizeY;
extern float lampSizeX;
extern float lampSizeY;
extern float inspectorWidth;
extern float inspectorLeftTextMargin;
extern float inspectorLineSize;
extern sf::Font arial;
extern sf::Color mainColor;
extern sf::Vector2f sectionSize;
struct coord {
	float x, y;
};
extern sf::Texture *resistorTexture, *batteryTexture, *lampTexture;