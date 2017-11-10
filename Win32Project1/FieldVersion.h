#pragma once
#include "EditorElement.h"

class FieldVersion
{
public:
	std::vector<Resistor*> resistors;
	std::vector<Battery*> batteries;
	std::vector<Lamp*> lamps;
	std::vector<EditorElement*> wires;
	std::vector<ChainVertex*> vertexes;
	FieldVersion();
	~FieldVersion();
};

