#ifndef READGAINPUT_H
#define READGAINPUT_H

#include "../StructOptions.h"

#include <string>
#include <vector>

class ReadGaInput
{
public:
	ReadGaInput();
	~ReadGaInput();

	std::vector<std::string> getOptions();

	int getSeed() {return seed;}

	zerg::GaParameters getGaParameters() { return gaParam; }

	std::string getGamessPath() { return gamessPath; }

	std::string getGamessNprocess() { return gamessNproc; }

private:
	int seed;
	std::string inputName = "GaInput.txt";
	std::vector<std::string> options;
	zerg::GaParameters gaParam;
	std::string gamessPath;
	std::string gamessNproc;
	void setDefaults();

};


#endif

