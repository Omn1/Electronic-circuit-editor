#include "schemeDetector.h"


schemeDetector::schemeDetector()
{
	this->fileName = "";
	this->width = 0;
	this->height = 0;
	this->pixelsAdded = 0;
	this->result = "";
	this->currentEnd = detectPoint();
	this->doNotIntersect = detectPoint(-1, -1);
	this->detectS = sf::Image();
	this->detectStarterImage = sf::Image();
	this->detectLoad = sf::Texture();

}


schemeDetector::~schemeDetector()
{
}

int schemeDetector::detectMax(int a, int b) {
	if (a > b) {
		return a;
	}
	return b;
}

int schemeDetector::detectMin(int a, int b) {
	if (a < b) {
		return a;
	}
	return b;
}

std::pair<int, int> schemeDetector::detectCellRound(int x, int y)
{
	int x1 = x / cellSize;
	int y1 = y / cellSize;
	if (x % cellSize > cellSize / 2) {
		x1 += 1;
	}
	if (y % cellSize > cellSize / 2) {
		y1 += 1;
	}
	std::pair <int, int> ans = std::make_pair(x1, y1);
	return ans;
}

std::string schemeDetector::from_number(int a) {
	if (a == 0) {
		return "0";
	}
	std::string ans = "";
	if (a < 0) {
		a = -a;
		ans = "-" + ans;
	}
	while (a > 0) {
		char ch = 48 + (a % 10);
		ans = ans + ch;
		a /= 10;
	}
	return ans;
}

//---------------------------------------------------------------------geometry


float schemeDetector::detectDistBetweenPoints(detectPoint a, detectPoint b) {
	return sqrt(float(a.x - b.x) * float(a.x - b.x) + float(a.y - b.y) * float(a.y - b.y));
}

bool schemeDetector::equalPoints(detectPoint a, detectPoint b) {
	return (a.x == b.x && a.y == b.y);
}

bool schemeDetector::equalLines(detectLine a, detectLine b) {
	return (a.x1 == b.x1 && a.y1 == b.y1 && a.x2 == b.x2 && a.y2 == b.y2);
}


float schemeDetector::detectGetAzimut(detectPoint from, detectPoint to) {
	float angleSinus = float(from.y - to.y) / detectDistBetweenPoints(from, to);
	if (angleSinus > 1) {
		angleSinus = 0.98;
	}
	if (angleSinus < -1) {
		angleSinus = -0.98;
	}
	float angle = asin(angleSinus);
	if (to.x < from.x) {
		if (angle < 0) {
			angle = -MM_PI - angle;
		}
		else {
			angle = MM_PI - angle;
		}
	}
	return angle;
}

float schemeDetector::ab(float a) {
	if (a < 0) {
		return -a;
	}
	return a;
}

bool schemeDetector::detectIsAcute(detectPoint vertex, detectPoint a, detectPoint b) {
	return ((a.x - vertex.x) * (b.x - vertex.x) + (a.y - vertex.y) * (b.y - vertex.y) > 0);
}

float schemeDetector::detectTriangleArea(detectPoint a, detectPoint b, detectPoint c) {
	int doubledS = (a.x - b.x) * (a.y + b.y) + (b.x - c.x) * (b.y + c.y) + (c.x - a.x) * (c.y + a.y);
	return (ab(float(doubledS)) / 2);
}

float schemeDetector::detectDistPointLine(detectLine l, detectPoint a) {
	detectPoint b1 = detectPoint(l.x1, l.y1);
	detectPoint b2 = detectPoint(l.x2, l.y2);
	if (!detectIsAcute(b1, a, b2)) {
		return (detectDistBetweenPoints(a, b1));
	}
	if (!detectIsAcute(b2, a, b1)) {
		return (detectDistBetweenPoints(a, b2));
	}
	return (2 * detectTriangleArea(a, b1, b2) / detectDistBetweenPoints(b1, b2));
}

float schemeDetector::detectDistBetweenLines(detectLine l, detectLine m) {
	detectPoint a = detectPoint(l.x1, l.y1);
	detectPoint b = detectPoint(l.x2, l.y2);
	detectPoint c = detectPoint(m.x1, m.y1);
	detectPoint d = detectPoint(m.x2, m.y2);
	return detectMin(detectMin(round(detectDistPointLine(m, a)), round(detectDistPointLine(m, b))), detectMin(round(detectDistPointLine(l, c)), round(detectDistPointLine(l, d))));
}

bool schemeDetector::detectOrientalTriangleArea(detectPoint a, detectPoint b, detectPoint c) {
	int doubledS = (a.x - b.x) * (a.y + b.y) + (b.x - c.x) * (b.y + c.y) + (c.x - a.x) * (c.y + a.y);
	return (doubledS > 0);
}

bool schemeDetector::detectDoLinesIntesect(detectLine l, detectLine m) {
	detectPoint a = detectPoint(l.x1, l.y1);
	detectPoint b = detectPoint(l.x2, l.y2);
	detectPoint c = detectPoint(m.x1, m.y1);
	detectPoint d = detectPoint(m.x2, m.y2);
	bool lIntersect = detectOrientalTriangleArea(a, c, d) ^ detectOrientalTriangleArea(b, c, d);
	bool mIntersect = detectOrientalTriangleArea(c, a, b) ^ detectOrientalTriangleArea(d, a, b);
	return (lIntersect && mIntersect);
}

detectPoint schemeDetector::detectLineIntersection(detectLine l, detectLine m) {
	detectPoint a = detectPoint(l.x1, l.y1);
	detectPoint b = detectPoint(l.x2, l.y2);
	detectPoint c = detectPoint(m.x1, m.y1);
	detectPoint d = detectPoint(m.x2, m.y2);
	float lA = b.y - a.y;
	float lB = a.x - b.x;
	float lC = b.x * a.y - a.x * b.y;
	float mA = d.y - c.y;
	float mB = c.x - d.x;
	float mC = d.x * c.y - c.x * d.y;
	int interX = round((mC * lB - lC * mB) / (lA * mB - lB * mA));
	int interY = round((-mC * lA + lC * mA) / (lA * mB - lB * mA));
	return detectPoint(interX, interY);
}

detectPoint schemeDetector::detectGetIntersection(detectLine l, detectLine m) {
	detectPoint a = detectPoint(l.x1, l.y1);
	detectPoint b = detectPoint(l.x2, l.y2);
	detectPoint c = detectPoint(m.x1, m.y1);
	detectPoint d = detectPoint(m.x2, m.y2);
	if (detectDoLinesIntesect(l, m)) {
		detectPoint inter = detectLineIntersection(l, m);
		return inter;
	}
	if (detectDistBetweenPoints(a, c) < mergingDist) {
		return a;
	}
	if (detectDistBetweenPoints(a, d) < mergingDist) {
		return a;
	}
	if (detectDistBetweenPoints(b, c) < mergingDist) {
		return b;
	}
	if (detectDistBetweenPoints(b, d) < mergingDist) {
		return b;
	}
	if (detectDistPointLine(m, a) < mergingDist) {
		return a;
	}
	if (detectDistPointLine(m, b) < mergingDist) {
		return b;
	}
	if (detectDistPointLine(l, c) < mergingDist) {
		return c;
	}
	if (detectDistPointLine(l, d) < mergingDist) {
		return d;
	}
	return doNotIntersect;
}

//---------------------------------------------------------------------cells check

bool schemeDetector::detectIsOnScreen(detectPoint a) {
	if (a.x < 0 || a.y < 0 || a.y >= height || a.x >= width) {
		return false;
	}
	return true;
}

bool schemeDetector::detectIsOnScheme(detectPoint a) {
	if (detectIsOnScreen(a) && detectBWArray[a.x][a.y]) {
		return true;
	}
	return false;
}


//---------------------------------------------------------------------starter image transform

bool schemeDetector::contrast(float red, float blue, float green) {
	if (red + blue + green < 1.5) {
		pixelsAdded++;
	}
	return (red + blue + green > 1.5);
}

//---------------------------------------------------------------------line detection



void schemeDetector::detectFillDynamicArrays() {
	for (int i = 0; i < height; i++) {
		for (int j = 0; j < width; j++) {
			detectHorL[i][j] = 0;
			detectVerL[i][j] = 0;
			detectMainDL[i][j] = 0;
			detectSecDL[i][j] = 0;
		}
	}
	//horisontal
	for (int j = 0; j < width; j++) {
		for (int i = 0; i < height; i++) {
			detectPoint current = detectPoint(j, i);
			if (!detectIsOnScheme(current)) {
				continue;
			}
			int minI = i - lineSquare / 3;
			int maxI = i + lineSquare / 3;
			int maxJ = j - 1;
			int minJ = j - lineSquare;
			for (int nJ = minJ; nJ <= maxJ; nJ++) {
				for (int nI = minI; nI <= maxI; nI++) {
					detectPoint prev = detectPoint(nJ, nI);
					float azimut = detectGetAzimut(current, prev);
					if (!detectIsOnScheme(prev) || azimut < 0.9 * MM_PI || azimut > 1.1 * MM_PI) {
						continue;
					}
					detectHorL[i][j] = detectMax(detectHorL[i][j], detectHorL[nI][nJ] + j - nJ);
				}
			}

		}
	}
	//vertical
	for (int i = 0; i < height; i++) {
		for (int j = 0; j < width; j++) {
			detectPoint current = detectPoint(j, i);
			if (!detectIsOnScheme(current)) {
				continue;
			}
			int minJ = j - lineSquare / 3;
			int maxJ = j + lineSquare / 3;
			int maxI = i - 1;
			int minI = i - lineSquare;
			for (int nJ = minJ; nJ <= maxJ; nJ++) {
				for (int nI = minI; nI <= maxI; nI++) {
					detectPoint prev = detectPoint(nJ, nI);
					float azimut = detectGetAzimut(current, prev);
					if (!detectIsOnScheme(prev) || azimut < 0.4 * MM_PI || azimut > 0.6 * MM_PI) {
						continue;
					}
					detectVerL[i][j] = detectMax(detectVerL[i][j], detectVerL[nI][nJ] + i - nI);
				}
			}
		}
	}
	//main diagonal
	for (int i = 0; i < height; i++) {
		for (int j = 0; j < width; j++) {
			detectPoint current = detectPoint(j, i);
			if (!detectIsOnScheme(current)) {
				continue;
			}
			int maxJ = j - 1;
			int minJ = j - lineSquare / 3;
			int maxI = i - 1;
			int minI = i - lineSquare / 3;
			for (int nJ = minJ; nJ <= maxJ; nJ++) {
				for (int nI = minI; nI <= maxI; nI++) {
					detectPoint prev = detectPoint(nJ, nI);
					float azimut = detectGetAzimut(current, prev);
					if (!detectIsOnScheme(prev) || azimut < 0.6 * MM_PI || azimut > 0.9 * MM_PI) {
						continue;
					}
					detectMainDL[i][j] = detectMax(detectMainDL[i][j], detectMainDL[nI][nJ] + i - nI);
				}
			}
		}
	}

	//second diagonal
	for (int i = height - 1; i >= 0; i--) {
		for (int j = 0; j < width; j++) {
			detectPoint current = detectPoint(j, i);
			if (!detectIsOnScheme(current)) {
				continue;
			}
			int maxJ = j - 1;
			int minJ = j - lineSquare / 3;
			int minI = i + 1;
			int maxI = i + lineSquare / 3;
			for (int nJ = minJ; nJ <= maxJ; nJ++) {
				for (int nI = minI; nI <= maxI; nI++) {
					detectPoint prev = detectPoint(nJ, nI);
					float azimut = detectGetAzimut(current, prev);
					if (!detectIsOnScheme(prev) || azimut < -0.9 * MM_PI || azimut > -0.6 * MM_PI) {
						continue;
					}
					detectSecDL[i][j] = detectMax(detectSecDL[i][j], detectSecDL[nI][nJ] - i + nI);
				}
			}
		}
	}
}

void schemeDetector::detectGenLineVector() {
	for (int i = 0; i < height; i++) {
		for (int j = 0; j < width; j++) {
			detectPoint cu = detectPoint(j, i);
			if (detectHorL[i][j] >= minLength) {
				detectHorV.push_back(detectLine(cu, 1.0 * MM_PI, detectHorL[i][j]));
			}
			if (detectVerL[i][j] >= minLength) {
				detectVerV.push_back(detectLine(cu, 0.5 * MM_PI, detectVerL[i][j]));
			}
			if (detectMainDL[i][j] >= minLength) {
				detectMainDV.push_back(detectLine(cu, 0.75 * MM_PI, 1.4142 * float(detectMainDL[i][j])));
			}
			if (detectSecDL[i][j] >= minLength) {
				detectSecDV.push_back(detectLine(cu, 1.25 * MM_PI, 1.4142 * float(detectSecDL[i][j])));
			}
		}
	}
}

void schemeDetector::detectDeleteDoubleLines() {
	detectLine trash = detectLine(-1, -1, -1, -1);
	for (int i = 0; i < detectHorV.size(); i++) {
		if (equalLines(detectHorV[i], trash)) {
			continue;
		}
		for (int j = i + 1; j < detectHorV.size(); j++) {
			if (equalLines(detectHorV[j], trash)) {
				continue;
			}
			if (detectDistBetweenLines(detectHorV[i], detectHorV[j]) < minDist) {
				if (detectHorV[i].length() < detectHorV[j].length()) {
					detectHorV[i] = trash;
				}
				else {
					detectHorV[j] = trash;
				}
			}
		}
	}
	for (int i = 0; i < detectVerV.size(); i++) {
		if (equalLines(detectVerV[i], trash)) {
			continue;
		}
		for (int j = i + 1; j < detectVerV.size(); j++) {
			if (equalLines(detectVerV[j], trash)) {
				continue;
			}
			if (detectDistBetweenLines(detectVerV[i], detectVerV[j]) < minDist) {
				if (detectVerV[i].length() < detectVerV[j].length()) {
					detectVerV[i] = trash;
				}
				else {
					detectVerV[j] = trash;
				}
			}
		}
	}
	for (int i = 0; i < detectMainDV.size(); i++) {
		if (equalLines(detectMainDV[i], trash)) {
			continue;
		}
		for (int j = i + 1; j < detectMainDV.size(); j++) {
			if (equalLines(detectMainDV[j], trash)) {
				continue;
			}
			if (detectDistBetweenLines(detectMainDV[i], detectMainDV[j]) < minDist) {
				if (detectMainDV[i].length() < detectMainDV[j].length()) {
					detectMainDV[i] = trash;
				}
				else {
					detectMainDV[j] = trash;
				}
			}
		}
	}
	for (int i = 0; i < detectSecDV.size(); i++) {
		if (equalLines(detectSecDV[i], trash)) {
			continue;
		}
		for (int j = i + 1; j < detectSecDV.size(); j++) {
			if (equalLines(detectSecDV[j], trash)) {
				continue;
			}
			if (detectDistBetweenLines(detectSecDV[i], detectSecDV[j]) < minDist) {
				if (detectSecDV[i].length() < detectSecDV[j].length()) {
					detectSecDV[i] = trash;
				}
				else {
					detectSecDV[j] = trash;
				}
			}
		}
	}
	for (int i = 0; i < detectHorV.size(); i++) {
		if (!(equalLines(detectHorV[i], trash))) {
			allLines.push_back(detectHorV[i]);
		}
	}
	for (int i = 0; i < detectVerV.size(); i++) {
		if (!(equalLines(detectVerV[i], trash))) {
			allLines.push_back(detectVerV[i]);
		}
	}
	for (int i = 0; i < detectMainDV.size(); i++) {
		if (!(equalLines(detectMainDV[i], trash))) {
			allLines.push_back(detectMainDV[i]);
		}
	}
	for (int i = 0; i < detectSecDV.size(); i++) {
		if (!(equalLines(detectSecDV[i], trash))) {
			allLines.push_back(detectSecDV[i]);
		}
	}
}

bool schemeDetector::detectIndexesComparator(int a, int b) {
	detectPoint aP = detectPoint(detectVertexes[a].x, detectVertexes[a].y);
	detectPoint bP = detectPoint(detectVertexes[b].x, detectVertexes[b].y);
	return (detectDistBetweenPoints(currentEnd, aP) < detectDistBetweenPoints(currentEnd, bP));
}

void schemeDetector::detectSortPoints(int i, int l, int r) {
	for (int j = 0; j < r; j++) {
		for (int k = l; k < r - 1; k++) {
			if (!detectIndexesComparator(detectVertexesOnLines[i][k], detectVertexesOnLines[i][k + 1])) {
				std::swap(detectVertexesOnLines[i][k], detectVertexesOnLines[i][k + 1]);
			}
		}
	}
}

void schemeDetector::detectPutVertexesOnLine() {
	std::vector <int> bas = {};
	for (int i = 0; i < allLines.size(); i++) {
		detectVertexesOnLines.push_back(bas);
	}
	for (int i = 0; i < allLines.size(); i++) {
		for (int j = i + 1; j < allLines.size(); j++) {
			detectPoint inter = detectGetIntersection(allLines[i], allLines[j]);
			if (!(equalPoints(inter, doNotIntersect))) {
				detectVertexes.push_back(detectSchemePoint(inter.x, inter.y));
				detectVertexesOnLines[i].push_back(detectVertexes.size() - 1);
				detectVertexesOnLines[j].push_back(detectVertexes.size() - 1);
			}
		}
	}
	for (int i = 0; i < allLines.size(); i++) {
		currentEnd = detectPoint(allLines[i].x2, allLines[i].y2);
		detectPoint currentBegin = detectPoint(allLines[i].x1, allLines[i].y1);
		detectSortPoints(i, 0, detectVertexesOnLines[i].size());
		//qSort(detectVertexesOnLines[i].begin(), detectVertexesOnLines[i].end(), detectIndexesComparator);
		if (detectVertexesOnLines[i].size() > 0) {
			int arSize = detectVertexesOnLines[i].size();
			detectPoint arBegin = detectPoint(detectVertexes[detectVertexesOnLines[i][0]].x, detectVertexes[detectVertexesOnLines[i][0]].y);
			detectPoint arEnd = detectPoint(detectVertexes[detectVertexesOnLines[i][arSize - 1]].x, detectVertexes[detectVertexesOnLines[i][arSize - 1]].y);
			if (detectDistBetweenPoints(arBegin, currentEnd) > minDistFromSide) {
				detectVertexes.push_back(detectSchemePoint(currentEnd.x, currentEnd.y));
				detectVertexesOnLines[i].push_back(detectVertexes.size() - 1);
			}
			if (detectDistBetweenPoints(arEnd, currentBegin) > minDistFromSide) {
				detectVertexes.push_back(detectSchemePoint(currentBegin.x, currentBegin.y));
				detectVertexesOnLines[i].push_back(detectVertexes.size() - 1);
			}
			detectSortPoints(i, 0, detectVertexesOnLines[i].size());
			//qSort(detectVertexesOnLines[i].begin(), detectVertexesOnLines[i].end(), detectIndexesComparator);
		}
		else {
			detectVertexes.push_back(detectSchemePoint(currentEnd.x, currentEnd.y));
			detectVertexesOnLines[i].push_back(detectVertexes.size() - 1);
			detectVertexes.push_back(detectSchemePoint(currentBegin.x, currentBegin.y));
			detectVertexesOnLines[i].push_back(detectVertexes.size() - 1);
			detectSortPoints(i, 0, detectVertexesOnLines[i].size());
			//qSort(detectVertexesOnLines[i].begin(), detectVertexesOnLines[i].end(), detectIndexesComparator);
		}
	}
}

void schemeDetector::detectBuildPointGraph() {
	for (int i = 0; i < allLines.size(); i++) {
		detectPoint lineStart = detectPoint(allLines[i].x1, allLines[i].y1);
		detectPoint lineFinish = detectPoint(allLines[i].x2, allLines[i].y2);
		for (int j = 0; j < detectVertexesOnLines[i].size() - 1; j++) {
			int firstIn = detectVertexesOnLines[i][j];
			int secondIn = detectVertexesOnLines[i][j + 1];
			float currentAzimut = detectGetAzimut(lineFinish, lineStart) + 2 * MM_PI;
			int roundedAzimut = round(currentAzimut * 4 / MM_PI);
			roundedAzimut %= 8;
			roundedAzimut = (14 - roundedAzimut) % 8;
			detectVertexes[firstIn].neighbours[roundedAzimut] = secondIn;
			detectVertexes[secondIn].neighbours[(roundedAzimut + 4) % 8] = firstIn;
		}
	}
}

void schemeDetector::detectGetGraph() {
	detectFillDynamicArrays();
	detectGenLineVector();
	detectDeleteDoubleLines();
	detectPutVertexesOnLine();
	detectBuildPointGraph();
}

//--------------------------------------------- object detection

int schemeDetector::detectFindClosestVertex(int vertexIndex, int dx, int dy, int maxLength) {
	int res = -1;
	float minDist = 100000;
	int x = detectVertexes[vertexIndex].x + dx;
	int y = detectVertexes[vertexIndex].y + dy;
	detectPoint ve = detectPoint(x, y);
	for (int i = 0; i < detectVertexes.size(); i++) {
		if (i == vertexIndex) {
			continue;
		}
		detectSchemePoint cu = detectVertexes[i];
		detectPoint ne = detectPoint(cu.x, cu.y);
		float dist = detectDistBetweenPoints(ve, ne);
		if (dist < maxLength && dist < minDist) {
			res = i;
			minDist = dist;
		}
	}
	return res;
}

bool schemeDetector::detectSchemeAnalogical(detectSchemePoint a, detectSchemePoint b) {
	for (int i = 0; i < connectionTypes; i++) {
		if (((a.neighbours[i] == -1) ^ (b.neighbours[i] == -1))) {
			return false;
		}
	}
	return true;
}

bool schemeDetector::detectSchemeInLimit(detectSchemePoint a, detectSchemePoint bMin, detectSchemePoint bMax) {
	for (int i = 0; i < connectionTypes; i++) {
		if ((!((bMin.neighbours[i] == -1) ^ (bMax.neighbours[i] == -1)) && ((a.neighbours[i] == -1) ^ (bMin.neighbours[i] == -1)))) {
			return false;
		}
	}
	return true;
}

//------------------------------------------------------- resistors

void schemeDetector::detectVerticalResistor(int in) {
	detectSchemePoint stI = detectVertexes[in];
	detectSchemePoint ex = detectSchemePoint(0, 0);
	ex.neighbours[0] = 0;
	ex.neighbours[2] = 0;
	ex.neighbours[4] = 0;
	if (!detectSchemeAnalogical(ex, stI)) {
		return;
	}
	detectSchemePoint ru = detectVertexes[stI.neighbours[0]];
	detectSchemePoint lu = detectVertexes[stI.neighbours[4]];
	ex.clear();
	ex.neighbours[4] = 0;
	ex.neighbours[6] = 0;
	if (!detectSchemeAnalogical(ex, ru)) {
		return;
	}
	detectSchemePoint rd = detectVertexes[ru.neighbours[6]];
	ex.clear();
	ex.neighbours[0] = 0;
	ex.neighbours[6] = 0;
	if (!detectSchemeAnalogical(ex, lu)) {
		return;
	}
	detectSchemePoint ld = detectVertexes[lu.neighbours[6]];
	ex.clear();
	ex.neighbours[2] = 0;
	ex.neighbours[4] = 0;
	if (!detectSchemeAnalogical(ex, rd)) {
		return;
	}
	int fiI = rd.neighbours[4];
	ex.clear();
	ex.neighbours[0] = 0;
	ex.neighbours[2] = 0;
	if (!detectSchemeAnalogical(ex, ld)) {
		return;
	}
	int nfiI = ld.neighbours[0];
	if (nfiI != fiI) {
		return;
	}
	ex.clear();
	ex.neighbours[0] = 0;
	ex.neighbours[4] = 0;
	ex.neighbours[6] = 0;
	if (!detectSchemeAnalogical(ex, detectVertexes[fiI])) {
		return;
	}
	detectSchemePoint en = detectVertexes[fiI];

	detectVertexes[stI.neighbours[0]].isNotPartOfElement = false;
	detectVertexes[stI.neighbours[4]].isNotPartOfElement = false;
	detectVertexes[en.neighbours[0]].isNotPartOfElement = false;
	detectVertexes[en.neighbours[4]].isNotPartOfElement = false;

	detectVertexes[in].neighbours[0] = -1;
	detectVertexes[in].neighbours[4] = -1;
	detectVertexes[fiI].neighbours[0] = -1;
	detectVertexes[fiI].neighbours[4] = -1;
	detectVertexes[fiI].neighbours[8] = in;
	detectVertexes[in].neighbours[9] = fiI;
}

void schemeDetector::detectHorisontalResistor(int in) {
	//qDebug("null!");
	detectSchemePoint stI = detectVertexes[in];
	detectSchemePoint ex = detectSchemePoint(0, 0);
	ex.neighbours[2] = 0;
	ex.neighbours[4] = 0;
	ex.neighbours[6] = 0;
	if (!detectSchemeAnalogical(ex, stI)) {
		return;
	}
	//qDebug("first!");
	detectSchemePoint lu = detectVertexes[stI.neighbours[2]];
	detectSchemePoint ld = detectVertexes[stI.neighbours[6]];
	ex.clear();
	ex.neighbours[0] = 0;
	ex.neighbours[6] = 0;
	if (!detectSchemeAnalogical(ex, lu)) {
		return;
	}
	// qDebug("second!");
	detectSchemePoint ru = detectVertexes[lu.neighbours[0]];
	ex.clear();
	ex.neighbours[0] = 0;
	ex.neighbours[2] = 0;
	if (!detectSchemeAnalogical(ex, ld)) {
		return;
	}
	// qDebug("third!");
	detectSchemePoint rd = detectVertexes[ld.neighbours[0]];
	ex.clear();
	ex.neighbours[4] = 0;
	ex.neighbours[6] = 0;
	if (!detectSchemeAnalogical(ex, ru)) {
		return;
	}
	// qDebug("fourth!");
	int fiI = ru.neighbours[6];
	ex.clear();
	ex.neighbours[2] = 0;
	ex.neighbours[4] = 0;
	if (!detectSchemeAnalogical(ex, rd)) {
		return;
	}
	int nfiI = rd.neighbours[2];
	if (nfiI != fiI) {
		return;
	}
	ex.clear();
	ex.neighbours[0] = 0;
	ex.neighbours[2] = 0;
	ex.neighbours[6] = 0;
	if (!detectSchemeAnalogical(ex, detectVertexes[fiI])) {
		return;
	}

	detectSchemePoint en = detectVertexes[fiI];

	detectVertexes[stI.neighbours[2]].isNotPartOfElement = false;
	detectVertexes[stI.neighbours[6]].isNotPartOfElement = false;
	detectVertexes[en.neighbours[2]].isNotPartOfElement = false;
	detectVertexes[en.neighbours[6]].isNotPartOfElement = false;

	detectVertexes[in].neighbours[2] = -1;
	detectVertexes[in].neighbours[6] = -1;
	detectVertexes[fiI].neighbours[2] = -1;
	detectVertexes[fiI].neighbours[6] = -1;
	detectVertexes[fiI].neighbours[11] = in;
	detectVertexes[in].neighbours[10] = fiI;
}

//---------------------------------------------------- lamps

void schemeDetector::detectHorisontalLamp(int in) {
	detectSchemePoint allCon;
	allCon.neighbours[0] = 0;
	allCon.neighbours[1] = 0;
	allCon.neighbours[2] = 0;
	allCon.neighbours[3] = 0;
	allCon.neighbours[4] = 0;
	allCon.neighbours[5] = 0;
	allCon.neighbours[6] = 0;
	allCon.neighbours[7] = 0;
	detectSchemePoint stI = detectVertexes[in];
	detectSchemePoint ex = detectSchemePoint(0, 0);
	ex.neighbours[1] = 0;
	ex.neighbours[3] = 0;
	ex.neighbours[5] = 0;
	ex.neighbours[7] = 0;
	if (!detectSchemeAnalogical(ex, stI)) {
		return;
	}
	detectSchemePoint ru = detectVertexes[stI.neighbours[1]];
	detectSchemePoint lu = detectVertexes[stI.neighbours[3]];
	detectSchemePoint ld = detectVertexes[stI.neighbours[5]];
	detectSchemePoint rd = detectVertexes[stI.neighbours[7]];
	if ((2 * (ld.x - stI.x) > 5 * (stI.x - ru.x)) || (5 * (ld.x - stI.x) < 2 * (stI.x - ru.x))) {
		return;
	}
	if ((2 * (rd.x - stI.x) > 5 * (stI.x - lu.x)) || (5 * (rd.x - stI.x) < 2 * (stI.x - lu.x))) {
		return;
	}
	int dy = (stI.y - lu.y) * 3 / 2;
	int start = detectFindClosestVertex(in, 0, -dy, dy / 3);
	int finish = detectFindClosestVertex(in, 0, dy, dy / 3);
	if (start == -1 || finish == -1) {
		return;
	}
	ex.clear();
	ex.neighbours[4] = 0;
	if (!detectSchemeInLimit(detectVertexes[start], ex, allCon)) {
		return;
	}
	ex.clear();
	ex.neighbours[0] = 0;
	if (!detectSchemeInLimit(detectVertexes[finish], ex, allCon)) {
		return;
	}
	for (int i = 0; i < 8; i++) {
		if (i != 0 && detectVertexes[finish].neighbours[i] >= 0) {
			detectVertexes[detectVertexes[finish].neighbours[i]].isNotPartOfElement = false;
		}
		if (i != 4 && detectVertexes[start].neighbours[i] >= 0) {
			detectVertexes[detectVertexes[start].neighbours[i]].isNotPartOfElement = false;
		}
	}
	detectVertexes[stI.neighbours[1]].isNotPartOfElement = false;
	detectVertexes[stI.neighbours[3]].isNotPartOfElement = false;
	detectVertexes[stI.neighbours[5]].isNotPartOfElement = false;
	detectVertexes[stI.neighbours[7]].isNotPartOfElement = false;
	detectVertexes[in].isNotPartOfElement = false;

	detectVertexes[start].neighbours[14] = finish;
	detectVertexes[finish].neighbours[15] = start;
}

void schemeDetector::detectVerticalLamp(int in) {
	detectSchemePoint allCon;
	allCon.neighbours[0] = 0;
	allCon.neighbours[1] = 0;
	allCon.neighbours[2] = 0;
	allCon.neighbours[3] = 0;
	allCon.neighbours[4] = 0;
	allCon.neighbours[5] = 0;
	allCon.neighbours[6] = 0;
	allCon.neighbours[7] = 0;
	detectSchemePoint stI = detectVertexes[in];
	detectSchemePoint ex = detectSchemePoint(0, 0);
	ex.neighbours[1] = 0;
	ex.neighbours[3] = 0;
	ex.neighbours[5] = 0;
	ex.neighbours[7] = 0;
	if (!detectSchemeAnalogical(ex, stI)) {
		return;
	}
	detectSchemePoint ru = detectVertexes[stI.neighbours[1]];
	detectSchemePoint lu = detectVertexes[stI.neighbours[3]];
	detectSchemePoint ld = detectVertexes[stI.neighbours[5]];
	detectSchemePoint rd = detectVertexes[stI.neighbours[7]];
	if ((2 * (ld.x - stI.x) > 5 * (stI.x - ru.x)) || (5 * (ld.x - stI.x) < 2 * (stI.x - ru.x))) {
		return;
	}
	if ((2 * (rd.x - stI.x) > 5 * (stI.x - lu.x)) || (5 * (rd.x - stI.x) < 2 * (stI.x - lu.x))) {
		return;
	}
	int dy = (stI.y - lu.y) * 3 / 2;
	int start = detectFindClosestVertex(in, -dy, 0, dy / 3);
	int finish = detectFindClosestVertex(in, dy, 0, dy / 3);
	if (start == -1 || finish == -1) {
		return;
	}
	ex.clear();
	ex.neighbours[2] = 0;
	if (!detectSchemeInLimit(detectVertexes[start], ex, allCon)) {
		return;
	}
	ex.clear();
	ex.neighbours[6] = 0;
	if (!detectSchemeInLimit(detectVertexes[finish], ex, allCon)) {
		return;
	}
	for (int i = 0; i < 8; i++) {
		if (i != 6 && detectVertexes[finish].neighbours[i] >= 0) {
			detectVertexes[detectVertexes[finish].neighbours[i]].isNotPartOfElement = false;
		}
		if (i != 2 && detectVertexes[start].neighbours[i] >= 0) {
			detectVertexes[detectVertexes[start].neighbours[i]].isNotPartOfElement = false;
		}
	}
	detectVertexes[stI.neighbours[1]].isNotPartOfElement = false;
	detectVertexes[stI.neighbours[3]].isNotPartOfElement = false;
	detectVertexes[stI.neighbours[5]].isNotPartOfElement = false;
	detectVertexes[stI.neighbours[7]].isNotPartOfElement = false;
	detectVertexes[in].isNotPartOfElement = false;

	detectVertexes[start].neighbours[13] = finish;
	detectVertexes[finish].neighbours[12] = start;
}

//------------------------------------------------- batteries

void schemeDetector::detectVerticalBattery(int in) {
	detectSchemePoint stI = detectVertexes[in];
	detectSchemePoint ex = detectSchemePoint(0, 0);
	ex.neighbours[0] = 0;
	ex.neighbours[2] = 0;
	ex.neighbours[4] = 0;
	if (!detectSchemeAnalogical(ex, stI)) {
		return;
	}
	detectSchemePoint ru = detectVertexes[stI.neighbours[0]];
	detectSchemePoint lu = detectVertexes[stI.neighbours[4]];
	ex.clear();
	ex.neighbours[4] = 0;
	if (!detectSchemeAnalogical(ex, ru)) {
		return;
	}
	ex.clear();
	ex.neighbours[0] = 0;
	if (!detectSchemeAnalogical(ex, lu)) {
		return;
	}
	int dx = stI.y - lu.y;
	int finish = detectFindClosestVertex(in, dx, 0, dx / 2);
	if (finish == -1) {
		return;
	}
	detectSchemePoint en = detectVertexes[finish];
	ex.clear();
	ex.neighbours[0] = 0;
	ex.neighbours[4] = 0;
	ex.neighbours[6] = 0;
	if (!detectSchemeAnalogical(ex, en)) {
		return;
	}
	detectSchemePoint ld = detectVertexes[en.neighbours[4]];
	detectSchemePoint rd = detectVertexes[en.neighbours[0]];

	ex.clear();
	ex.neighbours[0] = 0;
	if (!detectSchemeAnalogical(ex, ld)) {
		return;
	}
	ex.clear();
	ex.neighbours[4] = 0;
	if (!detectSchemeAnalogical(ex, rd)) {
		return;
	}

	detectVertexes[stI.neighbours[0]].isNotPartOfElement = false;
	detectVertexes[stI.neighbours[4]].isNotPartOfElement = false;
	detectVertexes[en.neighbours[0]].isNotPartOfElement = false;
	detectVertexes[en.neighbours[4]].isNotPartOfElement = false;

	detectVertexes[in].neighbours[0] = -1;
	detectVertexes[in].neighbours[4] = -1;
	detectVertexes[finish].neighbours[0] = -1;
	detectVertexes[finish].neighbours[4] = -1;
	int dxS = en.y - ld.y;
	if (dxS > dx) {
		detectVertexes[finish].neighbours[40] = in;
		detectVertexes[in].neighbours[41] = finish;
	}
	else {
		detectVertexes[finish].neighbours[44] = in;
		detectVertexes[in].neighbours[45] = finish;
	}
}

void schemeDetector::detectHorisontalBattery(int in) {
	detectSchemePoint stI = detectVertexes[in];
	detectSchemePoint ex = detectSchemePoint(0, 0);
	ex.neighbours[2] = 0;
	ex.neighbours[4] = 0;
	ex.neighbours[6] = 0;
	if (!detectSchemeAnalogical(ex, stI)) {
		return;
	}
	detectSchemePoint lu = detectVertexes[stI.neighbours[2]];
	detectSchemePoint ld = detectVertexes[stI.neighbours[6]];
	ex.clear();
	ex.neighbours[6] = 0;
	if (!detectSchemeAnalogical(ex, lu)) {
		return;
	}
	ex.clear();
	ex.neighbours[2] = 0;
	if (!detectSchemeAnalogical(ex, ld)) {
		return;
	}
	int dx = stI.x - lu.x;
	int finish = detectFindClosestVertex(in, 0, dx, dx / 2);
	if (finish == -1) {
		return;
	}
	detectSchemePoint en = detectVertexes[finish];
	ex.clear();
	ex.neighbours[0] = 0;
	ex.neighbours[2] = 0;
	ex.neighbours[6] = 0;
	if (!detectSchemeAnalogical(ex, en)) {
		return;
	}
	detectSchemePoint ru = detectVertexes[en.neighbours[2]];
	detectSchemePoint rd = detectVertexes[en.neighbours[6]];

	ex.clear();
	ex.neighbours[6] = 0;
	if (!detectSchemeAnalogical(ex, ru)) {
		return;
	}
	ex.clear();
	ex.neighbours[2] = 0;
	if (!detectSchemeAnalogical(ex, rd)) {
		return;
	}

	detectVertexes[stI.neighbours[2]].isNotPartOfElement = false;
	detectVertexes[stI.neighbours[6]].isNotPartOfElement = false;
	detectVertexes[en.neighbours[2]].isNotPartOfElement = false;
	detectVertexes[en.neighbours[6]].isNotPartOfElement = false;

	detectVertexes[in].neighbours[2] = -1;
	detectVertexes[in].neighbours[6] = -1;
	detectVertexes[finish].neighbours[2] = -1;
	detectVertexes[finish].neighbours[6] = -1;
	int dxS = en.x - ru.x;
	if (dxS > dx) {
		detectVertexes[finish].neighbours[47] = in;
		detectVertexes[in].neighbours[46] = finish;
	}
	else {
		detectVertexes[finish].neighbours[43] = in;
		detectVertexes[in].neighbours[42] = finish;
	}
}

//------------------------------------------------------  switches

void schemeDetector::detectVerticalSwitchNorthWestBreaker(int in) {
	detectSchemePoint ex;
	ex.clear();
	detectSchemePoint start = detectVertexes[in];
	ex.neighbours[3] = 0;
	ex.neighbours[6] = 0;
	if (!detectSchemeAnalogical(start, ex)) {
		return;
	}
	detectSchemePoint swit = detectVertexes[start.neighbours[3]];
	ex.clear();
	ex.neighbours[7] = 0;
	if (!detectSchemeAnalogical(swit, ex)) {
		return;
	}
	int dx = start.x - swit.x;
	int fin = detectFindClosestVertex(in, -dx, 0, dx / 2);
	if (fin == -1) {
		return;
	}
	detectSchemePoint finish = detectVertexes[fin];
	ex.clear();
	ex.neighbours[2] = 0;
	if (!detectSchemeAnalogical(finish, ex)) {
		return;
	}
	detectVertexes[start.neighbours[3]].isNotPartOfElement = false;
	detectVertexes[in].neighbours[3] = -1;

	detectVertexes[in].neighbours[16] = fin;
	detectVertexes[fin].neighbours[17] = in;
}

void schemeDetector::detectVerticalSwitchNorthEastBreaker(int in) {
	detectSchemePoint ex;
	ex.clear();
	detectSchemePoint start = detectVertexes[in];
	ex.neighbours[1] = 0;
	ex.neighbours[6] = 0;
	if (!detectSchemeAnalogical(start, ex)) {
		return;
	}
	detectSchemePoint swit = detectVertexes[start.neighbours[3]];
	ex.clear();
	ex.neighbours[5] = 0;
	if (!detectSchemeAnalogical(swit, ex)) {
		return;
	}
	int dx = start.x - swit.x;
	int fin = detectFindClosestVertex(in, -dx, 0, dx / 2);
	if (fin == -1) {
		return;
	}
	detectSchemePoint finish = detectVertexes[fin];
	ex.clear();
	ex.neighbours[2] = 0;
	if (!detectSchemeAnalogical(finish, ex)) {
		return;
	}
	detectVertexes[start.neighbours[1]].isNotPartOfElement = false;
	detectVertexes[in].neighbours[1] = -1;

	detectVertexes[in].neighbours[28] = fin;
	detectVertexes[fin].neighbours[29] = in;
}

void schemeDetector::detectVerticalSwitchSouthWestBreaker(int in) {
	detectSchemePoint ex;
	ex.clear();
	detectSchemePoint start = detectVertexes[in];
	ex.neighbours[2] = 0;
	ex.neighbours[5] = 0;
	if (!detectSchemeAnalogical(start, ex)) {
		return;
	}
	detectSchemePoint swit = detectVertexes[start.neighbours[5]];
	ex.clear();
	ex.neighbours[1] = 0;
	if (!detectSchemeAnalogical(swit, ex)) {
		return;
	}
	int dx = start.x - swit.x;
	int fin = detectFindClosestVertex(in, -dx, 0, -dx / 2);
	if (fin == -1) {
		return;
	}
	detectSchemePoint finish = detectVertexes[fin];
	ex.clear();
	ex.neighbours[6] = 0;
	if (!detectSchemeAnalogical(finish, ex)) {
		return;
	}
	detectVertexes[start.neighbours[5]].isNotPartOfElement = false;
	detectVertexes[in].neighbours[5] = -1;

	detectVertexes[in].neighbours[25] = fin;
	detectVertexes[fin].neighbours[24] = in;
}

void schemeDetector::detectVerticalSwitchSouthEastBreaker(int in) {
	detectSchemePoint ex;
	ex.clear();
	detectSchemePoint start = detectVertexes[in];
	ex.neighbours[2] = 0;
	ex.neighbours[7] = 0;
	if (!detectSchemeAnalogical(start, ex)) {
		return;
	}
	detectSchemePoint swit = detectVertexes[start.neighbours[7]];
	ex.clear();
	ex.neighbours[3] = 0;
	if (!detectSchemeAnalogical(swit, ex)) {
		return;
	}
	int dx = start.x - swit.x;
	int fin = detectFindClosestVertex(in, -dx, 0, -dx / 2);
	if (fin == -1) {
		return;
	}
	detectSchemePoint finish = detectVertexes[fin];
	ex.clear();
	ex.neighbours[6] = 0;
	if (!detectSchemeAnalogical(finish, ex)) {
		return;
	}
	detectVertexes[start.neighbours[7]].isNotPartOfElement = false;
	detectVertexes[in].neighbours[7] = -1;

	detectVertexes[in].neighbours[21] = fin;
	detectVertexes[fin].neighbours[20] = in;
}

void schemeDetector::detectHorisontalSwitchNorthWestBreaker(int in) {
	detectSchemePoint ex;
	ex.clear();
	detectSchemePoint start = detectVertexes[in];
	ex.neighbours[0] = 0;
	ex.neighbours[3] = 0;
	if (!detectSchemeAnalogical(start, ex)) {
		return;
	}
	detectSchemePoint swit = detectVertexes[start.neighbours[3]];
	ex.clear();
	ex.neighbours[7] = 0;
	if (!detectSchemeAnalogical(swit, ex)) {
		return;
	}
	int dy = start.y - swit.y;
	int fin = detectFindClosestVertex(in, 0, -dy, dy / 2);
	if (fin == -1) {
		return;
	}
	detectSchemePoint finish = detectVertexes[fin];
	ex.clear();
	ex.neighbours[4] = 0;
	if (!detectSchemeAnalogical(finish, ex)) {
		return;
	}
	detectVertexes[start.neighbours[3]].isNotPartOfElement = false;
	detectVertexes[in].neighbours[3] = -1;

	detectVertexes[in].neighbours[27] = fin;
	detectVertexes[fin].neighbours[26] = in;
}

void schemeDetector::detectHorisontalSwitchSouthWestBreaker(int in) {
	detectSchemePoint ex;
	ex.clear();
	detectSchemePoint start = detectVertexes[in];
	ex.neighbours[0] = 0;
	ex.neighbours[5] = 0;
	if (!detectSchemeAnalogical(start, ex)) {
		return;
	}
	detectSchemePoint swit = detectVertexes[start.neighbours[5]];
	ex.clear();
	ex.neighbours[1] = 0;
	if (!detectSchemeAnalogical(swit, ex)) {
		return;
	}
	int dy = start.y - swit.y;
	int fin = detectFindClosestVertex(in, 0, -dy, dy / 2);
	if (fin == -1) {
		return;
	}
	detectSchemePoint finish = detectVertexes[fin];
	ex.clear();
	ex.neighbours[4] = 0;
	if (!detectSchemeAnalogical(finish, ex)) {
		return;
	}
	detectVertexes[start.neighbours[5]].isNotPartOfElement = false;
	detectVertexes[in].neighbours[5] = -1;

	detectVertexes[in].neighbours[23] = fin;
	detectVertexes[fin].neighbours[22] = in;
}

void schemeDetector::detectHorisontalSwitchNorthEastBreaker(int in) {
	detectSchemePoint ex;
	ex.clear();
	detectSchemePoint start = detectVertexes[in];
	ex.neighbours[1] = 0;
	ex.neighbours[4] = 0;
	if (!detectSchemeAnalogical(start, ex)) {
		return;
	}
	detectSchemePoint swit = detectVertexes[start.neighbours[1]];
	ex.clear();
	ex.neighbours[5] = 0;
	if (!detectSchemeAnalogical(swit, ex)) {
		return;
	}
	int dy = start.y - swit.y;
	int fin = detectFindClosestVertex(in, 0, -dy, -dy / 2);
	if (fin == -1) {
		return;
	}
	detectSchemePoint finish = detectVertexes[fin];
	ex.clear();
	ex.neighbours[0] = 0;
	if (!detectSchemeAnalogical(finish, ex)) {
		return;
	}
	detectVertexes[start.neighbours[1]].isNotPartOfElement = false;
	detectVertexes[in].neighbours[1] = -1;

	detectVertexes[in].neighbours[18] = fin;
	detectVertexes[fin].neighbours[19] = in;
}


void schemeDetector::detectHorisontalSwitchSouthEastBreaker(int in) {
	detectSchemePoint ex;
	ex.clear();
	detectSchemePoint start = detectVertexes[in];
	ex.neighbours[4] = 0;
	ex.neighbours[7] = 0;
	if (!detectSchemeAnalogical(start, ex)) {
		return;
	}
	detectSchemePoint swit = detectVertexes[start.neighbours[7]];
	ex.clear();
	ex.neighbours[3] = 0;
	if (!detectSchemeAnalogical(swit, ex)) {
		return;
	}
	int dy = start.y - swit.y;
	int fin = detectFindClosestVertex(in, 0, -dy, -dy / 2);
	if (fin == -1) {
		return;
	}
	detectSchemePoint finish = detectVertexes[fin];
	ex.clear();
	ex.neighbours[0] = 0;
	if (!detectSchemeAnalogical(finish, ex)) {
		return;
	}
	detectVertexes[start.neighbours[7]].isNotPartOfElement = false;
	detectVertexes[in].neighbours[7] = -1;

	detectVertexes[in].neighbours[30] = fin;
	detectVertexes[fin].neighbours[31] = in;
}

//------------------------------------------------- main detectors

void schemeDetector::detectElements() {
	for (int i = 0; i < detectVertexes.size(); i++) {
		detectDfsVis.push_back(-1);
		detectColorMaxXY.push_back(0);
		detectColorMinXY.push_back(maxH + maxW);
		detectHorisontalResistor(i);
		detectVerticalResistor(i);
		detectHorisontalLamp(i);
		detectVerticalLamp(i);
		detectVerticalBattery(i);
		detectHorisontalBattery(i);
		detectVerticalSwitchNorthWestBreaker(i);
		detectVerticalSwitchNorthEastBreaker(i);
		detectVerticalSwitchSouthWestBreaker(i);
		detectVerticalSwitchSouthEastBreaker(i);
		detectHorisontalSwitchNorthWestBreaker(i);
		detectHorisontalSwitchSouthWestBreaker(i);
		detectHorisontalSwitchNorthEastBreaker(i);
		detectHorisontalSwitchSouthEastBreaker(i);
	}
}

void schemeDetector::detectDfsTrash(int cu, int color) {
	if (cu == -1) {
		return;
	}
	if (detectVertexes[cu].isNotPartOfElement == false) {
		return;
	}
	if (detectDfsVis[cu] >= 0) {
		return;
	}
	detectDfsVis[cu] = color;
	detectColorMinXY[color] = detectMin(detectColorMinXY[color], detectVertexes[cu].x + detectVertexes[cu].y);
	detectColorMaxXY[color] = detectMax(detectColorMaxXY[color], detectVertexes[cu].x + detectVertexes[cu].y);
	for (int i = 0; i < connectionTypes; i++) {
		detectDfsTrash(detectVertexes[cu].neighbours[i], color);
	}
}

void schemeDetector::detectAddTrash() {
	for (int i = 0; i < detectVertexes.size(); i++) {
		if (detectDfsVis[i] == -1 && detectVertexes[i].isNotPartOfElement == true) {
			detectDfsTrash(i, i);
		}
	}
	for (int i = 0; i < detectVertexes.size(); i++) {
		if (detectVertexes[i].isNotPartOfElement == true) {
			int color = detectDfsVis[i];
			if (detectColorMaxXY[color] - detectColorMinXY[color] < minPerimeter) {
				detectVertexes[i].isNotPartOfElement = false;
			}
		}
	}
}

void schemeDetector::detectDeleteTrashVertexes() {
	detectFinal.clear();
	detectConverter.clear();
	for (int i = 0; i < detectVertexes.size(); i++) {
		if (detectVertexes[i].isNotPartOfElement == false) {
			detectConverter.push_back(-1);
		}
		else {
			detectFinal.push_back(detectVertexes[i]);
			detectConverter.push_back(detectFinal.size() - 1);
		}
	}
	for (int i = 0; i < detectFinal.size(); i++) {
		for (int j = 0; j < connectionTypes; j++) {
			if (detectFinal[i].neighbours[j] != -1) {
				detectFinal[i].neighbours[j] = detectConverter[detectFinal[i].neighbours[j]];
			}
		}
	}
}

void schemeDetector::detectScheme() {
	detectGetGraph();
	detectElements();
	detectAddTrash();
	detectDeleteTrashVertexes();
}


void schemeDetector::detectOutScheme() {
	for (int i = 0; i < detectFinal.size(); i++) {
		for (int j = 0; j < connectionTypes; j++) {
			if (detectFinal[i].neighbours[j] > i) {
				int k = detectFinal[i].neighbours[j];
				std::string result = "";
				result += "A ";
				result += detectConnectiontypes[j];
				if (j >= 8 && j < 16) {
					result += " with R = ";
					result += from_number(detectFinal[i].val[j]);
					result += " * 10 ^ ";
					result += from_number(detectFinal[i].power[j]);
					result += " Om ";
				}
				if (j >= 40 && j < 48) {
					result += " with U = ";
					result += from_number(detectFinal[i].val[j]);
					result += " * 10 ^ ";
					result += from_number(detectFinal[i].power[j]);
					result += " V ";
				}
				result += " from ";
				result += from_number(i);
				result += " ( ";
				result += from_number(detectFinal[i].y);
				result += "; ";
				result += from_number(detectFinal[i].x);
				result += ") to ";
				result += from_number(k);
				result += " ( ";
				result += from_number(detectFinal[k].y);
				result += "; ";
				result += from_number(detectFinal[k].x);
				result += ") !";
				//qDebug(qUtf8Printable(result));
			}
		}
	}
	//qDebug("Detection ended.");
}

int schemeDetector::detectCharToInt(char l) {
	if (l == '0') {
		return 48;
	}
	if (l == '1') {
		return 49;
	}
	if (l == '2') {
		return 50;
	}
	if (l == '3') {
		return 51;
	}
	if (l == '4') {
		return 52;
	}
	if (l == '5') {
		return 53;
	}
	if (l == '6') {
		return 54;
	}
	if (l == '7') {
		return 55;
	}
	if (l == '8') {
		return 56;
	}
	if (l == '9') {
		return 57;
	}
	return 48;
}

int schemeDetector::detectGetMeasure(std::string a) {
	if (a == "") {
		return 0;
	}
	if (a[0] == 'O') {
		return 0;
	}
	if (a[0] == 'V') {
		return 0;
	}
	if (a[0] == 'E') {
		return 18;
	}
	if (a[0] == 'P') {
		return 15;
	}
	if (a[0] == 'T') {
		return 12;
	}
	if (a[0] == 'G') {
		return 9;
	}
	if (a[0] == 'M') {
		return 6;
	}
	if (a[0] == 'k') {
		return 3;
	}
	if (a[0] == 'h') {
		return 2;
	}
	if (a[0] == 'd' && a[1] == 'a') {
		return 1;
	}
	if (a[0] == 'd') {
		return -1;
	}
	if (a[0] == 's') {
		return -2;
	}
	if (a[0] == 'm') {
		return -3;
	}
	if (a[0] == 'u') {
		return -6;
	}
	if (a[0] == 'n') {
		return -9;
	}
	if (a[0] == 'p') {
		return -12;
	}
	if (a[0] == 'f') {
		return -15;
	}
	if (a[0] == 'a') {
		return -18;
	}
	if (a[0] == '*') {
		int st = 0;
		for (int i = 4; i < a.size(); i++) {
			if (a[i] < '0' || a[i] > '9') {
				return st;
			}
			st *= 10;
			st += detectCharToInt(a[i]) - 48;
		}
		return st;
	}
	return 0;
}

detectNumber schemeDetector::detectTransform(std::string a) {
	std::string b = "";
	for (int i = 0; i < a.size(); i++) {
		if (a[i] != ' ') {
			b = b + a[i];
		}
	}
	std::string number = "";
	std::string type = "";
	bool hasSt = false;
	for (int i = 0; i < b.size(); i++) {
		if (!((b[i] >= '0' && b[i] <= '9') || (b[i] == '.') || (b[i] == ','))) {
			hasSt = true;
		}
		if (!hasSt) {
			number = number + b[i];
		}
		else {
			type = type + b[i];
		}
	}
	int st = 0;
	int fullNumber = 0;
	if (number == "0") {
		fullNumber = 1;
	}
	for (int i = 0; i < number.size(); i++) {
		if (number[i] == '.' || number[i] == ',') {
			st = number.size() - i - 1;
		}
		else {
			fullNumber *= 10;
			fullNumber += detectCharToInt(number[i]) - 48;
		}
	}
	long double num = fullNumber;
	for (int i = 0; i < st; i++) {
		num /= 10;
	}
	int powd = detectGetMeasure(type);
	while (num > 10) {
		num /= 10;
		powd++;
	}
	while (num < 1) {
		num *= 10;
		powd--;
	}
	detectNumber res;
	res.val = num;
	res.pow = powd;
	return res;
}

void schemeDetector::detectImproveSides(int i, int k) {
	int xa = detectVertexes[i].x;
	int ya = detectVertexes[i].y;
	int xb = detectVertexes[k].x;
	int yb = detectVertexes[k].y;
	int xc = (4 * xa - xb) / 3;
	int yc = (4 * ya - yb) / 3;
	int xd = (4 * xb - xa) / 3;
	int yd = (4 * yb - ya) / 3;
	detectVertexes[i].x = xc;
	detectVertexes[i].y = yc;
	detectVertexes[k].x = xd;
	detectVertexes[k].y = yd;
}

void schemeDetector::detectMergeElements(int i, int k) {
	for (int j = 0; j < detectVertexes.size(); j++) {
		if (j == i || j == k) {
			continue;
		}
		for (int l = 0; l < connectionTypes; l++) {
			if (detectVertexes[j].neighbours[l] == k) {
				detectVertexes[j].neighbours[l] = i;
			}
		}
	}
	for (int l = 0; l < connectionTypes; l++) {
		int ne = detectVertexes[k].neighbours[l];
		if (ne != -1 && ne != i) {
			detectVertexes[i].neighbours[l] = ne;
		}
	}
	for (int l = 0; l < connectionTypes; l++) {
		if (detectVertexes[i].neighbours[l] == k) {
			detectVertexes[i].neighbours[l] = -1;
		}
	}
	detectVertexes[i].x = (detectVertexes[i].x + detectVertexes[k].x) / 2;
	detectVertexes[i].y = (detectVertexes[i].y + detectVertexes[k].y) / 2;
	detectVertexes[k].isNotPartOfElement = false;
}

void schemeDetector::detectTransformElements() {
	detectVertexes = detectFinal;
	detectFinal.clear();
	for (int i = 0; i < detectVertexes.size(); i++) {
		detectVertexes[i].isNotPartOfElement = true;
	}
	for (int i = 0; i < detectVertexes.size(); i++) {
		for (int j = 8; j < connectionTypes; j++) {
			int k = detectVertexes[i].neighbours[j];
			if (k != -1 && k > i) {
				detectImproveSides(i, k);
			}
			if (k != -1 && k > i && j >= 40 && j < 48) {
				detectImproveSides(i, k);
				detectImproveSides(i, k);
			}
		}
	}
	for (int i = 0; i < detectVertexes.size(); i++) {
		for (int j = 0; j < 8; j++) {
			int k = detectVertexes[i].neighbours[j];
			if (k != -1 && k > i) {
				detectPoint iP = detectPoint(detectVertexes[i].x, detectVertexes[i].y);
				detectPoint kP = detectPoint(detectVertexes[k].x, detectVertexes[k].y);
				if (detectDistBetweenPoints(iP, kP) < mergeImproveBorders) {
					detectMergeElements(i, k);
					//qDebug("Yay!");
				}
			}
		}
	}
	detectDeleteTrashVertexes();
}

long double schemeDetector::detectPow(long double a, int st) {
	long double re = a;
	for (int i = 0; i < st; i++) {
		re *= 10;
	}
	for (int i = 0; i < -st; i++) {
		re /= 10;
	}
	return re;
}

void schemeDetector::buildCorners() {
	for (int i = 0; i < detectFinal.size(); i++) {
		for (int j = 0; j < connectionTypes; j++) {
			int k = detectFinal[i].neighbours[j];
			if (k != -1 && detectFinal[i].x != detectFinal[k].x && detectFinal[i].y != detectFinal[k].y) {
				if (0 <= j && j < 8) {
					if (abs(detectFinal[i].x - detectFinal[k].x) > abs(detectFinal[i].y - detectFinal[k].y)) {
						detectFinal.push_back(detectSchemePoint((detectFinal[i].x + detectFinal[k].x) / 2, detectFinal[i].y));
						detectFinal.push_back(detectSchemePoint((detectFinal[i].x + detectFinal[k].x) / 2, detectFinal[k].y));
						detectFinal[i].neighbours[j] = detectFinal.size() - 2;
						detectFinal[detectFinal.size() - 2].neighbours[(j + 4) % 8] = i;
						detectFinal[k].neighbours[(j + 4) % 8] = detectFinal.size() - 1;
						detectFinal[detectFinal.size() - 1].neighbours[j] = k;
						if (detectFinal[i].y > detectFinal[k].y) {
							detectFinal[detectFinal.size() - 1].neighbours[6] = detectFinal.size() - 2;
							detectFinal[detectFinal.size() - 2].neighbours[2] = detectFinal.size() - 1;
						}
						else {
							detectFinal[detectFinal.size() - 1].neighbours[2] = detectFinal.size() - 2;
							detectFinal[detectFinal.size() - 2].neighbours[6] = detectFinal.size() - 1;
						}
					}
					else {
						detectFinal.push_back(detectSchemePoint(detectFinal[i].x, (detectFinal[i].y + detectFinal[k].y) / 2));
						detectFinal.push_back(detectSchemePoint(detectFinal[k].x, (detectFinal[i].y + detectFinal[k].y) / 2));
						detectFinal[i].neighbours[j] = detectFinal.size() - 2;
						detectFinal[detectFinal.size() - 2].neighbours[(j + 4) % 8] = i;
						detectFinal[k].neighbours[(j + 4) % 8] = detectFinal.size() - 1;
						detectFinal[detectFinal.size() - 1].neighbours[j] = k;
						if (detectFinal[i].x > detectFinal[k].x) {
							detectFinal[detectFinal.size() - 1].neighbours[0] = detectFinal.size() - 2;
							detectFinal[detectFinal.size() - 2].neighbours[4] = detectFinal.size() - 1;
						}
						else {
							detectFinal[detectFinal.size() - 1].neighbours[4] = detectFinal.size() - 2;
							detectFinal[detectFinal.size() - 2].neighbours[0] = detectFinal.size() - 1;
						}
					}
				}
				else {
					int xi = detectFinal[i].x;
					int yi = detectFinal[i].y;
					int xk = detectFinal[k].x;
					int yk = detectFinal[k].y;
					if (j == 9 || j == 13 || j == 17 || j == 21 || j == 25 || j == 29 || j == 33 || j == 37 || j == 41 || j == 45) {
						for (int it = 0; it < 4; it++) {
							detectFinal.push_back(detectSchemePoint());
						}
						int s = detectFinal.size();
						detectFinal[s - 4].x = xi;
						detectFinal[s - 4].y = yi + cellElWidth;
						detectFinal[s - 3].x = (xi + xk) / 2;
						detectFinal[s - 3].y = yi + cellElWidth;
						detectFinal[s - 2].x = (xi + xk) / 2;;
						detectFinal[s - 2].y = yk - cellElWidth;
						detectFinal[s - 1].x = xk;
						detectFinal[s - 1].y = yk - cellElWidth;

						detectFinal[i].neighbours[j] = -1;
						detectFinal[k].neighbours[j - 1] = -1;

						detectFinal[i].neighbours[6] = s - 4;
						detectFinal[s - 4].neighbours[2] = i;

						detectFinal[s - 3].neighbours[j] = s - 2;
						detectFinal[s - 2].neighbours[j - 1] = s - 3;

						detectFinal[s - 1].neighbours[6] = k;
						detectFinal[k].neighbours[2] = s - 1;

						if (xi > xk) {
							detectFinal[s - 4].neighbours[4] = s - 3;
							detectFinal[s - 3].neighbours[0] = s - 4;

							detectFinal[s - 2].neighbours[4] = s - 1;
							detectFinal[s - 1].neighbours[0] = s - 2;
						}
						else {
							detectFinal[s - 4].neighbours[0] = s - 3;
							detectFinal[s - 3].neighbours[4] = s - 4;

							detectFinal[s - 2].neighbours[0] = s - 1;
							detectFinal[s - 1].neighbours[4] = s - 2;
						}
					}
					if (j == 10 || j == 14 || j == 18 || j == 22 || j == 26 || j == 30 || j == 34 || j == 38 || j == 42 || j == 46) {
						for (int it = 0; it < 4; it++) {
							detectFinal.push_back(detectSchemePoint());
						}
						int s = detectFinal.size();
						detectFinal[s - 4].x = xi + cellElWidth;
						detectFinal[s - 4].y = yi;
						detectFinal[s - 3].x = xi + cellElWidth;
						detectFinal[s - 3].y = (yi + yk) / 2;
						detectFinal[s - 2].x = xk - cellElWidth;
						detectFinal[s - 2].y = (yi + yk) / 2;
						detectFinal[s - 1].x = xk - cellElWidth;
						detectFinal[s - 1].y = yk;

						detectFinal[i].neighbours[j] = -1;
						detectFinal[k].neighbours[j + 1] = -1;

						detectFinal[i].neighbours[0] = s - 4;
						detectFinal[s - 4].neighbours[4] = i;

						detectFinal[s - 3].neighbours[j] = s - 2;
						detectFinal[s - 2].neighbours[j + 1] = s - 3;

						detectFinal[s - 1].neighbours[0] = k;
						detectFinal[k].neighbours[4] = s - 1;

						if (yi > yk) {
							detectFinal[s - 4].neighbours[2] = s - 3;
							detectFinal[s - 3].neighbours[6] = s - 4;

							detectFinal[s - 2].neighbours[2] = s - 1;
							detectFinal[s - 1].neighbours[6] = s - 2;
						}
						else {
							detectFinal[s - 4].neighbours[6] = s - 3;
							detectFinal[s - 3].neighbours[2] = s - 4;

							detectFinal[s - 2].neighbours[6] = s - 1;
							detectFinal[s - 1].neighbours[2] = s - 2;
						}
					}
				}
			}
		}
	}
}

void schemeDetector::addHorisontalWire(detectPoint a) {
	detectElement b = detectElement(a.x, a.y, 0, 0);
	detectEl.push_back(b);
}

void schemeDetector::addVerticalWire(detectPoint a) {
	detectElement b = detectElement(a.x, a.y, 0, 1);
	detectEl.push_back(b);
}

void schemeDetector::buildWire(detectPoint a, detectPoint b) {
	if (detectDistBetweenPoints(a, b) > 1.6) {
		detectPoint c = detectPoint((a.x + b.x) / 2, (a.y + b.y) / 2);
		buildWire(a, c);
		buildWire(c, b);
		return;
	}
	if (a.x == b.x) {
		if (a.y == b.y) {
			return;
		}
		if (a.y == b.y - 1) {
			addVerticalWire(a);
			return;
		}
		if (a.y == b.y + 1) {
			addVerticalWire(b);
			return;
		}
	}
	if (a.x == b.x - 1) {
		if (a.y == b.y) {
			addHorisontalWire(a);
			return;
		}
		if (a.y == b.y - 1) {
			addHorisontalWire(a);
			addVerticalWire(detectPoint(b.x, a.y));
			return;
		}
		if (a.y == b.y + 1) {
			addHorisontalWire(a);
			addVerticalWire(b);
			return;
		}
	}
	if (a.x == b.x + 1) {
		if (a.y == b.y) {
			addHorisontalWire(b);
			return;
		}
		if (a.y == b.y - 1) {
			addVerticalWire(detectPoint(b.x, a.y));
			addHorisontalWire(detectPoint(b.x, a.y));
			return;
		}
		if (a.y == b.y + 1) {
			addVerticalWire(b);
			addHorisontalWire(detectPoint(b.x, a.y));
			return;
		}
	}
}

void schemeDetector::addElement(detectPoint a, int j) {
	if (j == 8) {
		detectEl.push_back(detectElement(a.x - 1, a.y - cellElSize, 1, 1));
	}
	if (j == 9) {
		detectEl.push_back(detectElement(a.x - 1, a.y, 1, 1));
	}
	if (j == 10) {
		detectEl.push_back(detectElement(a.x, a.y - 1, 1, 0));
	}
	if (j == 11) {
		detectEl.push_back(detectElement(a.x - cellElSize, a.y - 1, 1, 0));
	}
	if (j == 12) {
		detectEl.push_back(detectElement(a.x - cellElWidth, a.y - cellElSize, 2, 1));
	}
	if (j == 13) {
		detectEl.push_back(detectElement(a.x - cellElWidth, a.y, 2, 1));
	}
	if (j == 14) {
		detectEl.push_back(detectElement(a.x, a.y - cellElWidth, 2, 0));
	}
	if (j == 15) {
		detectEl.push_back(detectElement(a.x - cellElSize, a.y - cellElWidth, 2, 0));
	}
	if (j == 40) {
		detectEl.push_back(detectElement(a.x - cellElWidth, a.y - cellElSize, 3, 1));
	}
	if (j == 41) {
		detectEl.push_back(detectElement(a.x - cellElWidth, a.y, 3, 1));
	}
	if (j == 42) {
		detectEl.push_back(detectElement(a.x, a.y - cellElWidth, 3, 0));
	}
	if (j == 43) {
		detectEl.push_back(detectElement(a.x - cellElSize, a.y - cellElWidth, 3, 0));
	}
	if (j == 44) {
		detectEl.push_back(detectElement(a.x - cellElWidth, a.y - cellElSize, 3, 1));
	}
	if (j == 45) {
		detectEl.push_back(detectElement(a.x - cellElWidth, a.y, 3, 1));
	}
	if (j == 46) {
		detectEl.push_back(detectElement(a.x, a.y - cellElWidth, 3, 0));
	}
	if (j == 47) {
		detectEl.push_back(detectElement(a.x - cellElSize, a.y - cellElWidth, 3, 0));
	}
}

void schemeDetector::detectParse(int i, int ji, int k, int jk) {
	detectPoint ip = detectPoint(detectFinal[i].x, detectFinal[i].y);
	detectPoint kp = detectPoint(detectFinal[k].x, detectFinal[k].y);
	if (0 <= ji && ji < 8) {
		buildWire(ip, kp);
		return;
	}
	detectPoint mp = detectPoint((ip.x + kp.x) / 2, (ip.y + kp.y) / 2);
	int di = round(detectDistBetweenPoints(ip, mp));
	int dy = (ip.y - mp.y) / di * (cellElSize / 2);
	int dx = (ip.x - mp.x) / di * (cellElSize / 2);
	detectPoint is = detectPoint(mp.x + dx, mp.y + dy);
	di = round(detectDistBetweenPoints(kp, mp));
	dy = (kp.y - mp.y) / di * ((cellElSize + 1) / 2);
	dx = (kp.x - mp.x) / di * ((cellElSize + 1) / 2);
	detectPoint ks = detectPoint(mp.x + dx, mp.y + dy);
	addElement(is, ji);
	buildWire(ip, is);
	buildWire(ks, kp);
}

void schemeDetector::fillEl() {
	for (int i = 0; i < detectFinal.size(); i++) {
		for (int j = 0; j < connectionTypes; j++) {
			if ((j < 4) || (j >= 8 && j % 2 == 0)) {
				int k = detectFinal[i].neighbours[j];
				int j1;
				if (j < 4) {
					j1 = j + 4;
				}
				else {
					j1 = j + 1;
				}
				if (k != -1) {
					detectParse(i, j, k, j1);
				}
			}
		}
	}
}

std::string schemeDetector::detectGiveOut() {
	std::vector <int> base;
	for (int i = 0; i < detectFinal.size(); i++) {
		detectVertexesNumbers.push_back(base);
	}
	std::string vertexes = "";
	std::string edges = "";
	int verS = 0;
	int edgS = 0;
	for (int i = 0; i < detectFinal.size(); i++) {
		int conHere = -1;
		for (int j = 0; j < connectionTypes; j++) {
			if (detectFinal[i].neighbours[j] != -1) {
				conHere++;
				verS++;
			}
		}
		edgS += conHere;
	}
	verS /= 2;
	vertexes = vertexes + from_number(verS);
	edges = edges + from_number(edgS);
	vertexes += " ";
	edges += " ";
	int vertexNu = -1;
	for (int i = 0; i < detectFinal.size(); i++) {
		for (int j = 0; j < connectionTypes; j++) {
			int k = detectFinal[i].neighbours[j];
			if (k != -1 && k > i) {
				vertexNu++;
				if (j >= 0 && j < 8) {
					vertexes += from_number(3);
				}
				if (j >= 8 && j < 12) {
					vertexes += from_number(0);
				}
				if (j >= 12 && j < 16) {
					vertexes += from_number(1);
				}
				if (j >= 40 && j < 48) {
					if (j == 41 || j == 43 || j == 44 || j == 46) {
						std::swap(i, k);
					}
					vertexes += from_number(2);
				}
				vertexes += " 0 ";
				if (j >= 40 && j < 48) {
					vertexes += from_number(double(detectPow(detectFinal[i].val[j], detectFinal[i].power[j])));
				}
				else {
					vertexes += from_number(0);
				}
				vertexes += " ";
				if (j >= 8 && j < 16) {
					vertexes += from_number(double(detectPow(detectFinal[i].val[j], detectFinal[i].power[j])));
				}
				else {
					vertexes += from_number(0);
				}
				vertexes += " 0 ";
				detectPoint iP = detectPoint(detectFinal[i].y, detectFinal[i].x);
				detectPoint kP = detectPoint(detectFinal[k].y, detectFinal[k].x);
				vertexes += from_number(detectDistBetweenPoints(iP, kP));
				vertexes += " ";
				int s = 90 + (detectGetAzimut(iP, kP) * 180 / MM_PI);
				if (s > 180) {
					s -= 360;
				}
				vertexes += from_number(s);
				vertexes += " ";
				vertexes += from_number(iP.y / 2 + kP.y / 2);
				vertexes += " ";
				vertexes += from_number(iP.x / 2 + kP.x / 2);
				vertexes += " ";
				detectVertexesNumbers[i].push_back(2 * vertexNu);
				detectVertexesNumbers[k].push_back(2 * vertexNu + 1);
				if (j == 41 || j == 43 || j == 44 || j == 46) {
					std::swap(i, k);
				}
			}
		}
	}
	for (int i = 0; i < detectFinal.size(); i++) {
		if (detectVertexesNumbers[i].size() > 1) {
			for (int j = 1; j < detectVertexesNumbers[i].size(); j++) {
				edges += from_number(detectVertexesNumbers[i][0]);
				edges += " ";
				edges += from_number(detectVertexesNumbers[i][j]);
				edges += " ";
			}
		}
	}
	vertexes += edges;
	return vertexes;
}

int schemeDetector::detectMain()
{
	int elSize = 0;
	detectStarterImage.loadFromFile(fileName);
	sf::Vector2u detectSides = detectStarterImage.getSize();
	width = detectSides.x;
	height = detectSides.y;
	for (int j = 0; j < width; j++) {
		for (int i = 0; i < height; i++) {
			detectStarterArray[i][j] = detectStarterImage.getPixel(i, j);
			double blueColor = detectStarterArray[i][j].b / 256.0;
			double redColor = detectStarterArray[i][j].r / 256.0;
			double greenColor = detectStarterArray[i][j].g / 256.0;
			detectBWArray[i][j] = !contrast(redColor, blueColor, greenColor);
		}
	}
	detectScheme();
	int cellSize = 1000;
	int cuString = -1;
	detectTransformElements();
	for (int i = 0; i < detectFinal.size(); i++) {
		for (int j = 8; j < connectionTypes; j++) {
			if (j % 2 == 0) {
				if (detectFinal[i].neighbours[j] != -1) {
					int k = detectFinal[i].neighbours[j];
					cuString++;
					detectFinal[i].power[j] = 0;
					detectFinal[i].val[j] = 1;
					detectFinal[k].power[j + 1] = 0;
					detectFinal[k].val[j + 1] = 1;
					int dif = detectMax(abs(detectFinal[i].x - detectFinal[k].x), abs(detectFinal[i].y - detectFinal[k].y));
					cellSize = detectMin(cellSize, dif);
				}
			}
		}
	}
	cellSize = cellSize / (cellCo * cellElSize);
	for (int i = 0; i < detectFinal.size(); i++) {
		std::pair<int, int> newCoor = detectCellRound(detectFinal[i].x, detectFinal[i].y);
		detectFinal[i].x = newCoor.first;
		detectFinal[i].y = newCoor.second;
	}
	buildCorners();
	fillEl();
	detectOutScheme();
	result = detectGiveOut();
	return 0;
}



std::string schemeDetector::detect(std::string filename) {
	this->fileName = filename;
	int g = detectMain();
	return result;
}