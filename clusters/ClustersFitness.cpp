#include "ClustersFitness.h"

#include "Fitness.h"
#include "../StructOptions.h"

#include <iostream>

using namespace std;
using namespace zerg;

ClustersFitness::ClustersFitness(
	GaParameters & gaParam,
	std::vector< std::string > &options_in,
	std::string gamessPath_in,
	std::string gamessScr_in,
	std::string nProc_in)
:ClustersOperators(gaParam.pop_size, gaParam.numberOfParameters)
{
	startClustersOperators(gaParam);

	gamessPath = gamessPath_in;
	gamessScr = gamessScr_in;
	nProc = nProc_in;

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
	if (options.size() == 0)
		energy[ind_i] = fit_.fit(x_vec[ind_i], 0);
	else
		energy[ind_i] = fit_.runGamess(
			x_vec[ind_i], 
			options, 
			gamessPath, 
			gamessScr,
			nProc);

}
