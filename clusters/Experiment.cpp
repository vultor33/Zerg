#include "Experiment.h"

#include <iostream>

#include "ReadGaInput.h"
#include "ClustersFitness.h"
#include "../AuxMathGa.h"
#include "../AuxMath.h"
#include "../GeneticAlgorithm.h"

using namespace zerg;
using namespace std;

/*
objeto que seta os parametros e roda o ga umas
5 vezes com os mesmos parametros mas com sementes diferentes.

*/

Experiment::Experiment() {}

Experiment::~Experiment() {}

void Experiment::makeExperiment(int seed, string experimentMethod)
{
	int nAtoms = 26;

	ReadGaInput readGa_;
		
	readGa_.setExperimentDefaults(seed);

	zerg::GaParameters gaParam = readGa_.getGaParameters();

	gaParam.experimentMethod = experimentMethod;

	setExperiment(experimentMethod, gaParam);

	AuxMathGa::set_seed(gaParam.seed);

	gaParam.numberOfParameters = 3 * nAtoms;

	vector<string> options = readGa_.getOptions();

	ClustersFitness clFit_(
		gaParam,
		options,
		readGa_.getGamessPath(),
		readGa_.getGamessScr(),
		readGa_.getGamessNprocess());

	GeneticAlgorithm ga1(clFit_, gaParam);

	ga1.ga_start();



	// avaliar a melhora da populacao como um todo.
	// nao apenas o melhor individuo
	// o 26 e um caso classico, acho que eu poderia me concentrar nele.
	// a qualidade dos operadores ao longo do tempo
	// se as variacoes sao melhores no inicio da simulacao, no final, ou nao depende e tal.
	// adicionar um Nlm e um Ncalls
}

void Experiment::setExperiment(std::string experimentMethod, zerg::GaParameters & gaParam)
{
	if (experimentMethod == "Imigration")
		gaParam.initialCreationRate[0] = 0.5e0;
	else if (experimentMethod == "CrossoverMean")
		gaParam.initialCreationRate[1] = 0.5e0;
	else if (experimentMethod == "CrossoverTwoPoints")
		gaParam.initialCreationRate[2] = 0.5e0;
	else if (experimentMethod == "SphereCrossover")
		gaParam.initialCreationRate[5] = 0.5e0;
	else if (experimentMethod == "CartesianDisplacement")
		gaParam.initialCreationRate[6] = 0.5e0;
	else if (experimentMethod == "GeometricCenterDisplacement")
		gaParam.initialCreationRate[7] = 0.5e0;
	else if (experimentMethod == "TwistOperator")
		gaParam.initialCreationRate[8] = 0.5e0;
	else if (experimentMethod == "AngularOperator")
		gaParam.initialCreationRate[9] = 0.5e0;
	else if (experimentMethod == "AngularSurface")
		gaParam.initialCreationRate[10] = 0.5e0;
	else if (experimentMethod == "MoveToCenter")
		gaParam.initialCreationRate[11] = 0.5e0;
	else if (experimentMethod == "ModifiedAngularSurface")
		gaParam.initialCreationRate[12] = 0.5e0;
	else if (experimentMethod == "DeavenHoCutSplice")
		gaParam.initialCreationRate[13] = 0.5e0;
	else if (experimentMethod == "SmallAutoAdjust")
	{
		for (size_t i = 0; i < gaParam.initialCreationRate.size(); i++)
			gaParam.initialCreationRate[i] = 1.0e0 / (int)gaParam.initialCreationRate.size();
		gaParam.adminMaxCreationVariation = 0.2e0;
	}
	else if (experimentMethod == "MediumAutoAdjust")
	{
		for (size_t i = 0; i < gaParam.initialCreationRate.size(); i++)
			gaParam.initialCreationRate[i] = 1.0e0 / (int)gaParam.initialCreationRate.size();
		gaParam.adminMaxCreationVariation = 0.5e0;
	}
	else if (experimentMethod == "LargeAutoAdjust")
	{
		for (size_t i = 0; i < gaParam.initialCreationRate.size(); i++)
			gaParam.initialCreationRate[i] = 1.0e0 / (int)gaParam.initialCreationRate.size();
		gaParam.adminMaxCreationVariation = 0.9e0;
	}
	else
	{
		cout << "method not found" << endl;
		exit(1);
	}
}
