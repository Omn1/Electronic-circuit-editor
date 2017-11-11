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
		if (pos != tvertexes.end() && (*pos == twires[i]->v1)) {
			wires[i]->v1 = vertexes[pos - tvertexes.begin()];
		}
		pos = std::lower_bound(tvertexes.begin(), tvertexes.end(), twires[i]->v2);
		if (pos != tvertexes.end() && (*pos == twires[i]->v2)) {
			wires[i]->v2 = vertexes[pos - tvertexes.begin()];
		}
	}
	for (int i = 0; i < tresistors.size(); i++) {
		resistors.push_back(new Resistor(*tresistors[i]));
		auto pos = std::lower_bound(tvertexes.begin(), tvertexes.end(), tresistors[i]->v1);
		if (pos != tvertexes.end() && (*pos == tresistors[i]->v1)) {
			resistors[i]->v1 = vertexes[pos - tvertexes.begin()];
		}
		pos = std::lower_bound(tvertexes.begin(), tvertexes.end(), tresistors[i]->v2);
		if (pos != tvertexes.end() && (*pos == tresistors[i]->v2)) {
			resistors[i]->v2 = vertexes[pos - tvertexes.begin()];
		}
	}
	for (int i = 0; i < tbatteries.size(); i++) {
		batteries.push_back(new Battery(*tbatteries[i]));
		auto pos = std::lower_bound(tvertexes.begin(), tvertexes.end(), tbatteries[i]->v1);
		if (pos != tvertexes.end() && (*pos == tbatteries[i]->v1)) {
			batteries[i]->v1 = vertexes[pos - tvertexes.begin()];
		}
		pos = std::lower_bound(tvertexes.begin(), tvertexes.end(), tbatteries[i]->v2);
		if (pos != tvertexes.end() && (*pos == tbatteries[i]->v2)) {
			batteries[i]->v2 = vertexes[pos - tvertexes.begin()];
		}
	}
	for (int i = 0; i < tlamps.size(); i++) {
		lamps.push_back(new Lamp(*tlamps[i]));
		auto pos = std::lower_bound(tvertexes.begin(), tvertexes.end(), tlamps[i]->v1);
		if (pos != tvertexes.end() && (*pos == tlamps[i]->v1)) {
			lamps[i]->v1 = vertexes[pos - tvertexes.begin()];
		}
		pos = std::lower_bound(tvertexes.begin(), tvertexes.end(), tlamps[i]->v2);
		if (pos != tvertexes.end() && (*pos == tlamps[i]->v2)) {
			lamps[i]->v2 = vertexes[pos - tvertexes.begin()];
		}
	}
}


FieldVersion::~FieldVersion()
{
}
