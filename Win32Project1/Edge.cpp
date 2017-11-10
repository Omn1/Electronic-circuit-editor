#include "Edge.h"

// Edge class functions
Edge::Edge(int i, int j, double length)
	: i(i)
	, j(j)
	, length(length)
{

}

void Edge::setI(int i)
{
	this->i = i;
}

void Edge::setJ(int j)
{
	this->j = j;
}

void Edge::setLength(double length)
{
	this->length = length;
}

const int& Edge::getI() const
{
	return i;
}

const int& Edge::getJ() const
{
	return j;
}

const double& Edge::getLength() const
{
	return length;
}

bool operator < (Edge a, Edge b)
{
	if (a.getLength() != b.getLength()) return a.getLength() < b.getLength();
	if (a.getI() != b.getI()) return a.getI() < b.getI();
	return a.getJ() < b.getJ();
}
// end of Edge class functions


// Edge2 class functions
Edge2::Edge2(int i, int j, double l1, double l2)
	: i(i)
	, j(j)
	, l1(l1)
	, l2(l2)
{

}

void Edge2::setI(int i)
{
	this->i = i;
}

void Edge2::setJ(int j)
{
	this->j = j;
}

void Edge2::setL1(double l1)
{
	this->l1 = l1;
}

void Edge2::setL2(double l2)
{
	this->l2 = l2;
}

const int& Edge2::getI() const
{
	return i;
}

const int& Edge2::getJ() const
{
	return j;
}

const double& Edge2::getL1() const
{
	return l1;
}

const double& Edge2::getL2() const
{
	return l2;
}

bool operator < (Edge2 a, Edge2 b)
{
	if (a.getL1() != b.getL1()) return a.getL1() < b.getL1();
	if (a.getL2() != b.getL2()) return a.getL2() < b.getL2();
	if (a.getI() != b.getI()) return a.getI() < b.getI();
	return a.getJ() < b.getJ();
}
// end of Edge2 class functions


// Edge3 class functions
Edge3::Edge3(int i, int j, double l1, double l2, double l3)
	: i(i)
	, j(j)
	, l1(l1)
	, l2(l2)
	, l3(l3)
{

}

void Edge3::setI(int i)
{
	this->i = i;
}

void Edge3::setJ(int j)
{
	this->j = j;
}

void Edge3::setL1(double l1)
{
	this->l1 = l1;
}

void Edge3::setL2(double l2)
{
	this->l2 = l2;
}

void Edge3::setL3(double l3)
{
	this->l3 = l3;
}

const int& Edge3::getI() const
{
	return i;
}

const int& Edge3::getJ() const
{
	return j;
}

const double& Edge3::getL1() const
{
	return l1;
}

const double& Edge3::getL2() const
{
	return l2;
}

const double& Edge3::getL3() const
{
	return l3;
}

bool operator < (Edge3 a, Edge3 b)
{
	if (a.getL1() != b.getL1()) return a.getL1() < b.getL1();
	if (a.getL2() != b.getL2()) return a.getL2() < b.getL2();
	if (a.getL3() != b.getL3()) return a.getL3() < b.getL3();
	if (a.getI() != b.getI()) return a.getI() < b.getI();
	return a.getJ() < b.getJ();
}
// end of Edge3 class functions