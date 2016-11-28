#ifndef FITNESS_H
#define FITNESS_H

#include <vector>

class Fitness
{
public:
	Fitness::Fitness();
	Fitness::~Fitness();

	double fit(std::vector<double> &point, int type);

private:
	double lennardJones(std::vector<double> &x);

};

#endif
