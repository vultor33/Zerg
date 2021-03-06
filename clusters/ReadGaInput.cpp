#include "ReadGaInput.h"

#include <iostream>
#include <string>
#include <fstream>
#include <vector>
#include <sstream>
#include <stdlib.h>

#include "../AuxMath.h"

using namespace zerg;
using namespace std;

ReadGaInput::~ReadGaInput() 
{
}

ReadGaInput::ReadGaInput() 
{
	inputName = "GaInput.txt";
}

void ReadGaInput::readGaInput()
{
	setDefaults();
	ifstream input_(inputName.c_str());
	vector<string> baseFiles;
	string auxline, type, equal, value, projectName, gamessHeader;
	string interactionPotential;

	while (getline(input_, auxline))
	{
		stringstream line;
		line << auxline;
		line >> type >> equal >> value;
		if (type == "%")
			continue;
		if (equal != "=")
		{
			cout << "wrong input format" << endl;
			exit(1);
		}
		stringstream convert;
		convert << value;
		if (type == "seed")
			convert >> gaParam.seed;
		else if (type == "restart")
		{
			string flagYes;
			convert >> flagYes;
			gaParam.restart = (flagYes == "yes");
		}
		else if (type == "number_of_cores")
			convert >> gamessNproc;
		else if (type == "project_name")
			convert >> projectName;
		else if (type == "population_size")
			convert >> gaParam.pop_size;
		else if (type == "maximum_number_of_generations")
			convert >> gaParam.maxGeneration;
		else if (type == "highlander_initial_energy")
			convert >> gaParam.highlanderInitialFitness;
		else if (type == "highlander_max_iteration")
			convert >> gaParam.highlanderMaxIteration;
		else if (type == "large_energy_value")
			convert >> gaParam.adminLargeEnergyVariation;
		else if (type == "maximum_creation_operators_boost")
			convert >> gaParam.adminMaxCreationVariation;
		else if (type == "predator_method")
			convert >> gaParam.predatorMethod;
		else if (type == "mutation_value")
			convert >> gaParam.mutationValue;
		else if (type == "crossover_weight")
			convert >> gaParam.crossoverWeight;
		else if (type == "crossover_probability")
			convert >> gaParam.corssoverProbability;
		else if (type == "sCartesianDisplacementOperator")
			convert >> gaParam.scdo;
		else if (type == "alfaMinGeometricDisplacement")
			convert >> gaParam.alfaMinGcdo;
		else if (type == "alfaMaxGeometricDisplacement")
			convert >> gaParam.alfaMaxGcdo;
		else if (type == "wGeometricDisplacement")
			convert >> gaParam.wGcdo;
		else if (type == "tetaMinTwistOperator")
			convert >> gaParam.tetaMinTwisto;
		else if (type == "tetaMaxTwistOperator")
			convert >> gaParam.tetaMaxTwisto;
		else if (type == "contractionMinMoveToCenter")
			convert >> gaParam.contractionMinMtco;
		else if (type == "contractionMaxMoveToCenter")
			convert >> gaParam.contractionMaxMtco;
		else if (type == "number_of_atoms")
		{
			convert >> gaParam.numberOfParameters;
			gaParam.numberOfParameters *= 3;
		}
		else if (type == "gamma_creation_radius")
			convert >> gaParam.gammaInitializeAtoms;
		else if (type == "radius_factor")
			convert >> gaParam.rcaInitializeAtoms;
		else if (type == "max_distance_between_atoms")
			convert >> gaParam.maxDistance;
		else if (type == "iterations_to_repeat_if_it_is_similar")
			convert >> gaParam.insistOnSimilar;
		else if (type == "interaction_potential")
			convert >> interactionPotential;
		else if (type == "gamess_executable_path")
			convert >> gamessPath;
		else if (type == "gamess_scr_path")
			convert >> gamessScr;
		else if (type == "bases_files_number")
		{
			int nBases;
			convert >> nBases;
			for (int i = 0; i < nBases; i++)
			{
				string auxBasesName;
				string typeBase, baseName;
				getline(input_, auxBasesName);
				stringstream lineBase;
				lineBase << auxBasesName;
				lineBase >> typeBase >> equal >> baseName;
				if (typeBase == "%")
					continue;
				else if (equal != "=")
				{
					cout << "wrong input format" << endl;
					exit(1);
				}
				else if (typeBase == "base")
					baseFiles.push_back(baseName);
			}
		}
		else if (type == "gamess_header_file")
			convert >> gamessHeader;
	}
	input_.close();

	if (interactionPotential == "gamess")
	{
		options.push_back("gamess");
		options.push_back(projectName);
		ifstream gamHeader_(("auxFiles/" + gamessHeader).c_str());
		while (getline(gamHeader_, auxline))
		{
			if (auxline.find("EndOfGamessHeader") != string::npos)
				break;
			options.push_back(auxline);
		}
		gamHeader_.close();
		options.push_back("EndOfHeader");
		for (size_t k = 0; k < baseFiles.size(); k++)
			options.push_back(baseFiles[k]);
		options.push_back("EndOfBasis");
		options.push_back("NoECP");
	}
}

void ReadGaInput::setExperimentDefaults(int seed)
{
	gaParam.seed = seed;
	gaParam.restart = false;
	gaParam.pop_size = 40;
	gaParam.maxGeneration = 300;
	gaParam.highlanderInitialFitness = 1.0e99;
	gaParam.highlanderMaxIteration = 300;
	gaParam.adminLargeEnergyVariation = 2.0e0;
	gaParam.adminMaxCreationVariation = 0.0e0;
	gaParam.insistOnSimilar = 50;
	gaParam.predatorMethod = 0;
	gaParam.n_process = 1;

	gaParam.mutationValue = 0.1e0;
	gaParam.crossoverWeight = 0.7e0;
	gaParam.corssoverProbability = 0.7e0;
	gaParam.numberOfParameters = 15;
	gaParam.gammaInitializeAtoms = 0.4;
	gaParam.rcaInitializeAtoms = 2.0;
	gaParam.maxDistance = 1.0e99;

	AuxMath auxMath_;
	// OPERATORS PARAMETERS
	gaParam.scdo = 0.1e0;
	gaParam.alfaMinGcdo = 0.2e0;
	gaParam.alfaMaxGcdo = 0.45e0;
	gaParam.wGcdo = 2;
	gaParam.tetaMinTwisto = auxMath_._pi / 6;
	gaParam.tetaMaxTwisto = auxMath_._pi;
	gaParam.contractionMinMtco = 0.1e0;
	gaParam.contractionMaxMtco = 0.8e0;

	gaParam.initialCreationRate.resize(7);
	gaParam.initialCreationRate[0] = 0.0e0;
	gaParam.initialCreationRate[1] = 0.0e0;
	gaParam.initialCreationRate[2] = 0.0e0;
	gaParam.initialCreationRate[3] = 0.0e0;
	gaParam.initialCreationRate[4] = 0.0e0;
	gaParam.initialCreationRate[5] = 0.0e0;
	gaParam.initialCreationRate[6] = 0.0e0;
}


void ReadGaInput::setInputInformations(GaParameters gaParam_in)
{
	gaParam = gaParam_in;
}



vector<string> ReadGaInput::getOptions()
{
	return options;
}

void ReadGaInput::setDefaults()
{
	gaParam.seed = 3;
	gaParam.restart = false;
	gaParam.pop_size = 40;
	gaParam.maxGeneration = 300;
	gaParam.highlanderInitialFitness = 1.0e99;
	gaParam.highlanderMaxIteration = 50;
	gaParam.adminLargeEnergyVariation = 2.0e0;
	gaParam.adminMaxCreationVariation = 0.9e0;
	gaParam.insistOnSimilar = 30;
	gaParam.predatorMethod = 0;
	gaParam.n_process = 1;
	gaParam.mutationValue = 0.1e0;
	gaParam.crossoverWeight = 0.7e0;
	gaParam.corssoverProbability = 0.7e0;
	gaParam.numberOfParameters = 15;
	gaParam.gammaInitializeAtoms = 0.2;
	gaParam.rcaInitializeAtoms = 1.0;
	gaParam.maxDistance = 1.0e99;

	AuxMath auxMath_;

	// OPERATORS PARAMETERS
	gaParam.scdo = 0.1e0;
	gaParam.alfaMinGcdo = 0.2e0;
	gaParam.alfaMaxGcdo = 0.45e0;
	gaParam.wGcdo = 2;
	gaParam.tetaMinTwisto = auxMath_._pi / 6;
	gaParam.tetaMaxTwisto = auxMath_._pi;
	gaParam.contractionMinMtco = 0.1e0;
	gaParam.contractionMaxMtco = 0.8e0;
}
