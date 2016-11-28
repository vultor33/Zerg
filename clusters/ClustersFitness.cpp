#include "ClustersFitness.h"

#include "Fitness.h"

#include <iostream>

using namespace std;
using namespace zerg;

ClustersFitness::ClustersFitness(int pop_size, int number_parameters)
:ClustersOperators(pop_size, number_parameters)
{
	// set acceptable creation range - required
	for(int j = 0; j<number_parameters; j++)
	{
		random_individual_range_min[j] = -1.e0;
		random_individual_range_max[j] = 1.0e0;
	}


	// ATENCAO  - ATIVAR O CHECK SIMILARITY AQUI
	// starting first population - default rules
	bool aux;
	for(int i=0; i<pop_size; i++)
	{
		aux = create_individual(0,i,0,0); //method 0 always random
		local_optimization(i);
	}

	startUserOperators();
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
