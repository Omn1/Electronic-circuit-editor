#ifndef SCHEME_CALCULATOR_H
#define SCHEME_CALCULATOR_H

#include <vector>
#include <list>
#include <set>
#include <complex>
#include "Edge.h"

typedef std::complex<double> comp;

class SchemeCalculator
{
public:
	// user functions
	// --------------------------
	SchemeCalculator();
	void clear();
	// --------------------------

	// controlling elements set
	// --------------------------
	void addWire(int i, int j);
	void removeWire(int i, int j);

	void addResistance(int i, int j, double resistance);
	void addResistance(Edge resistor);
	void removeResistance(int i, int j, double resistance);
	void removeResistance(Edge resistor);

	int addCapacitor(int i, int j, double capacity);
	int addCapacitor(Edge capacitor);
	void removeCapacitor(int capacitor);

	int addInductor(int i, int j, double inductance);
	int addInductor(Edge inductor);
	void removeInductor(int inductor);

	int addDCBattery(int i, int j, double emf);
	int addDCBattery(Edge battery);
	void removeDCBattery(int battery);

	int addACBattery(int i, int j, double emf, double cf, double time);
	int addACBattery(Edge3 battery);
	void removeACBattery(int battery_id);
	// --------------------------

	// getting functions
	// --------------------------
	void recalculate(double time);
	std::vector<double> getPotentials();
	std::vector<double> getInductorsCurrents();
	std::vector<double> getCapacitorsCurrents();
	std::vector<double> getACBatteriesCurrents();
	std::vector<double> getDCBatteriesCurrents();
	std::vector<bool> getACBatteriesShortCircuits();
	std::vector<bool> getDCBAtteriesShortCircuits();
	double getResistorCurrent(int i, int j, double resistance);
	double getResistorCurrent(Edge resistor);
	// --------------------------

private:
	// calculating functions + variables
	// --------------------------
	void addEdge(int i, int j, comp resistance);
	void clearPhysics();
	std::vector <comp> getLocalPotentials();
	void updateGraph();
	void dfsUpdateGraph(int v, int o);

	std::vector <int> order;
	int s, t;
	// --------------------------

	// geometrical params
	// --------------------------
	std::vector <std::vector <comp> > graph, a;
	std::vector <std::vector <int> > wires;
	// --------------------------

	// physical params
	// --------------------------
	std::vector <double> potentials;
	std::list <Edge> capacitors, inductors, DCBatteries;
	std::list <Edge3> ACBatteries;
	std::vector <double> capacitorsCurrent, inductorsCurrent, DCBatteriesCurrents, ACBatteriesCurrents;
	std::vector <bool> DCShortCircuit, ACShortCircuit;
	// --------------------------

	// graph size functions + variables
	// --------------------------
	int n;
	void updateSize(int n);
	// --------------------------
};

#endif // !SCHEME_CALCULATOR_H

