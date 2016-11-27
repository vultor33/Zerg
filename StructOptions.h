#ifndef STRUCTOPTIONS_H
#define STRUCTOPTIONS_H

#include <vector>

namespace zerg{
struct GaOptions
{
	bool printBestWorseIndividuals;
	bool printAllIndividuals;
	bool printVariationOfCreationMethods;
	bool similarityProblem;
};

struct GaParameters
{
	bool default;
	int n_process;
	int pop_size;
	double highlanderInitialFitness;
	int highlanderMaxIteration;
	std::vector<double> initialCreationRate;
	int predatorMethod;

};

}

#endif