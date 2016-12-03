#ifndef CLUSTERSFITNESS_H
#define CLUSTERSFITNESS_H

#include <vector>
#include <string>

#include "ClustersOperators.h"
#include "../StructOptions.h"

class ClustersFitness : public ClustersOperators
{
public:
	ClustersFitness(
		zerg::GaParameters & gaParam, 
		std::vector< std::string > &options_in,
		std::string gamessPath_in,
		std::string nPrco_in);

	~ClustersFitness();

	void local_optimization(int ind_i);

private:
	void optimize(int ind_i);

	std::vector< std::string > options;
	std::string gamessPath;
	std::string nProc;


};

#endif

