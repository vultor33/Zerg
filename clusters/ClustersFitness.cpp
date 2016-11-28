#include "ClustersFitness.h"

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
}

void ClustersFitness::optimize(int ind_i)
{
	// you have:
	// x_vec[ind_i] is a vector<double> -> to be optimized
	// I want:
	// energy[ind_i] -> fitness function 
	//test function
	int size = energy.size();
	double auxsoma = 0.0e0;
	for(int j=0; j<n_parameters; j++)
	{
		auxsoma += x_vec[ind_i][j]*x_vec[ind_i][j];
	}
	energy[ind_i] = auxsoma;
}