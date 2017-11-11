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
	FieldVersion(const std::vector<ChainVertex*> &ttvertexes, const std::vector<EditorElement*> &twires, const std::vector<Resistor*> &tresistors, const std::vector<Battery*> &tbatteries, const std::vector<Lamp*> &tlamps);
	FieldVersion(const FieldVersion &temp);
	~FieldVersion();
};

