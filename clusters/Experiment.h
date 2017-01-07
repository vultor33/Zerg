#ifndef EXPERIMENT_H
#define EXPERIMENT_H

#include <string>
#include "ReadGaInput.h"

#include "../StructOptions.h"

class Experiment
{
public:
	Experiment();

	~Experiment();

	void makeExperiment(std::string inputName);

	void run(ReadGaInput & readGa_, int nAtoms, int seed);

};

#endif

