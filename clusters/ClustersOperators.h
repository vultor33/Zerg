#ifndef CLUSTERSOPERATORS_H
#define CLUSTERSOPERATORS_H

#include <vector>

#include "InitializeAtoms.h"
#include "../BasicOperators.h"
#include "../StructOptions.h"

class ClustersOperators : public zerg::BasicOperators
{
public:
	ClustersOperators(int pop_size, int number_parameters);
	~ClustersOperators();

	//reimplement or use basic
	void startClustersOperators(zerg::GaParameters & gaParam);//final adjustments  (like crossover probability)

	bool create_individual(int creation_type, int target, int parent1, int parent2);
	bool operatorAdministration(int method, const std::vector<double> &operatorPerformance);// modify operators with it's performance
	bool check_similarity(int target);

	//keep
	virtual void local_optimization(int ind_i) = 0;

protected:
	void appendTosimilarity(int ind_i);

private:
	//set on startUserOperators()
	int nAtoms;
	double gamma;
	double rca;
	double adminLargeEnergyVariation;
	double maxDistance;
	double distanceDiffererenceThreshold;

	bool sphereCutAndSplice(int target, int parent1, int parent2);
	void translateToGeometricCenter(std::vector<double> & x);
	std::vector<double> calculateRadius(std::vector<double> &x);


	std::vector<double> calcAndSortAllDistances(std::vector<double> &x);

	std::vector< std::vector<double> > allDistances;

	InitializeAtoms init_;

};

#endif