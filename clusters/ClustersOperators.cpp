#include "ClustersOperators.h"

#include <cmath>
#include <iostream>
#include <algorithm>

#include "../AuxMathGa.h"
#include "../AuxMath.h"
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
	nAtoms = gaParam.numberOfParameters / 3;
	gamma = gaParam.gammaInitializeAtoms;
	rca = gaParam.rcaInitializeAtoms;
	maxDistance = gaParam.maxDistance;
	adminLargeEnergyVariation = gaParam.adminLargeEnergyVariation;
	mutationValue = gaParam.mutationValue / nAtoms;
	crossoverWeight = gaParam.crossoverWeight;
	crossoverProbability = gaParam.corssoverProbability;
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
		if (operatorPerformance[0] > adminLargeEnergyVariation)
			mutationValue = AuxMathGa::randomNumber(0.005,2.0) / nAtoms;
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
	if (auxDistances[auxDistances.size() - 1] > maxDistance)
	{
		energy[ind_i] += 1.0e99;
	}
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

std::vector<double> ClustersOperators::calcAndSortDistancesOverI(std::vector<double> &x, int i)
{
	vector<double> auxDistances;
	for (int j = 0; j < nAtoms; j++)
	{
		if (i == j)
			continue;
		double dist = init_.calcDist(x, i, j);
		auxDistances.push_back(dist);
	}
	sort(auxDistances.begin(), auxDistances.end());
	return auxDistances;
}

std::vector<double> ClustersOperators::calcDistanceToCenter(vector<double> &x)
{
	vector<double> auxDistances(nAtoms);
	for (int i = 0; i < nAtoms; i++)
	{
		auxDistances[i] = sqrt(
			x[i] * x[i] + 
			x[i + nAtoms] * x[i + nAtoms] + 
			x[i + 2 * nAtoms] * x[i + 2 * nAtoms]);
	}
	return auxDistances;
}

double ClustersOperators::calcDistancesOverIAndGetMin(vector<double> &x, int i)
{
	vector<double> auxDistances;
	for (int j = 0; j < nAtoms; j++)
	{
		if (i == j)
			continue;
		double dist = init_.calcDist(x, i, j);
		auxDistances.push_back(dist);
	}
	return *min_element(auxDistances.begin(),auxDistances.end());
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


void ClustersOperators::printAtomsVectorDouble(vector<double> & atoms, string testName)
{
	int natm = atoms.size() / 3;
	ofstream teste_;
	teste_.open(testName.c_str(), std::ofstream::out | std::ofstream::app);
	teste_ << natm << endl << "t" << endl;
	for (int i = 0; i < natm; i++)
	{
		teste_ << "H "
			<< atoms[i] << "  "
			<< atoms[i + natm] << "  "
			<< atoms[i + 2 * natm] << endl;
	}
	teste_.close();
}

vector<double> ClustersOperators::rondinaCartesianDisplacementOperator(vector<double> & x)
{
	//parameters - esse nMaxAtoms poderia ser fixo e tal, ou mudar ao longo da simulacao
	int nMaxAtoms = AuxMathGa::randomNumber(1, nAtoms);
	double S = 0.1e0;
	vector<double> newX = x;

	//	remove("CDO.xyz");
	//printAtomsVectorDouble(x, "CDO.xyz");

	vector<int> alreadyMoved;
	bool moved;
	do
	{
		int atom = AuxMathGa::randomNumber(0, nAtoms - 1);
		moved = false;
		for (int i = 0; i < alreadyMoved.size(); i++)
		{
			moved = alreadyMoved[i] == atom;
			if (moved)
				break;
		}
		if (moved)
			continue;
		alreadyMoved.push_back(atom);
		double multiplyFactor = S * calcDistancesOverIAndGetMin(newX, atom);
		newX[atom] += multiplyFactor * AuxMathGa::randomNumber(-1.0e0, 1.0e0);
		newX[atom + nAtoms] += multiplyFactor *  AuxMathGa::randomNumber(-1.0e0, 1.0e0);
		newX[atom + 2 * nAtoms] += multiplyFactor * AuxMathGa::randomNumber(-1.0e0, 1.0e0);
		nMaxAtoms--;
	} while (nMaxAtoms != 0);

	//printAtomsVectorDouble(newX, "CDO.xyz");

	return newX;
}

vector<double> ClustersOperators::rondinaGeometricCenterDisplacementOperator(vector<double> & x)
{
	//parameters - esse nMaxAtoms poderia ser fixo e tal, ou mudar ao longo da simulacao
	int nMaxAtoms = AuxMathGa::randomNumber(1, nAtoms);
	double alfaMin = 0.2;
	double alfaMax = 0.45;
	double w = 2;
	vector<double> newX = x;
	vector<double> rCenterDistances = calcDistanceToCenter(x);
	double rMax = *max_element(rCenterDistances.begin(), rCenterDistances.end());

//	remove("GCDO.xyz");
//	printAtomsVectorDouble(x, "GCDO.xyz");

	vector<int> alreadyMoved;
	bool moved;
	do
	{
		int atom = AuxMathGa::randomNumber(0, nAtoms - 1);
		moved = false;
		for (int i = 0; i < alreadyMoved.size(); i++)
		{
			moved = alreadyMoved[i] == atom;
			if (moved)
				break;
		}
		if (moved)
			continue;
		alreadyMoved.push_back(atom);

		vector<double> unitSphericalVector = AuxMathGa::unitarySphericalVector();

		double multiplyFactor = (
			(alfaMax - alfaMin) * 
			pow(rCenterDistances[atom] / rMax, w) + alfaMin) 
			* calcDistancesOverIAndGetMin(newX,atom);

		newX[atom] += multiplyFactor * unitSphericalVector[0];
		newX[atom + nAtoms] += multiplyFactor * unitSphericalVector[1];
		newX[atom + 2 * nAtoms] += multiplyFactor * unitSphericalVector[2];
		nMaxAtoms--;
	} while (nMaxAtoms != 0);

//	printAtomsVectorDouble(newX, "GCDO.xyz");

	return newX;
}



vector<double> ClustersOperators::rondinaTwistOperator(vector<double> & x)
{
	AuxMath auxMath_;

	//parameters - esse nMaxAtoms poderia ser fixo e tal, ou mudar ao longo da simulacao
	double tetaMin = auxMath_._pi / 6;
	double tetaMax = auxMath_._pi;
	
	double teta = AuxMathGa::randomNumber(tetaMin, tetaMax);

	vector<double> newX = x;

	vector<double> unitSphericalVector = AuxMathGa::unitarySphericalVector();

//	remove("TWISTO.xyz");

//	printAtomsVectorDouble(x, "TWISTO.xyz");

	vector<bool> activateTwist(nAtoms);
	for (int i = 0; i < nAtoms; i++)
		activateTwist[i] = (
			newX[i] * unitSphericalVector[0] +
			newX[i + nAtoms] * unitSphericalVector[1] +
			newX[i + 2 * nAtoms] * unitSphericalVector[2]
			) > 0;

	vector< vector< double > > mRot = auxMath_.rotationMatrix(unitSphericalVector[0], unitSphericalVector[1], unitSphericalVector[2],teta);

	for (int i = 0; i < nAtoms; i++)
	{
		if (activateTwist[i])
		{
			vector<double> newCoordinates =
				auxMath_.matrixXVector(
					mRot, 
					x[i],
					x[i + nAtoms], 
					x[i + 2 * nAtoms]);
			newX[i] = newCoordinates[0];
			newX[i + nAtoms] = newCoordinates[1];
			newX[i + 2 * nAtoms] = newCoordinates[2];
		}
	}

//	printAtomsVectorDouble(newX, "TWISTO.xyz");

	return newX;
}












