#include "PayoffHeston.h"

//Implémentation du members constructor

EuropeanOptionPayoffH::EuropeanOptionPayoffH(double strike, CALL_PUT call_put)
	: _strike(strike), _call_put(call_put)
{
}

//Implémentation de la fonction path_price pour les options européennes

double EuropeanOptionPayoffH::path_price(std::vector<double> asset_path) const
{
	double price_at_maturity = asset_path[asset_path.size() - 1]; //ST
	double multpilicative_factor = _call_put == CALL_PUT::CALL ? 1. : -1.; 
	//si le _call_put=CALL alors multpilicative_factor = 1 sinon multpilicative_factor= -1.

	double payoff = std::max(multpilicative_factor * (price_at_maturity - _strike), 0.);
	//payoff = (ST - K)+ si _call_put = CALL et payoff = (K - ST)+ si _call_put = PUT

	return payoff;
}

//Implémentation de la méthode clone
EuropeanOptionPayoffH* EuropeanOptionPayoffH::clone() const
{
	return new EuropeanOptionPayoffH(*this);
}
