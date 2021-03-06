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

	void readGaInput();

	void setExperimentDefaults(int seed);

	void setInputInformations(zerg::GaParameters gaParam_in);

	std::vector<std::string> getOptions();

	int getSeed() {return seed;}

	zerg::GaParameters getGaParameters() { return gaParam; }

	std::string getGamessPath() { return gamessPath; }

	std::string getGamessNprocess() { return gamessNproc; }

	std::string getGamessScr() { return gamessScr; }

	std::string inputName;

private:
	int seed;
	std::vector<std::string> options;
	zerg::GaParameters gaParam;
	std::string gamessPath;
	std::string gamessScr;
	std::string gamessNproc;
	void setDefaults();
};


#endif

