#pragma once
#include "PayoffHeston.hpp"

// D�finition d'une classe Monte-Carlo pour le Mod�le de Heston

class MonteCarloH {
public:
  // D�finition du members constructor
  MonteCarloH(size_t nb_sims, const PathSimulatorSVM &path_sim,
              const PayoffH &payoff);

  // d�finition de la fonction membre
  double price() const;

private:
  size_t _number_simulations;
  const PathSimulatorSVM *_path_simulator_ptr;
  const PayoffH *_payoff_ptr;
};