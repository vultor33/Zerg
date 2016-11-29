#ifndef CLUSTERSFITNESS_H
#define CLUSTERSFITNESS_H

#include "ClustersOperators.h"
#include "../StructOptions.h"

class ClustersFitness : public ClustersOperators
{
public:
	ClustersFitness(zerg::GaParameters & gaParam);

	~ClustersFitness();

	void local_optimization(int ind_i);

private:
	void optimize(int ind_i);
};

#endif

