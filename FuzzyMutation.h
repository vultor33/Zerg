#ifndef FUZZYMUTATION_H
#define FUZZYMUTATION_H

//#include <fl/Headers.h>
#include <vector>

namespace zerg{
class FuzzyMutation
{
public:
	FuzzyMutation(){};
	~FuzzyMutation();

	void startFuzzyMutation(const std::vector<double> &random_individual_range_min,
	                        const std::vector<double> &random_individual_range_max);

	void adminFuzzyStep(double methodMean);

	void makeMutation(int target, int parent, std::vector<std::vector<double>> &x_vec);

private:
	void setFuzzyRules();
	/*
	fl::Engine * engine;
	fl::InputVariable* inputVariable1;
	fl::InputVariable* inputVariable2;
	fl::OutputVariable* outputVariable1;
	fl::RuleBlock* ruleBlock;
	*/

	std::vector<double> referenceVariation;
	double meanFuzzyStep;
	
};

}

#endif

