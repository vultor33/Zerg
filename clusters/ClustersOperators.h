#ifndef CLUSTERSOPERATORS_H
#define CLUSTERSOPERATORS_H

#include <vector>

#include "InitializeAtoms.h"
#include "../BasicOperators.h"

class ClustersOperators : public zerg::BasicOperators
{
public:
	ClustersOperators(int pop_size, int number_parameters);
	~ClustersOperators();

	//reimplement or use basic
	void startUserOperators();//final adjustments  (like crossover probability)

	bool create_individual(int creation_type, int target, int parent1, int parent2);
	bool operatorAdministration(int method, const std::vector<double> &operatorPerformance);// modify operators with it's performance
	bool check_similarity(int target);

	//keep
	virtual void local_optimization(int ind_i) = 0;

protected:
	//start this information here


private:
	InitializeAtoms init_;

	int nAtoms;
	double gamma;
	double rca;
	double adminLargeEnergyVariation;
	double maxDistance;

	void appendTosimilarity(int ind_i);

	std::vector<double> calcAndSortAllDistances(std::vector<double> &x);

	std::vector< std::vector<double> > allDistances;

};

#endif