#include "ClustersOperators.h"

#include <iostream>
#include <algorithm>

#include "../AuxMathGa.h"

using namespace std;
using namespace zerg;

ClustersOperators::ClustersOperators(int pop_size, int number_parameters)
:BasicOperators(pop_size, number_parameters)
{
//	number_of_creation_methods = 6;
//	tol_similarity = 1.0e-2;

}

ClustersOperators::~ClustersOperators(){}

void ClustersOperators::startUserOperators()
{
	mutationValue = 0.1e0;
	crossoverWeight = 0.7e0;
	crossoverProbability = 0.7e0;

	//definir gamma, rca, adminLargeEnergyVariation

}

bool ClustersOperators::create_individual(int creation_type,int target, int parent1, int parent2)
{
	switch(creation_type)
	{
	case 0:
		x_vec[target] = init_.generateCluster(nAtoms, gamma, rca);
		break;

	case 1:
		make_crossover_mean(target,parent1,parent2);
		break;

	case 2:
		make_crossover_2_points(target, parent1, parent2);
		break;

	case 3:
		make_mutation(target, parent1);
		break;

	case 4:
		make_crossover_probability(target, parent1, parent2);
		break;

	default:
		cout << "Creation type not avaible" << endl;
		return false;
	}
	return true;
}

bool ClustersOperators::operatorAdministration(int method, const std::vector<double> &operatorPerformance)
{
	// if it has an administrator, pass to him.
	switch(method)
	{
	case 0:
		break;
	case 1:
		if(operatorPerformance[0] > adminLargeEnergyVariation)
			crossoverWeight = AuxMathGa::randomNumber(0.5e0,0.9e0);
		break;
	case 2:
		break;
	case 3:
		if(operatorPerformance[0] > adminLargeEnergyVariation)
			mutationValue = AuxMathGa::randomNumber(0.05e0,0.3e0);
		break;
	case 4:
		if(operatorPerformance[0] > adminLargeEnergyVariation)
			crossoverWeight = AuxMathGa::randomNumber(0.5e0,0.9e0);
		break;
	default:
		cout << " administration of this operator undefined - contact developers " << endl;
		exit(2);
	}
	return true;
}



/////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////
/// SIMILARITY //////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////


void ClustersOperators::appendTosimilarity(int ind_i)
{
	vector<double> auxDistances = calcAndSortAllDistances(x_vec[ind_i]);
	allDistances.push_back(auxDistances);
}

std::vector<double> ClustersOperators::calcAndSortAllDistances(std::vector<double> &x)
{
	vector<double> auxDistances;
	for (int i = 0; i < (nAtoms - 1); i++)
	{
		for (int j = (i + 1); j < nAtoms; j++)
		{
			double dist = init_.calcDist(x_vec[i], i, j);
			auxDistances.push_back(dist);
		}
	}
	sort(auxDistances.begin(), auxDistances.end());
	return auxDistances;
}

bool ClustersOperators::check_similarity(int target)
{
	vector<double> auxDistance = calcAndSortAllDistances(x_vec[target]);

	if (auxDistance[auxDistance.size() - 1] > maxDistance)
		return true;

	int size = auxDistance.size();
	for (size_t i = 0; i < allDistances.size(); i++)
	{
		double distanceDiffererence = 0.e0;
		for (int k = 0; k < size; k++)
			distanceDiffererence += abs(auxDistance[k] - allDistances[i][k]);

		distanceDiffererence /= (double)size;
		if (distanceDiffererence < distanceDiffererenceThreshold)
			return true;
	}
	return false;
}




/////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////
/// OPERATORS DEFINITONS ////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////

