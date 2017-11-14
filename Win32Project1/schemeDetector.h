#pragma once
#include "detectline.h"
#include "detectschemepoint.h"
#include "detectnumber.h"
#include "detectElement.h"
#include <string>
#include <SFML\Graphics.hpp>

const int maxW = 2000;
const int maxH = 2000;
const int lineSquare = 3;
const int minDist = 10;
const int minLength = 20;
const int connectionTypes = 48;
const int mergingDist = 10;
const int minDistFromSide = 10;
const int minPerimeter = 250;
const int mergeImproveBorders = 50;
const double MM_PI = 3.1415926;
const int cellCo = 3;
const int cellElSize = 5;
const int cellElWidth = 2;
const double contrastDifference = 0.2;

class schemeDetector
{
public:
	schemeDetector();
	std::vector <detectElement> detectEl;
	detectPoint currentEnd;
	std::string detect(std::string filename);
	bool detectIndexesComparator(int a, int b);
	std::vector <std::vector<int> > detectVertexesOnLines;
	std::vector <detectSchemePoint> detectVertexes;
	float detectDistBetweenPoints(detectPoint a, detectPoint b);
	void detectSortPoints(int i, int l, int r);
	~schemeDetector();

private:
	std::string from_number(int a);
	std::string fileName;
	std::string result;
	sf::Texture detectLoad;
	sf::Image detectS;
	int width;
	int height;
	int cellSize;
	sf::Color detectStarterArray[maxW][maxH];
	bool detectBWArray[maxW][maxH];
	int detectHorL[maxW][maxH];
	int detectVerL[maxW][maxH];
	int detectMainDL[maxW][maxH];
	int detectSecDL[maxW][maxH];
	sf::Image detectStarterImage;
	int pixelsAdded;
	std::string detectConnectiontypes[connectionTypes] = {
		"East-heading wire",    //0
		"North-east-heading wire",
		"North-heading wire",
		"North-west-heading wire",
		"West-heading wire",
		"South-west-heading wire",
		"South-heading wire",
		"South-east-heading wire",
		"South part of verical resistor",  //8
		"North part of verical resistor",
		"West part of horisontal resistor",
		"East part of horisontal resistor",
		"South part of verical lamp",  //12
		"North part of verical lamp",
		"West part of horisontal lamp",
		"East part of horisontal lamp",
		"South part of vertical switch-north-west breaker",  //16
		"North part of vertical switch-north-west breaker",
		"West part of horisontal switch-north-east breaker",
		"East part of horisontal switch-north-east breaker",
		"South part of vertical switch-south-east breaker",  //20
		"North part of vertical switch-south-east breaker",
		"West part of horisontal switch-south-west breaker",
		"East part of horisontal switch-south-west breaker",
		"South part of vertical switch-south-west breaker",  //24
		"North part of vertical switch-south-west breaker",
		"West part of horisontal switch-north-west breaker",
		"East part of horisontal switch-north-west breaker",
		"South part of vertical switch-north-east breaker",  //28
		"North part of vertical switch-north-east breaker",
		"West part of horisontal switch-south-east breaker",
		"East part of horisontal switch-south-east breaker",
		"South part of verical amperemeter",  //32
		"North part of verical amperemeter",
		"West part of horisontal amperemeter",
		"East part of horisontal amperemeter",
		"South part of verical voltmeter",  //36
		"North part of verical voltmeter",
		"West part of horisontal voltmeter",
		"East part of horisontal voltmeter",
		"South part of vertical big-part-south battery",  //40
		"North part of vertical big-part-south battery",
		"West part of horisontal big-part-west battery",
		"East part of horisontal big-part-west battery",
		"South part of vertical big-part-north battery",  //44
		"North part of vertical big-part-north battery",
		"West part of horisontal big-part-east battery",
		"East part of horisontal big-part-east battery",
	};

	std::vector <int> detectConverter;
	std::vector <detectSchemePoint> detectFinal;
	std::vector <detectLine> detectHorV;
	std::vector <detectLine> detectVerV;
	std::vector <detectLine> detectMainDV;
	std::vector <detectLine> detectSecDV;
	std::vector <detectLine> allLines;
	detectPoint doNotIntersect;
	std::vector <int> detectDfsVis;
	std::vector <int> detectColorMaxXY;
	std::vector <int> detectColorMinXY;
	std::vector< std::vector <int> > detectVertexesNumbers;
	int detectMax(int a, int b);
	int detectMin(int a, int b);
	std::pair<int, int> detectCellRound(int x, int y, int t);
	bool equalPoints(detectPoint a, detectPoint b);
	bool equalLines(detectLine a, detectLine b);
	float detectGetAzimut(detectPoint from, detectPoint to);
	float ab(float a);
	bool detectIsAcute(detectPoint vertex, detectPoint a, detectPoint b);
	float detectTriangleArea(detectPoint a, detectPoint b, detectPoint c);
	float detectDistPointLine(detectLine l, detectPoint a);
	float detectDistBetweenLines(detectLine l, detectLine m);
	bool detectOrientalTriangleArea(detectPoint a, detectPoint b, detectPoint c);
	bool detectDoLinesIntesect(detectLine l, detectLine m);
	detectPoint detectLineIntersection(detectLine l, detectLine m);
	detectPoint detectGetIntersection(detectLine l, detectLine m);
	bool detectIsOnScreen(detectPoint a);
	bool detectIsOnScheme(detectPoint a);
	float detectBright(sf::Color col);
	bool contrast(float red, float blue, float green, int i, int j);
	void detectFillDynamicArrays();
	void detectGenLineVector();
	void detectDeleteDoubleLines();
	void detectPutVertexesOnLine();
	void detectBuildPointGraph();
	void detectGetGraph();
	int detectFindClosestVertex(int vertexIndex, int dx, int dy, int maxLength);
	bool detectSchemeAnalogical(detectSchemePoint a, detectSchemePoint b);
	bool detectSchemeInLimit(detectSchemePoint a, detectSchemePoint bMin, detectSchemePoint bMax);
	void detectVerticalResistor(int in);
	void detectHorisontalResistor(int in);
	void detectHorisontalLamp(int in);
	void detectVerticalLamp(int in);
	void detectVerticalBattery(int in);
	void detectHorisontalBattery(int in);
	void detectVerticalSwitchNorthWestBreaker(int in);
	void detectVerticalSwitchNorthEastBreaker(int in);
	void detectVerticalSwitchSouthWestBreaker(int in);
	void detectVerticalSwitchSouthEastBreaker(int in);
	void detectHorisontalSwitchNorthWestBreaker(int in);
	void detectHorisontalSwitchSouthWestBreaker(int in);
	void detectHorisontalSwitchNorthEastBreaker(int in);
	void detectHorisontalSwitchSouthEastBreaker(int in);
	void detectElements();
	void detectDfsTrash(int cu, int color);
	void detectAddTrash();
	void detectDeleteTrashVertexes();
	void detectScheme();
	void detectOutScheme();
	void addVerticalWire(detectPoint a);
	void addHorisontalWire(detectPoint a);
	void buildWire(detectPoint a, detectPoint b);
	void addElement(detectPoint a, int j);
	void detectParse(int i, int ji, int k, int jk);
	void fillEl();
	int detectCharToInt(char l);
	int detectGetMeasure(std::string a);
	detectNumber detectTransform(std::string a);
	void detectImproveSides(int i, int k);
	void buildCorners();
	void detectMergeElements(int i, int k);
	void detectTransformElements();
	long double detectPow(long double a, int st);
	std::string detectGiveOut();
	int detectMain();

};


