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
	int n_process;
	int pop_size;
	double highlanderInitialFitness;
	int highlanderMaxIteration;
	std::vector<double> initialCreationRate;
	double adminLargeEnergyVariation;
	double adminMaxCreationVariation;
	int predatorMethod;
	double mutationValue;
	double crossoverWeight;
	double corssoverProbability;
	double gammaInitializeAtoms;
	double rcaInitializeAtoms;
	int insistOnSimilar;


};

}

#endif