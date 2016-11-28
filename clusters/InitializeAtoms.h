#ifndef INITIALIZEATOMS_H
#define INITIALIZEATOMS_H

#include <vector>
#include <string>

class InitializeAtoms
{
public:
	InitializeAtoms::InitializeAtoms();

	InitializeAtoms::~InitializeAtoms();

	std::vector<double> generateCluster(int Natoms, double gamma_in, double Rca_in);
	
	double calcDist(const std::vector<double> &x, int i, int j);

private:
	double gamma;//parameter - sugerido: entre 0,2 e 0,4. Sao gerados entre (1-gamma) ate (1+gamma)
	double Rca;//Rcalfa na tese pag 62 - media do raio covalente.
	int natm;

	const double pi4sqr2 = 17.771531752633464988063523960243;

	const double pi = 3.1415926535897932384626433832795e0;

	const double exp3 = 0.33333333333333333333333333333333e0;

	std::vector<double> generateClusterRondina(int Natoms, double gamma_in, double Rca_in);

	std::vector<double> generateClusterFred(int Natoms, double gamma_in, double Rca_in);

	bool checkSuperposition(const std::vector<double> &x, int iMax);

	double findMinimumDistance(const std::vector<double> &dist, int i);

	double findMinimumDistanceUntilActualI(const std::vector<double> &dist, int i);

};

#endif



