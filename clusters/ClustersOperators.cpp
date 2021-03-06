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
	number_of_creation_methods = 7;
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
		if (!sphereCutAndSplice(target, parent1, parent2))
			make_mutation(target, parent1);
		break;

	case 2:
		x_vec[target] = rondinaGeometricCenterDisplacementOperator(x_vec[parent1]);
		break;

	case 3:
		x_vec[target] = rondinaTwistOperator(x_vec[parent1]);
		break;

	case 4:
		x_vec[target] = deavenHoCutSplice(x_vec[parent1], x_vec[parent2]);
		break;

	case 5:
		x_vec[target] = rondinaAngularOperator(x_vec[parent1]);
		break;

	case 6:
		x_vec[target] = rondinaAngularSurfaceOperator(x_vec[parent1]);
		break;

	case 7:
		make_crossover_mean(target,parent1,parent2);
		break;

	case 8:
		make_crossover_2_points(target, parent1, parent2);
		break;

	case 9:
		make_mutation(target, parent1);
		break;

	case 10:
		make_crossover_probability(target, parent1, parent2);
		break;

	case 11:
		x_vec[target] = rondinaCartesianDisplacementOperator(x_vec[parent1]);
		break;

	case 12:
		x_vec[target] = rondinaMoveToCenterOperator(x_vec[parent1]);
		break;

	case 13:
		x_vec[target] = fredAngularSurfaceOperator(x_vec[parent1]);
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
		break;
		//if(operatorPerformance[0] > adminLargeEnergyVariation)
		//crossoverWeight = AuxMathGa::randomNumber(0.5e0,0.9e0);

	case 2:
		break;

	case 3:
		break;
//		if (operatorPerformance[0] > adminLargeEnergyVariation)
//      mutationValue = AuxMathGa::randomNumber(0.005,2.0) / nAtoms;

	case 4:
		break;
//		if(operatorPerformance[0] > adminLargeEnergyVariation)
//			crossoverWeight = AuxMathGa::randomNumber(0.5e0,0.9e0);

	case 5:
		break;

	case 6:
	case 7:
	case 8:
	case 9:
	case 10:
	case 11:
	case 12:
	case 13:
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

void ClustersOperators::translate(vector<double> & x, vector<double> & translateVector)
{
	int sizeCluster = x.size() / 3;
	for (int i = 0; i < sizeCluster; i++)
	{
		x[i] += translateVector[0];
		x[i + sizeCluster] += translateVector[1];
		x[i + 2 * sizeCluster] += translateVector[2];
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

vector<double> ClustersOperators::rondinaCartesianDisplacementOperator(const vector<double> & x)
{
	//parameters - esse nMaxAtoms poderia ser fixo e tal, ou mudar ao longo da simulacao
	int nMaxAtoms = AuxMathGa::randomNumber(1, nAtoms);
	vector<double> newX = x;

//	remove("CDO.xyz");
//  printAtomsVectorDouble(x, "CDO.xyz");

	vector<int> alreadyMoved;
	bool moved;
	do
	{
		int atom = AuxMathGa::randomNumber(0, nAtoms - 1);
		moved = false;
		for (size_t i = 0; i < alreadyMoved.size(); i++)
		{
			moved = alreadyMoved[i] == atom;
			if (moved)
				break;
		}
		if (moved)
			continue;
		alreadyMoved.push_back(atom);
		double multiplyFactor = scdo * calcDistancesOverIAndGetMin(newX, atom);
		newX[atom] += multiplyFactor * AuxMathGa::randomNumber(-1.0e0, 1.0e0);
		newX[atom + nAtoms] += multiplyFactor *  AuxMathGa::randomNumber(-1.0e0, 1.0e0);
		newX[atom + 2 * nAtoms] += multiplyFactor * AuxMathGa::randomNumber(-1.0e0, 1.0e0);
		nMaxAtoms--;
	} while (nMaxAtoms != 0);

	//printAtomsVectorDouble(newX, "CDO.xyz");

	return newX;
}

vector<double> ClustersOperators::rondinaGeometricCenterDisplacementOperator(const vector<double> & x)
{
	//parameters - esse nMaxAtoms poderia ser fixo e tal, ou mudar ao longo da simulacao
	int nMaxAtoms = AuxMathGa::randomNumber(1, nAtoms);
	vector<double> newX = x;
	vector<double> rCenterDistances = calcDistanceToCenter(newX);
	double rMax = *max_element(rCenterDistances.begin(), rCenterDistances.end());

//	remove("GCDO.xyz");
//	printAtomsVectorDouble(x, "GCDO.xyz");

	vector<int> alreadyMoved;
	bool moved;
	do
	{
		int atom = AuxMathGa::randomNumber(0, nAtoms - 1);
		moved = false;
		for (size_t i = 0; i < alreadyMoved.size(); i++)
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
			(alfaMaxGcdo - alfaMinGcdo) * 
			pow(rCenterDistances[atom] / rMax, wGcdo) + alfaMinGcdo) 
			* calcDistancesOverIAndGetMin(newX,atom);

		newX[atom] += multiplyFactor * unitSphericalVector[0];
		newX[atom + nAtoms] += multiplyFactor * unitSphericalVector[1];
		newX[atom + 2 * nAtoms] += multiplyFactor * unitSphericalVector[2];
		nMaxAtoms--;
	} while (nMaxAtoms != 0);

//	printAtomsVectorDouble(newX, "GCDO.xyz");

	return newX;
}



vector<double> ClustersOperators::rondinaTwistOperator(const vector<double> & x)
{
	AuxMath auxMath_;
	
	double teta = AuxMathGa::randomNumber(tetaMinTwisto, tetaMaxTwisto);

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


vector<double> ClustersOperators::rondinaAngularOperator(const vector<double> & x)
{
	//parameters - esse nMaxAtoms poderia ser fixo e tal, ou mudar ao longo da simulacao
	// IGUAL O GEOMETRIC CENTER DISPLACEMENT so que mais forte e com menos regras
	int nMaxAtoms = AuxMathGa::randomNumber(1, nAtoms);

	vector<double> newX = x;

//	remove("ADO.xyz");
//	printAtomsVectorDouble(x, "ADO.xyz");

	vector<int> alreadyMoved;
	bool moved;
	do
	{
		int atom = AuxMathGa::randomNumber(0, nAtoms - 1);
		moved = false;
		for (size_t i = 0; i < alreadyMoved.size(); i++)
		{
			moved = alreadyMoved[i] == atom;
			if (moved)
				break;
		}
		if (moved)
			continue;
		alreadyMoved.push_back(atom);

		vector<double> unitSphericalVector = AuxMathGa::unitarySphericalVector();

		double atomRadius = sqrt(
			x[atom] * x[atom] +
			x[atom + nAtoms] * x[atom + nAtoms] +
			x[atom + 2 * nAtoms] * x[atom + 2 * nAtoms]);

		newX[atom] = atomRadius * unitSphericalVector[0];
		newX[atom + nAtoms] = atomRadius * unitSphericalVector[1];
		newX[atom + 2 * nAtoms] = atomRadius * unitSphericalVector[2];
		nMaxAtoms--;
	} while (nMaxAtoms != 0);

//	printAtomsVectorDouble(newX, "ADO.xyz");

	return newX;
}


vector<double> ClustersOperators::rondinaAngularSurfaceOperator(const vector<double> & x)
{
	//parameters - esse nMaxAtoms poderia ser fixo e tal, ou mudar ao longo da simulacao
	int nMaxAtoms = AuxMathGa::randomNumber(1, nAtoms);

	vector<double> newX = x;

//	remove("ASDO.xyz");
//	printAtomsVectorDouble(x, "ASDO.xyz");

	vector<double> rCenterDistances = calcDistanceToCenter(newX);
	double rMax = *max_element(rCenterDistances.begin(), rCenterDistances.end());

	vector<int> alreadyMoved;
	bool moved;
	do
	{
		int atom = AuxMathGa::randomNumber(0, nAtoms - 1);
		moved = false;
		for (size_t i = 0; i < alreadyMoved.size(); i++)
		{
			moved = alreadyMoved[i] == atom;
			if (moved)
				break;
		}
		if (moved)
			continue;
		alreadyMoved.push_back(atom);

		vector<double> unitSphericalVector = AuxMathGa::unitarySphericalVector();

		newX[atom] = rMax * unitSphericalVector[0];
		newX[atom + nAtoms] = rMax * unitSphericalVector[1];
		newX[atom + 2 * nAtoms] = rMax * unitSphericalVector[2];
		nMaxAtoms--;
	} while (nMaxAtoms != 0);

//	printAtomsVectorDouble(newX, "ASDO.xyz");

	return newX;
}

vector<double> ClustersOperators::fredAngularSurfaceOperator(const vector<double> & x)
{
	//parameters - esse nMaxAtoms poderia ser fixo e tal, ou mudar ao longo da simulacao
	int nMaxAtoms = AuxMathGa::randomNumber(1, nAtoms);
	// atencao --- para convergir para o RONDINA o moveToSurfaceFactor
	// tem que ser sempre igual a um.

	vector<double> newX = x;

//	remove("FASDO.xyz");
//	printAtomsVectorDouble(x, "FASDO.xyz");

	vector<double> rCenterDistances = calcDistanceToCenter(newX);
	double rMax = *max_element(rCenterDistances.begin(), rCenterDistances.end());

	vector<int> alreadyMoved;
	bool moved;
	do
	{
		int atom = AuxMathGa::randomNumber(0, nAtoms - 1);
		moved = false;
		for (size_t i = 0; i < alreadyMoved.size(); i++)
		{
			moved = alreadyMoved[i] == atom;
			if (moved)
				break;
		}
		if (moved)
			continue;
		alreadyMoved.push_back(atom);

		vector<double> unitSphericalVector = AuxMathGa::unitarySphericalVector();

		double moveToSurfaceFactor = rCenterDistances[atom] +
			(rMax - rCenterDistances[atom]) * AuxMathGa::randomNumber(0.0e0, 1.0e0);

		newX[atom] = moveToSurfaceFactor * unitSphericalVector[0];
		newX[atom + nAtoms] = moveToSurfaceFactor * unitSphericalVector[1];
		newX[atom + 2 * nAtoms] = moveToSurfaceFactor * unitSphericalVector[2];
		nMaxAtoms--;
	} while (nMaxAtoms != 0);

//	printAtomsVectorDouble(newX, "FASDO.xyz");

	return newX;
}

vector<double> ClustersOperators::rondinaMoveToCenterOperator(const vector<double> & x)
{
//  parameters - esse nMaxAtoms poderia ser fixo e tal, ou mudar ao longo da simulacao
	int nMaxAtoms = AuxMathGa::randomNumber(1, nAtoms);

	vector<double> newX = x;

//	remove("MTCO.xyz");
//	printAtomsVectorDouble(x, "MTCO.xyz");

	vector<int> alreadyMoved;
	bool moved;
	do
	{
		int atom = AuxMathGa::randomNumber(0, nAtoms - 1);
		moved = false;
		for (size_t i = 0; i < alreadyMoved.size(); i++)
		{
			moved = alreadyMoved[i] == atom;
			if (moved)
				break;
		}
		if (moved)
			continue;
		alreadyMoved.push_back(atom);

		double atomRadius = sqrt(
			x[atom] * x[atom] +
			x[atom + nAtoms] * x[atom + nAtoms] +
			x[atom + 2 * nAtoms] * x[atom + 2 * nAtoms]);

		double moveFactor = AuxMathGa::randomNumber(
			contractionMinMtco * atomRadius, 
			contractionMaxMtco * atomRadius);

		newX[atom] *= moveFactor;
		newX[atom + nAtoms] = moveFactor;
		newX[atom + 2 * nAtoms] = moveFactor;
		nMaxAtoms--;
	} while (nMaxAtoms != 0);

//	printAtomsVectorDouble(newX, "MTCO.xyz");

	return newX;
}

vector<double> ClustersOperators::deavenHoCutSplice(
	const vector<double> & x1_parent, 
	const vector<double> & x2_parent)
{
	vector<double> x1 = x1_parent;
	vector<double> x2 = x2_parent;

//	remove("DHO.xyz");
//	printAtomsVectorDouble(x1, "DHO.xyz");
//	printAtomsVectorDouble(x2, "DHO.xyz");

	vector<int> cutAtoms1, cutAtoms2;
	vector<double> unit = AuxMathGa::unitarySphericalVector();
	vector<double> rDistance1, rDistance2;
	double d1, d2;
	for (int i = 0; i < nAtoms; i++)
	{
		double prodInt1 =
			unit[0] * x1[i] +
			unit[1] * x1[i + nAtoms] +
			unit[2] * x1[i + 2 * nAtoms];
		double prodInt2 = 
			unit[0] * x2[i] + 
			unit[1] * x2[i + nAtoms] + 
			unit[2] * x2[i + 2 * nAtoms];
		if (prodInt1 > 0)
		{
			cutAtoms1.push_back(i);
			d1 = sqrt(
				x1[i] * x1[i] +
				x1[i + nAtoms] * x1[i + nAtoms] +
				x1[i + 2 * nAtoms] * x1[i + 2 * nAtoms]);			
			rDistance1.push_back(d1);
		}
		if (prodInt2 > 0)
		{
			cutAtoms2.push_back(i);
			d2 = sqrt(
				x2[i] * x2[i] +
				x2[i + nAtoms] * x2[i + nAtoms] +
				x2[i + 2 * nAtoms] * x2[i + 2 * nAtoms]);
			rDistance2.push_back(d2);
		}
	}
	vector<int> orderInstruct1 = AuxMathGa::vector_ordering(rDistance1);
	vector<int> orderInstruct2 = AuxMathGa::vector_ordering(rDistance2);
	AuxMathGa::vector_ordering_with_instructions(cutAtoms1, orderInstruct1);
	AuxMathGa::vector_ordering_with_instructions(cutAtoms2, orderInstruct2);
	double translateValue;
	vector<double> translateVector(3);
	while (cutAtoms1.size() != cutAtoms2.size())
	{
		if (cutAtoms1.size() > cutAtoms2.size())
		{
			translateValue = (rDistance1[0] + rDistance1[1]) / 2.0e0;
			translateVector[0] = -unit[0] * translateValue;
			translateVector[1] = -unit[1] * translateValue;
			translateVector[2] = -unit[2] * translateValue;
			translate(x1, translateVector);
			rDistance1.erase(rDistance1.begin(), rDistance1.begin() + 1);
			cutAtoms1.erase(cutAtoms1.begin(), cutAtoms1.begin() + 1);
		}
		else if (cutAtoms1.size() < cutAtoms2.size())
		{
			translateValue = (rDistance2[0] + rDistance2[1]) / 2.0e0;
			translateVector[0] = -unit[0] * translateValue;
			translateVector[1] = -unit[1] * translateValue;
			translateVector[2] = -unit[2] * translateValue;
			translate(x2, translateVector);
			rDistance2.erase(rDistance2.begin(), rDistance2.begin() + 1);
			cutAtoms2.erase(cutAtoms2.begin(), cutAtoms2.begin() + 1);
		}
	}
	for (size_t i = 0; i < cutAtoms1.size(); i++)
	{
		x1[cutAtoms1[i]] = x2[cutAtoms2[i]];
		x1[cutAtoms1[i] + nAtoms] = x2[cutAtoms2[i] + nAtoms];
		x1[cutAtoms1[i] + 2 * nAtoms] = x2[cutAtoms2[i] + 2 * nAtoms];
	}

//	printAtomsVectorDouble(x1, "DHO.xyz");

	return x1;
}



