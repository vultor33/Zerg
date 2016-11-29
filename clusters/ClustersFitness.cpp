#include "ClustersFitness.h"

#include "Fitness.h"
#include "../StructOptions.h"

#include <iostream>

using namespace std;
using namespace zerg;

ClustersFitness::ClustersFitness(int pop_size, int number_parameters, GaParameters & gaParam)
:ClustersOperators(pop_size, number_parameters)
{

	// ATENCAO  - ATIVAR O CHECK SIMILARITY AQUI
	// starting first population - default rules
	bool aux;
	aux = create_individual(0, 0, 0, 0); //method 0 always random
	for(int i=1; i<pop_size; i++)
	{
		for (int i = 0; i < gaParam.insistOnSimilar; i++)
		{
			aux = create_individual(0, i, 0, 0); //method 0 always random
			if (!check_similarity(i))
				break;
		}		
		local_optimization(i);
	}

	startClustersOperators(gaParam);
}

ClustersFitness::~ClustersFitness(){}


void ClustersFitness::local_optimization(int ind_i)
{
	optimize(ind_i);
	appendTosimilarity(ind_i);
}

void ClustersFitness::optimize(int ind_i)
{
	// you have:
	// x_vec[ind_i] is a vector<double> -> to be optimized
	// I want:
	// energy[ind_i] -> fitness function 

	Fitness fit_;
	energy[ind_i] = fit_.fit(x_vec[ind_i], 0);
}
