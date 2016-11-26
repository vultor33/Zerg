#include "FuzzyMutation.h"

#include <iostream>
#include <fl/Headers.h>
#include "MATH.h"

using namespace std;
using namespace zerg;
using namespace fl;

namespace zerg{
void FuzzyMutation::startFuzzyMutation(const std::vector<double> &random_individual_range_min,
	                                   const std::vector<double> &random_individual_range_max)
{
	cout << "start fuzzy mut " << endl;

	int size = (int)random_individual_range_min.size();
	referenceVariation.resize(size);

	double meanRange;
	for(int i=0; i<size; i++)
	{
		meanRange = 0.5e0*(random_individual_range_min[i]+
			               random_individual_range_max[i]);
		referenceVariation[i] = abs(meanRange-random_individual_range_min[i]);
	}

	setFuzzyRules();
	meanFuzzyStep = 1.0e0;
}

FuzzyMutation::~FuzzyMutation()
{
	delete engine;
}

void FuzzyMutation::setFuzzyRules()
{
	engine = new Engine;
	engine->setName("FuzzyMutation");

	inputVariable1 = new InputVariable;
	inputVariable1->setEnabled(true);
	inputVariable1->setName("Variation");
	inputVariable1->setRange(0.000, 6.000);
	inputVariable1->addTerm(new Triangle("Magicarp", 0.000, 0.500, 0.750));
	inputVariable1->addTerm(new Triangle("Terrible", 0.500, 1.250, 1.750));
	inputVariable1->addTerm(new Triangle("Bad", 1.250, 1.750, 2.000));
	inputVariable1->addTerm(new Triangle("Zero", 1.750, 2.000, 2.250));//2 is zero
	inputVariable1->addTerm(new Triangle("Good", 2.000, 2.250, 2.750));
	inputVariable1->addTerm(new Triangle("Excelent", 2.250, 2.750, 3.250));
	inputVariable1->addTerm(new Triangle("Outstanding", 2.750, 3.250, 4.000));
	engine->addInputVariable(inputVariable1);

	inputVariable2 = new InputVariable;
	inputVariable2->setEnabled(true);
	inputVariable2->setName("Step"); //normalized mean step
	inputVariable2->setRange(0.000, 6.000);
	inputVariable2->addTerm(new Triangle("Insignificant", 0.000, 0.200, 0.400));
	inputVariable2->addTerm(new Triangle("VerySmall", 0.200, 0.400, 0.800));
	inputVariable2->addTerm(new Triangle("Small", 0.600, 0.800, 1.000));
	inputVariable2->addTerm(new Triangle("Same", 0.8000, 1.000, 1.200));
	inputVariable2->addTerm(new Triangle("Big", 1.000, 1.200, 1.400));
	inputVariable2->addTerm(new Triangle("VeryBig", 1.200, 1.400, 1.800));
	inputVariable2->addTerm(new Triangle("Colossal", 1.600, 1.800, 2.000));
	engine->addInputVariable(inputVariable2);

	outputVariable1 = new OutputVariable;
	outputVariable1->setEnabled(true);
	outputVariable1->setName("NewStep");
	outputVariable1->setRange(0.000, 4.000);
	outputVariable1->fuzzyOutput()->setAccumulation(new Maximum);
	outputVariable1->setDefuzzifier(new MeanOfMaximum(500));
	outputVariable1->setDefaultValue(12345);
	outputVariable1->setLockPreviousOutputValue(false);
	outputVariable1->setLockOutputValueInRange(false);
	outputVariable1->addTerm(new Triangle("toGreatDecrease", 0.4000, 0.6000, 0.800));
	outputVariable1->addTerm(new Triangle("toDecrease", 0.600, 0.800, 1.000));
	outputVariable1->addTerm(new Triangle("toKeep", 0.800, 1.000, 1.200)); //1 is zero
	outputVariable1->addTerm(new Triangle("toIncrease", 1.000, 1.200, 1.400));
	engine->addOutputVariable(outputVariable1);

	ruleBlock = new RuleBlock;
	ruleBlock->setEnabled(true);
	ruleBlock->setName("");
	ruleBlock->setConjunction(new Minimum);
	ruleBlock->setDisjunction(new Maximum);
	ruleBlock->setActivation(new Minimum);
	ruleBlock->addRule(fl::Rule::parse("if Variation is Magicarp then NewStep is toGreatDecrease", engine));
	ruleBlock->addRule(fl::Rule::parse("if Variation is Terrible and Step is Same then NewStep is toDecrease", engine));
	ruleBlock->addRule(fl::Rule::parse("if Variation is Terrible and Step is Small then NewStep is toDecrease", engine));
	ruleBlock->addRule(fl::Rule::parse("if Variation is Terrible and Step is Big then NewStep is toDecrease", engine));
	ruleBlock->addRule(fl::Rule::parse("if Variation is Terrible and Step is VeryBig then NewStep is toGreatDecrease", engine));
	ruleBlock->addRule(fl::Rule::parse("if Variation is Terrible and Step is Colossal then NewStep is toGreatDecrease", engine));
	ruleBlock->addRule(fl::Rule::parse("if Variation is Terrible and Step is VerySmall then NewStep is toKeep", engine));
	ruleBlock->addRule(fl::Rule::parse("if Variation is Terrible and Step is Insignificant then NewStep is toKeep", engine));
	ruleBlock->addRule(fl::Rule::parse("if Variation is Bad and Step is Same then NewStep is toDecrease", engine));
	ruleBlock->addRule(fl::Rule::parse("if Variation is Bad and Step is Big then NewStep is toDecrease", engine));
	ruleBlock->addRule(fl::Rule::parse("if Variation is Bad and Step is VeryBig then NewStep is toGreatDecrease", engine));
	ruleBlock->addRule(fl::Rule::parse("if Variation is Bad and Step is Colossal then NewStep is toGreatDecrease", engine));
	ruleBlock->addRule(fl::Rule::parse("if Variation is Bad and Step is Small then NewStep is toKeep", engine));
	ruleBlock->addRule(fl::Rule::parse("if Variation is Bad and Step is VerySmall then NewStep is toKeep", engine));
	ruleBlock->addRule(fl::Rule::parse("if Variation is Bad and Step is Insignificant then NewStep is toIncrease", engine));
	ruleBlock->addRule(fl::Rule::parse("if Variation is Zero then NewStep is toIncrease", engine));
	ruleBlock->addRule(fl::Rule::parse("if Variation is Good then NewStep is toIncrease", engine));
	ruleBlock->addRule(fl::Rule::parse("if Variation is Excelent then NewStep is toKeep", engine));
	ruleBlock->addRule(fl::Rule::parse("if Variation is Outstanding then NewStep is toKeep", engine));

	engine->addRuleBlock(ruleBlock);

}

void FuzzyMutation::adminFuzzyStep(double methodMean)
{
	if(methodMean<=-2.0e0)
	{
		//keep
	}
	else
	{
		if(methodMean>=2.0e0)
		{
			methodMean = 1.95;
		}

		scalar in = -methodMean + 2.0e0;
		scalar in2 = meanFuzzyStep;

		inputVariable1->setInputValue(in);
		inputVariable2->setInputValue(in2);
		engine->process();
		outputVariable1->defuzzify();

		//instructions
		double steptemp = meanFuzzyStep + (-1.0e0 + outputVariable1->getOutputValue());
		if(steptemp<0.05e0)
			meanFuzzyStep *= 0.5e0;
		else if(steptemp > 2.0e0)
			meanFuzzyStep = 2.0e0;
		else
			meanFuzzyStep = steptemp;

		//cout << "result of fuzzy " << outputVariable1->getOutputValue() << "   step    " << meanFuzzyStep << endl;
	
	}

}

void FuzzyMutation::makeMutation(int target, int parent, std::vector<vector<double>> &x_vec)
{
	double fuzzyStep;
	for(int i=0;i<(int)x_vec[parent].size();i++)
	{
		fuzzyStep = referenceVariation[i]*meanFuzzyStep;
		x_vec[target][i] = x_vec[parent][i] + MATH::randomNumber(-fuzzyStep,fuzzyStep);
	}
}


}



