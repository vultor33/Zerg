#include <iostream>
#include <math.h>
#include <fstream>
#include <string>

#include "BasicOperators.h"
#include "GeneticAlgorithm.h"
#include "Hooklaw.h"
#include "UserFitness.h"

using namespace std;
using namespace zerg;

/*
impotante: opcao para desativar variacoes e regular o que e um valor grande:
double FuzzyAdministration::getCreateRateVariation

estudo das formas de gerar clusters iniciais:
- Tem o de sao carlos, esfera, cubo e árvore.


*/
//ele checa similaridade so 30 vezes, se
// nao passar vai igual mesmo.
int main()
{
	int pop_size = 32; //multiplo de 4 e ponto final
	int n_param = 3;
	int maxGeneration = 5;
	UserFitness hk(pop_size, n_param);

	GeneticAlgorithm ga1(hk,pop_size);
	ga1.ga_start(maxGeneration);

	cout << "tlu tlu tlu tlu " << endl;
	return 0;
}
