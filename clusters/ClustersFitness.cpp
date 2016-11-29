#include "ClustersFitness.h"

#include "Fitness.h"
#include "../StructOptions.h"

#include <iostream>

using namespace std;
using namespace zerg;

ClustersFitness::ClustersFitness(GaParameters & gaParam)
:ClustersOperators(gaParam.pop_size, gaParam.numberOfParameters)
{

	// ATENCAO  - ATIVAR O CHECK SIMILARITY AQUI
	// starting first population - default rules
	startClustersOperators(gaParam);

	bool aux;
	aux = create_individual(0, 0, 0, 0); //method 0 always random
	local_optimization(0);
	for(int i=1; i<gaParam.pop_size; i++)
	{
		for (int k = 0; k < gaParam.insistOnSimilar; k++)
		{
			aux = create_individual(0, i, 0, 0); //method 0 always random
			if (!check_similarity(i))
				break;
		}		
		local_optimization(i);
	}

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
