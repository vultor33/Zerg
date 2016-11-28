#ifndef CLUSTERSFITNESS_H
#define CLUSTERSFITNESS_H

#include "ClustersOperators.h"

class ClustersFitness : public ClustersOperators
{
public:
	ClustersFitness(int pop_size, int number_parameters);

	~ClustersFitness();

	void local_optimization(int ind_i);

private:
	void optimize(int ind_i);
};

#endif

