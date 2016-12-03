#ifndef FITNESS_H
#define FITNESS_H

#include <vector>
#include <string>

class Fitness
{
public:
	Fitness::Fitness();
	Fitness::~Fitness();

	double fit(std::vector<double> &point, int type);

private:
	double lennardJones(std::vector<double> &x);

	double runGamess(
		std::vector<double> &x, 
		std::vector< std::string > &options, 
		std::string gamessPath,
		std::string nProc);

};

#endif
