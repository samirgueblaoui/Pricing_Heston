#pragma once

#include "PathSimulatorHeston.hpp"
#include <vector>

// D�finition d'une classe CALL ou PUT.
enum class CALL_PUT { CALL, PUT };

// D�finition d'une classe m�re Payoff.

class PayoffH {
public:
  /*D�finition d'une fonction membre path_price qui donnera
  le prix du produit selon notre vecteur path()*/
  virtual double path_price(std::vector<double> asset_path) const = 0;

  // D�finition de la m�thode clone
  virtual PayoffH *clone() const = 0;
  // d�finition du destructeur
  virtual ~PayoffH() = default;
};

// D�finition d'une classe fille option europ�enne.

class EuropeanOptionPayoffH : public PayoffH {
public:
  // D�finition du constructeur
  EuropeanOptionPayoffH(double strike, CALL_PUT call_put);

  /*D�finition d'une fonction membre path_price qui donnera
  le prix de l'option europ�enne selon notre vecteur path()*/
  double path_price(std::vector<double> asset_path) const override;

  EuropeanOptionPayoffH *clone() const override;

private:
  double _strike;
  CALL_PUT _call_put;
};