#include <iostream>
#include <math.h>
#include <fstream>
#include <string>

#include "BasicOperators.h"
#include "GeneticAlgorithm.h"
#include "Hooklaw.h"
#include "UserFitness.h"
#include "AuxMathGa.h"

#include "clusters/InitializeAtoms.h"
#include "clusters/ClustersFitness.h"
#include "clusters/ReadQuantumOutput.h"
#include "clusters/WriteQuantumInput.h"
#include "clusters/ReadGaInput.h"
#include "clusters/Fitness.h"
#include "clusters/ClustersOperators.h"

using namespace std;
using namespace zerg;

/*
estudo das formas de gerar clusters iniciais:
- Tem o de Sao Carlos, esfera, cubo e árvore.

- usar a algebra de polya para contar isomeros lennard jones dos bimetalicos,
  gerar todos e avaliar suas diferencas.

- remover os outros branchs

- os operadores deveriam seguir o seguinte paradigma receber
  sempre coordenadas.

- colocar const no cluster operators por seguranca
*/

void printAtomsVectorDouble(vector<double> & atoms, string testName = "teste.xyz");

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

	zerg::GaParameters gaParam = readGa_.getGaParameters();

	vector<string> options = readGa_.getOptions();

	ClustersFitness clFit_(
		gaParam,
		options,
		readGa_.getGamessPath(),
		readGa_.getGamessScr(),
		readGa_.getGamessNprocess());

	GeneticAlgorithm ga1(clFit_, gaParam);
	ga1.ga_start();

	clFit_.printAllIndividualas("finalPopulation.xyz");

	return 0;
}

void printAtomsVectorDouble(vector<double> & atoms, string testName)
{
	int natm = atoms.size() / 3;
	ofstream teste_(testName.c_str());
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

