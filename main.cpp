#include <iostream>
#include <math.h>
#include <fstream>
#include <string>

#include "BasicOperators.h"
#include "GeneticAlgorithm.h"
#include "Hooklaw.h"
#include "UserFitness.h"
#include "AuxMathGa.h"

#include "clusters\InitializeAtoms.h"
#include "clusters\ClustersFitness.h"
#include "clusters\ReadQuantumOutput.h"
#include "clusters\WriteQuantumInput.h"
#include "clusters\ReadGaInput.h"

using namespace std;
using namespace zerg;

/*
impotante: opcao para desativar variacoes e regular o que e um valor grande:
double FuzzyAdministration::getCreateRateVariation

estudo das formas de gerar clusters iniciais: 
- Tem o de Sao Carlos, esfera, cubo e árvore.

- ele checa similaridade so 30 vezes, se nao passar vai igual mesmo.
  observar isso.

- gerador de condicao inicial muito ruim (nao consegue 50 atomos)


*/

void printAtomsVectorDouble(vector<double> & atoms);

void setGaParamDefaults(GaParameters & gaParam);

int main(int argc, char *argv[])
{
	ReadGaInput readGa_;
	string gaInput;
	if (argc != 1)
	{
		gaInput = argv[1];
		readGa_.inputName = gaInput;		
	}

	readGa_.readGaInput();

	AuxMathGa::set_seed(readGa_.getSeed());

	ClustersFitness clFit_(
		readGa_.getGaParameters(),
		readGa_.getOptions(),
		readGa_.getGamessPath(),
		readGa_.getGamessScr(),
		readGa_.getGamessNprocess());

	GeneticAlgorithm ga1(clFit_,readGa_.getGaParameters());
	ga1.ga_start();

	clFit_.printAllIndividualas("finalPopulation.xyz");

	return 0;
}

void printAtomsVectorDouble(vector<double> & atoms)
{
	int natm = atoms.size() / 3;
	ofstream teste_("teste.xyz");
	teste_ << natm << endl << "t" << endl;
	for (int i = 0; i < natm; i++)
	{
		teste_ << "H "
			<< atoms[i] << "  "
			<< atoms[i + natm] << "  "
			<< atoms[i + 2 * natm] << endl;
	}
	teste_.close();
}

void setGaParamDefaults(GaParameters & gaParam)
{
	gaParam.n_process = 1;
	gaParam.pop_size = 200;
	gaParam.maxGeneration = 500;
	gaParam.highlanderInitialFitness = 1.0e99;
	gaParam.highlanderMaxIteration = 100;
	gaParam.adminLargeEnergyVariation = 2.0e0;
	gaParam.adminMaxCreationVariation = 0.9e0;
	gaParam.predatorMethod = 0;
	gaParam.mutationValue = 0.1e0;
	gaParam.crossoverWeight = 0.7e0;
	gaParam.corssoverProbability = 0.7e0;
	gaParam.numberOfParameters = 21;
	gaParam.gammaInitializeAtoms = 0.2;
	gaParam.rcaInitializeAtoms = 1.0;
	gaParam.maxDistance = 1.0e99;
	gaParam.insistOnSimilar = 30;
}


