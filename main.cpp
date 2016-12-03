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

int main()
{
	AuxMathGa::set_seed(3);
	GaParameters gaParam;
	setGaParamDefaults(gaParam);
//	gaParam.maxGeneration = 5;
	gaParam.numberOfParameters = 15;
	ClustersFitness clFit_(gaParam);

//	GeneticAlgorithm ga1(clFit_,gaParam);
//	ga1.ga_start();


//	ReadQuantumOutput readQ_("gamess");
//	readQ_.readOutput("na0li6.xyz1");

	vector<string> options(16);
	options[0] = "gamess";
	options[1] = "teste";
	options[2] = " $CONTRL SCFTYP=RHF RUNTYP=OPTIMIZE EXETYP=RUN MPLEVL=2 MAXIT=200 MULT=1";
	options[3] = "   ISPHER=1 COORD=UNIQUE NOSYM=1 UNITS=ANGS $END";
	options[4] = " $GUESS GUESS=HUCKEL $END";
	options[5] = " $SYSTEM MWORDS=40 MEMDDI=20  $END";
	options[6] = " $SCF DIRSCF=.FALSE. $END";
	options[7] = " $DATA";
	options[8] = "titulo";
	options[9] = "C1";
	options[10] = "EndOfHeader";
	options[11] = "li-base.txt";
	options[12] = "li-base.txt";
	options[13] = "li-base.txt";
	options[14] = "EndOfBasis";
	options[15] = "NoEcp";
	WriteQuantumInput writeInp_(options);
	vector<CoordXYZ> mol(3);
	mol[0].atomlabel = "Li";
	mol[1].atomlabel = "Li";
	mol[2].atomlabel = "Li";
	mol[0].x = 0;
	mol[0].y = 0;
	mol[0].z = 0;
	mol[1].x = 1;
	mol[1].y = 0;
	mol[1].z = 0;
	mol[2].x = 2;
	mol[2].y = 0;
	mol[2].z = 0;
	writeInp_.createInput(mol,5);



	cout << "tlu tlu tlu tlu " << endl;
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


