#pragma once

#include <vector>
#include "PathSimulatorHeston.h"

//Définition d'une classe CALL ou PUT.
enum class CALL_PUT
{
	CALL,
	PUT
};

//Définition d'une classe mère Payoff.

class PayoffH
{
public:

	/*Définition d'une fonction membre path_price qui donnera 
	le prix du produit selon notre vecteur path()*/
	virtual double path_price(std::vector<double> asset_path) const = 0;

	//Définition de la méthode clone
	virtual PayoffH* clone() const = 0;
	//définition du destructeur
	virtual ~PayoffH() = default;
};

//Définition d'une classe fille option européenne.

class EuropeanOptionPayoffH : public PayoffH
{
public:

	// Définition du constructeur
	EuropeanOptionPayoffH(double strike, CALL_PUT call_put);

	/*Définition d'une fonction membre path_price qui donnera 
	le prix de l'option européenne selon notre vecteur path()*/
	double path_price(std::vector<double> asset_path) const override;

	EuropeanOptionPayoffH* clone() const override;

private:
	double _strike;
	CALL_PUT _call_put;
};