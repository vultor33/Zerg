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
	int maxGeneration;
	double highlanderInitialFitness;
	int highlanderMaxIteration;
	double adminLargeEnergyVariation;
	double adminMaxCreationVariation;
	int predatorMethod;
	double mutationValue;
	double crossoverWeight;
	double corssoverProbability;
	int numberOfParameters;
	double gammaInitializeAtoms;
	double rcaInitializeAtoms;
	double maxDistance;
	int insistOnSimilar;
	std::vector<double> initialCreationRate;


};

}

#endif