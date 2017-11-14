#pragma once
#include "FieldVersion.h"
#include <vector>
#include <string>
class VersionHandler
{
public:
	VersionHandler();
	~VersionHandler();
	void addVersion(FieldVersion* version);
	void undo();
	void redo();
	void saveToFile(std::string filename);
	void openFromFile(std::string filename);
	void reset();
	int curVersion;
	std::vector<FieldVersion*> versions;
	FieldVersion* getCurrentVersion();
};

