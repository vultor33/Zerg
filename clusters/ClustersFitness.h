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
		std::string gamessScr_in,
		std::string nProc_in);

	~ClustersFitness();

	void local_optimization(int ind_i);

	void printAllIndividualas(std::string fileName);

private:
	void optimize(int ind_i);

	std::vector< std::string > options;
	std::string gamessPath;
	std::string gamessScr;
	std::string nProc;


};

#endif

