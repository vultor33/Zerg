#ifndef FUZZYGA_H
#define FUZZYGA_H

//#include <fl/Headers.h>

namespace zerg{
class FuzzyAdministration
{
public:
	FuzzyAdministration(){};
	~FuzzyAdministration();

	void setFuzzyRules();
	double getCreateRateVariation(double methodMean);


private:
	/*
	fl::Engine * engine;
	fl::InputVariable* fitnessVariation;
	fl::OutputVariable* creationRateControl;
	fl::RuleBlock* ruleBlock;
	*/
};

}

#endif