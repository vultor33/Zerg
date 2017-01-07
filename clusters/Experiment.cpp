#include "Experiment.h"

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

void Experiment::makeExperiment(string inputName)
{
	ReadGaInput readGa_;

	readGa_.inputName = inputName;
		
	readGa_.readGaInput();

	run(readGa_, 5, 7);
	
	// avaliar a melhora da populacao como um todo.
	// nao apenas o melhor individuo

}

void Experiment::run(ReadGaInput & readGa_, int nAtoms, int seed)
{
	AuxMathGa::set_seed(seed);

	zerg::GaParameters gaParam = readGa_.getGaParameters();

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

}


