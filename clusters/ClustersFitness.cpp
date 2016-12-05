#include "ClustersFitness.h"

#include "../AuxMathGa.h"
#include "Fitness.h"
#include "../StructOptions.h"

#include <iostream>
#include <fstream>
#include <iomanip>

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

void ClustersFitness::printAllIndividualas(string fileName)
{
	// ordering
	int pop_size = energy.size();
	vector<double> fitness_energies(pop_size);
	vector<int> fitness_rank(pop_size);
	for (int i = 0; i<pop_size; i++)
	{
		fitness_energies[i] = energy[i];
		fitness_rank[i] = i;
	}
	vector<int> vector_order = AuxMathGa::vector_ordering(fitness_energies);
	AuxMathGa::vector_ordering_with_instructions(fitness_rank, vector_order);

	ofstream printAll_(fileName.c_str());
	int nAtoms = x_vec[0].size() / 3;
	for (size_t ind = 0; ind < x_vec.size(); ind++)
	{
		int best = fitness_rank[ind];
		printAll_ << nAtoms << endl << setprecision(16) << energy[best] << endl;
		for (int i = 0; i < nAtoms; i++)
			printAll_ << "N "
				<< x_vec[best][i] << "  "
				<< x_vec[best][i + nAtoms] << "  "
				<< x_vec[best][i + 2 * nAtoms] << "  "
				<< endl;
	}
	printAll_.close();
}
