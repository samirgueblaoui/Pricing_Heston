#pragma once
#include "PayoffHeston.h"


//Définition d'une classe Monte-Carlo pour le Modèle de Heston

class MonteCarloH
{
public:

	//Définition du members constructor
	MonteCarloH(size_t nb_sims, const PathSimulatorSVM& path_sim, const PayoffH& payoff);

	//définition de la fonction membre 
	double price() const;

private:
	size_t _number_simulations;
	const PathSimulatorSVM* _path_simulator_ptr;
	const PayoffH* _payoff_ptr;

};