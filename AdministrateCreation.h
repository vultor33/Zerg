#ifndef ADMINISTRATECREATION_H
#define ADMINISTRATECREATION_H

#include <fstream>
#include <vector>

#include "Population.h"
#include "FuzzyAdministration.h"
#include "StructOptions.h"

// This class just look at results and decide what
// is the best course of action.
// to stop it just do: maximum_creation_operators_boost = 0
namespace zerg{
class AdministrateCreation
{
public:
	void initializeAdministration(
		std::ofstream &geneticOut_, 
		zerg::GaOptions &gaoptions,
		zerg::GaParameters &gaParam
		);

	void setNewIndividuals(int newComer, int method, double fitness);

	void adminCreationMethods(zerg::Population &pop, std::vector<double> &creation_rate);

private:
	int popSize;

	std::vector<double> oldFitness;
	std::vector<int> newIndividuals;
	std::vector<int> methodUsed;
	zerg::GaOptions * pgaoptions_;
	std::ofstream * pgeneticOut_;

	void setNewCreationRate(std::vector<double> &creation_rate, const std::vector<double> &methodMean, const std::vector<int> &methodUsed);
	bool wasCreated(int method,const std::vector<int> &methodUsed);

	zerg::FuzzyAdministration fuzzy_;
};

}

#endif



// a primeira tentativa vai ser simples
// guardo o individuo e o fitness medio dos pais.
// injusto em alguns casos, quem disse que a natureza e justa?

// espero calcular
// imprimo na tela os resultados sem mudar nada.





