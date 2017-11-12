#include "SchemeCalculator.h"
#include <complex>
#include <cmath>
#include <iostream>

int min(int x, int y)
{
	return x < y ? x : y;
}

int max(int x, int y)
{
	return x > y ? x : y;
}

// user needed functions block
SchemeCalculator::SchemeCalculator()
{
	clear();
}

void SchemeCalculator::clear()
{
	capacitors.clear();
	inductors.clear();
	DCBatteries.clear();
	ACBatteries.clear();

	capacitorsCurrent.clear();
	inductorsCurrent.clear();

	n = 0;
	a.clear();
	graph.clear();
	wires.clear();
	potentials.clear();
}
// end of user needed function block


// wires block
void SchemeCalculator::addWire(int i, int j)
{
	if (i >= n) updateSize(i + 1);
	if (j >= n) updateSize(j + 1);
	wires[i][j]++;
	wires[j][i]++;
}

void SchemeCalculator::removeWire(int i, int j)
{
	wires[i][j]--;
	wires[i][j]--;
}
// end of wires block


// resistors block
void SchemeCalculator::addResistance(int i, int j, double resistance)
{
	if (i >= n) updateSize(i + 1);
	if (j >= n) updateSize(j + 1);
	addEdge(i, j, comp(resistance));
}

void SchemeCalculator::addResistance(Edge resistor)
{
	if (resistor.getI() >= n) updateSize(resistor.getI() + 1);
	if (resistor.getJ() >= n) updateSize(resistor.getJ() + 1);
	addEdge(resistor.getI(), resistor.getJ(), comp(resistor.getLength()));
}

void SchemeCalculator::removeResistance(int i, int j, double resistance)
{
	addEdge(i, j, comp(-resistance));
}

void SchemeCalculator::removeResistance(Edge resistor)
{
	addEdge(resistor.getI(), resistor.getJ(), comp(-resistor.getLength()));
}
// end of resistors block


// inductors block
int SchemeCalculator::addInductor(int i, int j, double inductance)
{
	if (i >= n) updateSize(i + 1);
	if (j >= n) updateSize(j + 1);
	inductors.push_back(Edge(i, j, inductance));
	inductorsCurrent.push_back(0);
	return inductors.size() - 1;
}

int SchemeCalculator::addInductor(Edge inductor)
{
	if (inductor.getI() >= n) updateSize(inductor.getI() + 1);
	if (inductor.getJ() >= n) updateSize(inductor.getJ() + 1);
	inductors.push_back(inductor);
	inductorsCurrent.push_back(0);
	return inductors.size() - 1;
}

void SchemeCalculator::removeInductor(int inductor)
{
	inductors.erase(std::next(inductors.begin(), inductor));
	inductorsCurrent.erase(std::next(inductorsCurrent.begin(), inductor));
}
// end of inductors block


// capacitors block
int SchemeCalculator::addCapacitor(int i, int j, double capacity)
{
	if (i >= n) updateSize(i + 1);
	if (j >= n) updateSize(j + 1);
	capacitors.push_back(Edge(i, j, capacity));
	capacitorsCurrent.push_back(0);
	return capacitors.size() - 1;
}

int SchemeCalculator::addCapacitor(Edge capacitor)
{
	if (capacitor.getI() >= n) updateSize(capacitor.getI() + 1);
	if (capacitor.getJ() >= n) updateSize(capacitor.getJ() + 1);
	capacitors.push_back(capacitor);
	capacitorsCurrent.push_back(0);
	return capacitors.size() - 1;
}

void SchemeCalculator::removeCapacitor(int capacitor)
{
	capacitors.erase(std::next(capacitors.begin(), capacitor));
	capacitorsCurrent.erase(std::next(capacitorsCurrent.begin(), capacitor));
}
// end of capacitors block


// dc batteries block
int SchemeCalculator::addDCBattery(int i, int j, double emf)
{
	if (i >= n) updateSize(i + 1);
	if (j >= n) updateSize(j + 1);
	wires[i][j]++;
	wires[j][i]++;
	DCBatteries.push_back(Edge(i, j, emf));
	return DCBatteries.size() - 1;
}

int SchemeCalculator::addDCBattery(Edge battery)
{
	if (battery.getI() >= n) updateSize(battery.getI() + 1);
	if (battery.getJ() >= n) updateSize(battery.getJ() + 1);
	wires[battery.getI()][battery.getJ()]++;
	wires[battery.getJ()][battery.getI()]++;
	DCBatteries.push_back(battery);
	return DCBatteries.size() - 1;
}

void SchemeCalculator::removeDCBattery(int battery_id)
{
	auto battery = std::next(DCBatteries.begin(), battery_id);
	wires[battery->getI()][battery->getJ()]--;
	wires[battery->getJ()][battery->getI()]--;
	DCBatteries.erase(battery);
}
// end of dc batteries block


// ac batteries block
int SchemeCalculator::addACBattery(int i, int j, double emf, double cf, double phi)
{
	if (i >= n) updateSize(i + 1);
	if (j >= n) updateSize(j + 1);
	wires[i][j]++;
	wires[j][i]++;
	ACBatteries.push_back(Edge3(i, j, emf, cf, phi));
	return ACBatteries.size() - 1;
}

int SchemeCalculator::addACBattery(Edge3 battery)
{
	if (battery.getI() >= n) updateSize(battery.getI() + 1);
	if (battery.getJ() >= n) updateSize(battery.getJ() + 1);
	wires[battery.getI()][battery.getJ()]++;
	wires[battery.getJ()][battery.getI()]++;
	ACBatteries.push_back(battery);
	return ACBatteries.size() - 1;
}

void SchemeCalculator::removeACBattery(int battery_id)
{
	auto battery = std::next(ACBatteries.begin(), battery_id);
	wires[battery->getI()][battery->getJ()]--;
	wires[battery->getJ()][battery->getI()]--;
	ACBatteries.erase(battery);
}
// end of ac batteries block


// calculating-related functions block
void SchemeCalculator::recalculate(double time)
{
	clearPhysics();
	std::vector <comp> local_p(n), temp;

	for (auto battery : ACBatteries)
	{
		int ss = battery.getI();
		int tt = battery.getJ();
		wires[ss][tt]--;
		wires[tt][ss]--;
		updateGraph();
		s = order[ss];
		t = order[tt];

		for (auto inductor : inductors) addEdge(inductor.getI(), inductor.getJ(), comp(0, battery.getL2() * inductor.getLength()));
		for (auto capacitor : capacitors) addEdge(capacitor.getI(), capacitor.getJ(), double(-1) / (battery.getL2() * capacitor.getLength()));
		
		temp = getLocalPotentials();
		for (int i = 0; i < n; i++) local_p[i] = temp[order[i]];

		for (int i = 0; i < n; i++) local_p[i] *= comp(battery.getL1(), battery.getL3());
		for (int i = 0; i < n; i++)
		{
			potentials[i] += abs(local_p[i]) * cos(battery.getL2() * time + arg(local_p[i]));
		}
		
		std::list <double>::iterator it;
		it = inductorsCurrent.begin();
		for (auto inductor : inductors)
		{
			comp res = comp(0, battery.getL2() * inductor.getLength());
			addEdge(inductor.getI(), inductor.getJ(), -res);
			comp cur = (local_p[inductor.getJ()] - local_p[inductor.getI()]) / res;
			*it += abs(cur) * cos(battery.getL2() * time + arg(cur));
			it++;
		}
		it = capacitorsCurrent.begin();
		for (auto capacitor : capacitors)
		{
			comp res = comp(0, double(-1) / (battery.getL2() * capacitor.getLength()));
			addEdge(capacitor.getI(), capacitor.getJ(), -res);
			comp cur = (local_p[capacitor.getJ()] - local_p[capacitor.getI()]) / res;
			*it += abs(cur) * cos(battery.getL2() * time + arg(cur));
			it++;
		}

		wires[ss][tt]++;
		wires[tt][ss]++;
	}

	for (auto inductor : inductors)
	{
		wires[inductor.getI()][inductor.getJ()]++;
		wires[inductor.getJ()][inductor.getI()]++;
	}
	for (auto battery : DCBatteries)
	{
		int ss = battery.getI();
		int tt = battery.getJ();
		wires[ss][tt]--;
		wires[tt][ss]--;
		updateGraph();
		s = order[ss];
		t = order[tt];

		temp = getLocalPotentials();
		for (int i = 0; i < n; i++) local_p[i] = temp[order[i]];

		for (int i = 0; i < n; i++) local_p[i] *= battery.getLength();
		for (int i = 0; i < n; i++) potentials[i] += abs(local_p[i]);

		wires[ss][tt]++;
		wires[tt][ss]++;
	}
	for (auto inductor : inductors)
	{
		wires[inductor.getI()][inductor.getJ()]--;
		wires[inductor.getJ()][inductor.getI()]--;
	}
}

void SchemeCalculator::updateGraph()
{
	order.clear();
	order.resize(n, -1);
	int o = 0;
	for (int i = 0; i < n; i++)
	{
		if (order[i] == -1) dfsUpdateGraph(i, o++);
		for (int j = 0; j < n; j++) graph[i][j] = 0;
	}

	graph.clear();
	graph.resize(o, std::vector <comp>(o, 0));

	for (int i = 0; i < n; i++)
	{
		for (int j = 0; j < n; j++)
		{
			graph[order[i]][order[j]] += a[i][j];
		}
	}
}

void SchemeCalculator::dfsUpdateGraph(int v, int o)
{
	order[v] = o;
	for (int i = 0; i < n; i++) if (wires[v][i] > 0 && order[i] == -1) dfsUpdateGraph(i, o);
}

void SchemeCalculator::addEdge(int i, int j, comp resistance)
{
	if (resistance == comp(0) || i == j) return;

	a[i][i] += comp(1) / resistance;
	a[i][j] -= comp(1) / resistance;

	a[j][j] += comp(1) / resistance;
	a[j][i] -= comp(1) / resistance;
}

void SchemeCalculator::clearPhysics()
{
	potentials.clear();
	potentials.resize(n, 0);

	capacitorsCurrent.clear();
	capacitorsCurrent.resize(capacitors.size(), 0);

	inductorsCurrent.clear();
	inductorsCurrent.resize(inductors.size(), 0);
}

std::vector <comp> SchemeCalculator::getLocalPotentials()
{
	int n = graph.size();

	std::vector <std::vector <comp> > sys(n - 2, std::vector <comp>(n - 1, 0));
	for (int i = 0; i < min(s, t); i++)
	{
		for (int j = 0; j < min(s, t); j++) sys[i][j] = graph[i][j];
		for (int j = min(s, t) + 1; j < max(s, t); j++) sys[i][j - 1] = graph[i][j];
		for (int j = max(s, t) + 1; j < n; j++) sys[i][j - 2] = graph[i][j];
		sys[i][n - 2] = -graph[i][s];
	}
	for (int i = min(s, t) + 1; i < max(s, t); i++)
	{
		for (int j = 0; j < min(s, t); j++) sys[i - 1][j] = graph[i][j];
		for (int j = min(s, t) + 1; j < max(s, t); j++) sys[i - 1][j - 1] = graph[i][j];
		for (int j = max(s, t) + 1; j < n; j++) sys[i - 1][j - 2] = graph[i][j];
		sys[i - 1][n - 2] = -graph[i][s];
	}
	for (int i = max(s, t) + 1; i < n; i++)
	{
		for (int j = 0; j < min(s, t); j++) sys[i - 2][j] = graph[i][j];
		for (int j = min(s, t) + 1; j < max(s, t); j++) sys[i - 2][j - 1] = graph[i][j];
		for (int j = max(s, t) + 1; j < n; j++) sys[i - 2][j - 2] = graph[i][j];
		sys[i - 2][n - 2] = -graph[i][s];
	}

	for (int i = 0; i < n - 2; i++)
	{
		int j = i;
		while (j < n - 2 && sys[j][i] == comp(0)) j++;
		if (j == n - 2)
		{
			for (int j = 0; j < n - 2; j++) sys[j][i] = 0;
			continue;
		}

		if (i != j)
		{
			for (int k = 0; k < n - 1; k++)
			{
				comp x = sys[i][k];
				sys[i][k] = sys[j][k];
				sys[j][k] = x;
			}
		}

		for (j = n - 2; j >= i; j--) sys[i][j] /= sys[i][i];
		for (j = 0; j < n - 2; j++)
		{
			if (j == i) continue;
			for (int k = n - 2; k >= i; k--) sys[j][k] -= sys[i][k] * sys[j][i];
		}
	}

	std::vector <comp> p(n);
	p[s] = 1;
	p[t] = 0;
	for (int j = 0; j < min(s, t); j++) p[j] = sys[j][n - 2];
	for (int j = min(s, t) + 1; j < max(s, t); j++) p[j] = sys[j - 1][n - 2];
	for (int j = max(s, t) + 1; j < n; j++) p[j] = sys[j - 2][n - 2];

	std::vector <comp> ans(this->n);
	for (int i = 0; i < this->n; i++) ans[i] = p[order[i]];

	return p;
}
// end of calculating-related functions block


// updating graph size function block
void SchemeCalculator::updateSize(int n)
{
	this->n = n;

	for (int i = 0; i < n; i++)
	{
		if (i == a.size()) a.push_back(std::vector <comp>(n, 0));
		else a[i].resize(n, 0);
	}

	for (int i = 0; i < n; i++)
	{
		if (i == graph.size()) graph.push_back(std::vector <comp>(n, 0));
		else graph[i].resize(n, 0);
	}

	for (int i = 0; i < n; i++)
	{
		if (i == wires.size()) wires.push_back(std::vector <int>(n, 0));
		else wires[i].resize(n, 0);
	}

	potentials.resize(n, 0);
}
// end of updating graph size function block


// getting functions block
std::vector<double> SchemeCalculator::getPotentials()
{
	return potentials;
}

std::vector<double> SchemeCalculator::getInductorsCurrents()
{
	std::vector<double> v;
	v.clear();
	for (auto it = inductorsCurrent.begin(); it != inductorsCurrent.end(); it++) v.push_back(*it);
	return v;
}

std::vector<double> SchemeCalculator::getCapacitorsCurrents()
{
	std::vector<double> v;
	v.clear();
	for (auto it = capacitorsCurrent.begin(); it != capacitorsCurrent.end(); it++) v.push_back(*it);
	return v;
}
// end of getting function block