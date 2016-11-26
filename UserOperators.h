#ifndef USEROPERATORS_H
#define USEROPERATORS_H

#include <vector>

#include "BasicOperators.h"

class UserOperators : public zerg::BasicOperators
{
public:
	UserOperators(int pop_size, int number_parameters);
	~UserOperators();

	void startUserOperators();//final adjustments


	bool create_individual(int creation_type, int target, int parent1, int parent2);
	bool operatorAdministration(int method, const std::vector<double> &operatorPerformance);// modify operators
};

#endif