#include "Creation.h"

#include <iostream>
#include <vector>
#include <math.h>
#include <fstream>

#include "Population.h"
#include "Predator.h"
#include "AuxMathGa.h"
#include "ParallelOptimization.h"
#include "StructOptions.h"

using namespace std;
using namespace zerg;


// print de cada individuo e no final eu quero saber qual o metodo de criacao
// e qual o fitness dele (talvez ate antes e depois)


namespace zerg{
void Creation::initialize_creation(int pop_size,
								   int number_creation_methods, 
								   int n_process, 
								   ofstream &geneticOut_,
								   GaOptions &gaoptions)
{
	pgeneticOut_ = &geneticOut_;
	pgaoptions_ = &gaoptions;
	admin_.initializeAdministration(geneticOut_, pop_size, gaoptions);

	number_methods = number_creation_methods;
	creation_methods.resize(number_methods);
	creation_rate.resize(number_methods);
	for(int i=0; i<number_methods; i++)
	{
		creation_methods[i].resize(pop_size+1);
		creation_rate[i] = (1.0e0)/((double)number_methods);
	}
	go_parallel_.set_number_of_processor(n_process);
}

void Creation::make_new_individuals(Population &pop, Predator &pred)
{
	set_creation_methods(pred);

	creation_from_methods(pop,pred);

	go_parallel_.do_optimization(pop,pred.dead_individuals);

	admin_.adminCreationMethods(pop, creation_rate);

}

void Creation::set_creation_methods(Predator &pred)
{
	double to_create = (double)pred.dead_individuals.size();
	vector<bool> methodDone(number_methods);
	for(int i=0; i<number_methods; i++)
	{
		methodDone[i] = false;
	}

	// set number for each method
	int creationTry;
	int auxtoCreate = 0;
	for(;;)
	{
		creationTry = AuxMathGa::randomNumber(0,number_methods-1);
		if(lastMethod(methodDone, creation_methods, (int)to_create - auxtoCreate))
		{
			break;
		}
		else if(!methodDone[creationTry])
		{
			creation_methods[creationTry][0] = (int) floor(0.5e0+to_create*creation_rate[creationTry]);
			methodDone[creationTry] = true;
			auxtoCreate += creation_methods[creationTry][0];

			if(auxtoCreate==9)
			{
				creation_methods[creationTry][0]-=1;
				auxtoCreate--;
			}
			else if(auxtoCreate>9)
			{
				*pgeneticOut_ << " Erro em set_creation_method " << endl;
				exit(1);
			}
		}
	}


	// set individuals to be created from dead (1,12 etc.)
	int dead = 0;
	for(int method=0; method<number_methods; method++)
	{
		for(int i=0; i<creation_methods[method][0]; i++)
		{
			creation_methods[method][i+1] = pred.dead_individuals[dead];
			dead++;
		}
	}
}


bool Creation::lastMethod(const vector<bool> &methodDone, std::vector<vector<int>> &creation_methods, int createLast)
{
	int lastPos = -1;
	bool lastFlag = false;
	int auxSum = 0;
	for(int i=0; i<(int)methodDone.size();i++)
	{
		auxSum += (int)methodDone[i];
		if(!methodDone[i])
		{
			lastPos = i;
		}
	}

	if(auxSum!=((int)methodDone.size()-1))
	{
		return false;
	}
	else
	{
		creation_methods[lastPos][0] = createLast;
		return true;
	}

}




void Creation::creation_from_methods(Population &pop, Predator &pred)
{
	bool aux1,is_similar;
	int parent1=0;
	int parent2=0;
	int target;

	for(int method=0; method<number_methods; method++)
	{
		for(int i=0; i<creation_methods[method][0]; i++)
		{
			target = creation_methods[method][i+1];
			int similar_cont = 0;

			// repeat until is different
			do
			{
				select_parents(pred,parent1,parent2);
				aux1 = pop.create_individual(method, 
											target, 
											parent1, 
											parent2);

				is_similar = pop.check_similarity(target);

				similar_cont++;
				if(similar_cont>29)
				{
					printSimilarityProblem(method);
					break;
				}
			} while(is_similar);

			admin_.setNewIndividuals(target,
									 method,
									 0.5e0*(pop.getfitness(parent1)+pop.getfitness(parent2)));
		}
	}
}

void Creation::select_parents(Predator &pred, int &parent1, int &parent2)
{
	int pop_size = pred.get_pop_size();

	parent1=-1;
	parent2=-1;
	while(parent1==parent2)
	{
		do
		{
			parent1 = AuxMathGa::randomNumber(0,pop_size-1);
		}while(is_dead(pred, parent1));

		do
		{
			parent2 = AuxMathGa::randomNumber(0,pop_size-1);
		}while(is_dead(pred, parent2));
	}
}

bool Creation::is_dead(Predator &pred, int parent)
{
	int size = pred.dead_individuals.size();
	bool dead = false;
	dead = parent<0;
	if(dead)
	{
		return dead;
	}
	for(int i=0; i < size ; i++)
	{
		dead = parent==pred.dead_individuals[i];
		if(dead)
		{
			break;
		}
	}

	return dead;
}


void Creation::printSimilarityProblem(int method)
{
	if(pgaoptions_->similarityProblem)
		*pgeneticOut_ << "WARNING!!!    Method:  " << 
			method << "   didnt surpass similarity. " << endl;
}


}
