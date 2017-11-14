#include "VersionHandler.h"


VersionHandler::VersionHandler()
	:curVersion(0)
	, versions(std::vector<FieldVersion*>(1, new FieldVersion()))
{
}


VersionHandler::~VersionHandler()
{
}

void VersionHandler::addVersion(FieldVersion* version) {
	while (versions.size() - 1 != curVersion) {
		versions.pop_back();
	}
	versions.push_back(version);
	curVersion++;
}

void VersionHandler::undo() {
	if (curVersion > 0) {
		curVersion--;
	}
}

void VersionHandler::redo() {
	if (curVersion < versions.size() - 1) {
		curVersion++;
	}
}

void VersionHandler::saveToFile(std::string filename)
{
	versions[curVersion]->saveToFile(filename);
}

void VersionHandler::reset() {
	curVersion = 0;
	while (versions.size() - 1 != curVersion) {
		versions.pop_back();
	}
}

void VersionHandler::openFromFile(std::string filename) {
	reset();
	FieldVersion * temp = new FieldVersion();
	temp->openFromFile(filename);
	addVersion(temp);
}

FieldVersion* VersionHandler::getCurrentVersion()
{
	return versions[curVersion];
}
