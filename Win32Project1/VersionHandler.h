#pragma once
#include "FieldVersion.h"
#include <vector>
class VersionHandler
{
public:
	VersionHandler();
	~VersionHandler();
	void addVersion(FieldVersion version);
	void undo();
	void redo();
	FieldVersion getCurrentVersion();
	int curVersion;
	std::vector<FieldVersion> versions;
};

