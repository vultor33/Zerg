#ifndef MATH_H
#define MATH_H

#include <vector>
#include <fstream>

namespace zerg{
class MATH
{
public:
	//random numbers
	static void set_seed(int seed);
	static double randomNumber(double fMin, double fMax);
	static int randomNumber(int fMin, int fMax);

	//ordering
	static std::vector<int> vector_ordering(std::vector<double> &vetor_entrada); //organiza a entrada e solta um vetor de organizacao
	static void vector_ordering_with_instructions(std::vector<std::vector<double>> &vetor_entrada, const std::vector<int> &vetor_organiza); //organiza uma matriz com a instrucao obtida no anterior.
	static void vector_ordering_with_instructions(std::vector<int> &vetor_entrada, const std::vector<int> &vetor_organiza); //organiza uma matriz com a instrucao obtida no anterior.

};
}

#endif