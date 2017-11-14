#include "FieldVersion.h"
#include <fstream>


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

void FieldVersion::saveToFile(std::string filename)
{
	std::vector<ChainVertex*> tvertexes(vertexes.size());
	for (int i = 0; i < vertexes.size(); i++) {
		tvertexes[i] = vertexes[i];
	}
	std::sort(tvertexes.begin(), tvertexes.end());
	std::ofstream cout(filename);
	cout << tvertexes.size() << "\n";
	for (int i = 0; i < tvertexes.size(); i++) {
		cout << tvertexes[i]->pos.x << " " << tvertexes[i]->pos.y << "\n";
	}
	cout << resistors.size() << "\n";
	for (int i = 0; i < resistors.size(); i++) {
		cout << resistors[i]->pos.x << " " << resistors[i]->pos.y << " " << resistors[i]->isRotated << " " << resistors[i]->resistance << "\n";
		auto pos = std::lower_bound(tvertexes.begin(), tvertexes.end(), resistors[i]->v1);
		if (pos == tvertexes.end() || (*pos != resistors[i]->v1)) {
			cout << -1 << " ";
		}
		else {
			cout << pos - tvertexes.begin() << " ";
		}
		pos = std::lower_bound(tvertexes.begin(), tvertexes.end(), resistors[i]->v2);
		if (pos == tvertexes.end() || (*pos != resistors[i]->v2)) {
			cout << -1 << " ";
		}
		else {
			cout << pos - tvertexes.begin() << " ";
		}
		cout << "\n";
	}
	cout << batteries.size() << "\n";
	for (int i = 0; i < batteries.size(); i++) {
		cout << batteries[i]->pos.x << " " << batteries[i]->pos.y << " " << batteries[i]->isRotated << " " << batteries[i]->realVoltage << "\n";
		auto pos = std::lower_bound(tvertexes.begin(), tvertexes.end(), batteries[i]->v1);
		if (pos == tvertexes.end() || (*pos != batteries[i]->v1)) {
			cout << -1 << " ";
		}
		else {
			cout << pos - tvertexes.begin() << " ";
		}
		pos = std::lower_bound(tvertexes.begin(), tvertexes.end(), batteries[i]->v2);
		if (pos == tvertexes.end() || (*pos != batteries[i]->v2)) {
			cout << -1 << " ";
		}
		else {
			cout << pos - tvertexes.begin() << " ";
		}
		cout << "\n";
	}
	cout << lamps.size() << "\n";
	for (int i = 0; i < lamps.size(); i++) {
		cout << lamps[i]->pos.x << " " << lamps[i]->pos.y << " " << lamps[i]->isRotated << " " << lamps[i]->resistance << "\n";
		auto pos = std::lower_bound(tvertexes.begin(), tvertexes.end(), lamps[i]->v1);
		if (pos == tvertexes.end() || (*pos != lamps[i]->v1)) {
			cout << -1 << " ";
		}
		else {
			cout << pos - tvertexes.begin() << " ";
		}
		pos = std::lower_bound(tvertexes.begin(), tvertexes.end(), lamps[i]->v2);
		if (pos == tvertexes.end() || (*pos != lamps[i]->v2)) {
			cout << -1 << " ";
		}
		else {
			cout << pos - tvertexes.begin() << " ";
		}
		cout << "\n";
	}
	cout << wires.size() << "\n";
	for (int i = 0; i < wires.size(); i++) {
		cout << wires[i]->pos.x << " " << wires[i]->pos.y << " " << wires[i]->isRotated << "\n";
		auto pos = std::lower_bound(tvertexes.begin(), tvertexes.end(), wires[i]->v1);
		if (pos == tvertexes.end() || (*pos != wires[i]->v1)) {
			cout << -1 << " ";
		}
		else {
			cout << pos - tvertexes.begin() << " ";
		}
		pos = std::lower_bound(tvertexes.begin(), tvertexes.end(), wires[i]->v2);
		if (pos == tvertexes.end() || (*pos != wires[i]->v2)) {
			cout << -1 << " ";
		}
		else {
			cout << pos - tvertexes.begin() << " ";
		}
		cout << "\n";
	}
}

void FieldVersion::openFromFile(std::string filename)
{
	std::ifstream cin(filename);
	int temp;
	cin >> temp;
	vertexes.resize(temp);
	for (int i = 0; i < temp; i++) {
		float x, y;
		cin >> x >> y;
		vertexes[i] = new ChainVertex({ x,y });
	}
	cin >> temp;
	resistors.resize(temp);
	for (int i = 0; i < temp; i++) {
		float x, y, isRot, resistance;
		cin >> x >> y >> isRot >> resistance;
		resistors[i] = new Resistor({ x,y }, isRot);
		resistors[i]->resistance = resistance;
		int v1n, v2n;
		cin >> v1n >> v2n;
		if (v1n != -1)
			resistors[i]->v1 = vertexes[v1n];
		if (v2n != -2)
			resistors[i]->v2 = vertexes[v2n];
	}
	cin >> temp;
	batteries.resize(temp);
	for (int i = 0; i < temp; i++) {
		float x, y, isRot, voltage;
		cin >> x >> y >> isRot >> voltage;
		batteries[i] = new Battery({ x,y }, isRot);
		batteries[i]->realVoltage = voltage;
		int v1n, v2n;
		cin >> v1n >> v2n;
		if (v1n != -1)
			batteries[i]->v1 = vertexes[v1n];
		if (v2n != -1)
			batteries[i]->v2 = vertexes[v2n];
	}
	cin >> temp;
	lamps.resize(temp);
	for (int i = 0; i < temp; i++) {
		float x, y, isRot, resistance;
		cin >> x >> y >> isRot >> resistance;
		lamps[i] = new Lamp({ x,y }, isRot);
		lamps[i]->resistance = resistance;
		int v1n, v2n;
		cin >> v1n >> v2n;
		if (v1n != -1)
			lamps[i]->v1 = vertexes[v1n];
		if (v2n != -1)
			lamps[i]->v2 = vertexes[v2n];
	}
	cin >> temp;
	wires.resize(temp);
	for (int i = 0; i < temp; i++) {
		float x, y, isRot;
		cin >> x >> y >> isRot;
		wires[i] = new EditorElement({ x,y }, isRot);
		int v1n, v2n;
		cin >> v1n >> v2n;
		if (v1n != -1)
			wires[i]->v1 = vertexes[v1n];
		if (v2n != -1)
			wires[i]->v2 = vertexes[v2n];
	}
}
