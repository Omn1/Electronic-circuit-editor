#ifndef EDGE_H
#define EDGE_H

class Edge
{
public:
	Edge(int i = -1, int j = -1, double length = 0);

	void setI(int i);
	void setJ(int j);
	void setLength(double length);

	const int& getI() const;
	const int& getJ() const;
	const double& getLength() const;

private:
	int i, j;
	double length;
};

class Edge2
{
public:
	Edge2(int i = -1, int j = -1, double l1 = 0, double l2 = 0);

	void setI(int i);
	void setJ(int j);
	void setL1(double l1);
	void setL2(double l2);

	const int& getI() const;
	const int& getJ() const;
	const double& getL1() const;
	const double& getL2() const;

private:
	int i, j;
	double l1, l2;
};

class Edge3
{
public:
	Edge3(int i = -1, int j = -1, double l1 = 0, double l2 = 0, double l3 = 0);

	void setI(int i);
	void setJ(int j);
	void setL1(double l1);
	void setL2(double l2);
	void setL3(double l3);

	const int& getI() const;
	const int& getJ() const;
	const double& getL1() const;
	const double& getL2() const;
	const double& getL3() const;

private:
	int i, j;
	double l1, l2, l3;
};

#endif // !EDGE_H
