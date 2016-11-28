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

using namespace std;
using namespace zerg;

/*
impotante: opcao para desativar variacoes e regular o que e um valor grande:
double FuzzyAdministration::getCreateRateVariation

estudo das formas de gerar clusters iniciais: 
- Tem o de Sao Carlos, esfera, cubo e árvore.

- ele checa similaridade so 30 vezes, se nao passar vai igual mesmo.
  observar isso.


*/

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

int main()
{
	AuxMathGa::set_seed(3);

	GaParameters gaParam;
	gaParam.default = true; //input

	InitializeAtoms init_;
	vector<double> atomos = init_.generateCluster(4, 0.1e0, 1);
	printAtomsVectorDouble(atomos);

	int pop_size = 80; //multiplo de 4 e ponto final
	int n_param = 3;
	int maxGeneration = 5;
	UserFitness hk(pop_size, n_param);

	GeneticAlgorithm ga1(hk,gaParam);
	ga1.ga_start(maxGeneration);

	cout << "tlu tlu tlu tlu " << endl;
	return 0;
}
