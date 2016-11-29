#include "ClustersOperators.h"

#include <iostream>
#include <algorithm>

#include "../AuxMathGa.h"
#include "../StructOptions.h"

using namespace std;
using namespace zerg;

ClustersOperators::ClustersOperators(int pop_size, int number_parameters)
:BasicOperators(pop_size, number_parameters)
{
	number_of_creation_methods = 6;
	tol_similarity = 3.0e-2;
}

ClustersOperators::~ClustersOperators(){}

void ClustersOperators::startClustersOperators(GaParameters & gaParam)
{
	mutationValue = gaParam.mutationValue;
	crossoverWeight = gaParam.crossoverWeight;
	crossoverProbability = gaParam.corssoverProbability;	
	nAtoms = gaParam.numberOfParameters / 3;
	gamma = gaParam.gammaInitializeAtoms;
	rca = gaParam.rcaInitializeAtoms;
	maxDistance = gaParam.maxDistance;
	adminLargeEnergyVariation = gaParam.adminLargeEnergyVariation;
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

	case 5:
		if (!sphereCutAndSplice(target, parent1, parent2))
			make_mutation(target, parent1);
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

	case 5:
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
			double dist = init_.calcDist(x, i, j);
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
		if (distanceDiffererence < tol_similarity)
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

bool ClustersOperators::sphereCutAndSplice(int target, int parent1, int parent2)
{
	vector<double> x1 = x_vec[parent1];
	vector<double> x2 = x_vec[parent2];
	translateToGeometricCenter(x1);
	translateToGeometricCenter(x2);
	vector<double> r1 = calculateRadius(x1);
	vector<double> r2 = calculateRadius(x2);

	int natm = r1.size();

	vector<int> atomPositions1(natm);
	for (int i = 0; i < natm; i++)
		atomPositions1[i] = i;
	vector<int> atomPositions2 = atomPositions1;

	vector<int> order1 = AuxMathGa::vector_ordering(r1);

	vector<int> order2 = AuxMathGa::vector_ordering(r2);

	//find M (check: A sphere-cut-splice crossover for the evolution of cluster structures)
	int M = -1;
	for (int i = 0; i < (natm - 1); i++)
	{
		if ((r1[i] < r2[i + 1]) && (r2[i] < r1[i + 1]))
			M++;
		else
			break;
	}

	if (M == -1)
		return false;

	AuxMathGa::vector_ordering_with_instructions(atomPositions1, order1);
	AuxMathGa::vector_ordering_with_instructions(atomPositions2, order2);

	int m = AuxMathGa::randomNumber(0, M);
	//change to target
	int takeAtomI;
	for (int i = 0; i < natm; i++)
	{
		if (i <= m)
		{
			takeAtomI = atomPositions1[i];
			x_vec[target][i] = x1[takeAtomI];
			x_vec[target][i + natm] = x1[takeAtomI + natm];
			x_vec[target][i + 2 * natm] = x1[takeAtomI + 2 * natm];
		}
		else
		{
			takeAtomI = atomPositions2[i];
			x_vec[target][i] = x2[takeAtomI];
			x_vec[target][i + natm] = x2[takeAtomI + natm];
			x_vec[target][i + 2 * natm] = x2[takeAtomI + 2 * natm];
		}
	}
	return true;
}

void ClustersOperators::translateToGeometricCenter(vector<double> & x)
{
	int natm = x.size() / 3;
	double xcm = 0.0e0;
	double ycm = 0.0e0;
	double zcm = 0.0e0;
	for (int i = 0; i < natm; i++)
	{
		xcm += x[i];
		ycm += x[i + natm];
		zcm += x[i + 2 * natm];
	}
	xcm /= natm;
	ycm /= natm;
	zcm /= natm;
	for (int i = 0; i < natm; i++)
	{
		x[i] -= xcm;
		x[i + natm] -= ycm;
		x[i + 2 * natm] -= zcm;
	}
}

vector<double> ClustersOperators::calculateRadius(vector<double> &x)
{
	int natm = x.size() / 3;
	vector<double> radius(natm);
	for (int i = 0; i < natm; i++)
		radius[i] = sqrt(x[i] * x[i] +
			x[i + natm] * x[i + natm] +
			x[i + 2 * natm] * x[i + 2 * natm]);

	return radius;
}


