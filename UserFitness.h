#ifndef USERFITNESS_H
#define USERFITNESS_H

#include "UserOperators.h"

class UserFitness : public UserOperators
{
public:
	UserFitness(int pop_size, int number_parameters);
	~UserFitness();

private:
	void optimize(int ind_i);
};

#endif

