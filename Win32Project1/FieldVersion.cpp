#include "FieldVersion.h"



FieldVersion::FieldVersion()
{
}

FieldVersion::FieldVersion(const std::vector<ChainVertex*> &ttvertexes, const std::vector<EditorElement*> &twires, const std::vector<Resistor*> &tresistors, const std::vector<Battery*> &tbatteries, const std::vector<Lamp*> &tlamps)
{
	std::vector<ChainVertex*> tvertexes(ttvertexes.size());
	for (int i = 0; i < ttvertexes.size(); i++) {
		tvertexes[i] = ttvertexes[i];
	}
	std::sort(tvertexes.begin(), tvertexes.end());
	for (int i = 0; i < tvertexes.size(); i++) {
		vertexes.push_back(new ChainVertex(tvertexes[i]));
	}
	for (int i = 0; i < twires.size(); i++) {
		wires.push_back(new EditorElement(twires[i]));
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
		resistors.push_back(new Resistor(tresistors[i]));
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
		batteries.push_back(new Battery(tbatteries[i]));
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
		lamps.push_back(new Lamp(tlamps[i]));
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

FieldVersion::FieldVersion(const FieldVersion & temp)
{
	wires.resize(temp.wires.size());
	for (int i = 0; i < wires.size(); i++) {
		wires[i] = temp.wires[i];
	}
	resistors.resize(temp.resistors.size());
	for (int i = 0; i < resistors.size(); i++) {
		resistors[i] = temp.resistors[i];
	}
	batteries.resize(temp.batteries.size());
	for (int i = 0; i < batteries.size(); i++) {
		batteries[i] = temp.batteries[i];
	}
	lamps.resize(temp.lamps.size());
	for (int i = 0; i < lamps.size(); i++) {
		lamps[i] = temp.lamps[i];
	}
	vertexes.resize(temp.vertexes.size());
	for (int i = 0; i < vertexes.size(); i++) {
		vertexes[i] = temp.vertexes[i];
	}
}


FieldVersion::~FieldVersion()
{
}
