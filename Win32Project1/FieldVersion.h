#pragma once
#include "EditorElement.h"
#include <string>

class FieldVersion
{
public:
	std::vector<Resistor*> resistors;
	std::vector<Battery*> batteries;
	std::vector<Lamp*> lamps;
	std::vector<EditorElement*> wires;
	std::vector<ChainVertex*> vertexes;
	std::vector<Capacitor*> capacitors;
	FieldVersion();
	FieldVersion(const std::vector<ChainVertex*> &ttvertexes, const std::vector<EditorElement*> &twires, const std::vector<Resistor*> &tresistors, const std::vector<Battery*> &tbatteries, const std::vector<Lamp*> &tlamps, const std::vector<Capacitor*> &tcapacitors);
	FieldVersion(const FieldVersion &temp);
	~FieldVersion();
	void saveToFile(std::string filename);
	void openFromFile(std::string filename);
};

