#include "FieldVersion.h"



FieldVersion::FieldVersion()
{
}

FieldVersion::FieldVersion(std::vector<ChainVertex*> tvertexes, std::vector<EditorElement*> twires, std::vector<Resistor*> tresistors, std::vector<Battery*> tbatteries, std::vector<Lamp*> tlamps)
{
	std::sort(tvertexes.begin(), tvertexes.end());
	for (int i = 0; i < tvertexes.size(); i++) {
		vertexes.push_back(new ChainVertex(*tvertexes[i]));
	}
	for (int i = 0; i < twires.size(); i++) {
		wires.push_back(new EditorElement(*twires[i]));
		auto pos = std::lower_bound(tvertexes.begin(), tvertexes.end(), twires[i]->v1);
		//if(pos != tvertexes.end() && (*pos ))
	}
	for (int i = 0; i < tresistors.size(); i++) {
		resistors.push_back(new Resistor(*tresistors[i]));
	}
	for (int i = 0; i < tbatteries.size(); i++) {
		batteries.push_back(new Battery(*tbatteries[i]));
	}
	for (int i = 0; i < tlamps.size(); i++) {
		lamps.push_back(new Lamp(*tlamps[i]));
	}
}


FieldVersion::~FieldVersion()
{
}
