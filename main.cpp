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


//ele checa similaridade so 30 vezes, se
// nao passar vai igual mesmo.
int main()
{
	int pop_size = 32; //multiplo de 4 e ponto final
	int n_param = 3;
	UserFitness hk(pop_size, n_param);
	GeneticAlgorithm ga1(hk,pop_size);
	ga1.ga_start(300);

	cout << "tlu tlu tlu tlu " << endl;
	cin.get();
	return 0;
}
