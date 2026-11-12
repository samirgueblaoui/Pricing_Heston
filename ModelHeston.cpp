#include "ModelHeston.h"
#include <utility>

/*Implémentation des différent constructeurs de la classe du MVS*/

//Parameters constructor
StochasticVolatilityModel::StochasticVolatilityModel(const double& S0, const double& V0, const double& rho)
	: _S0(S0), _V0(V0), _rho(rho)
{
}
//Copy constructor
StochasticVolatilityModel::StochasticVolatilityModel(const StochasticVolatilityModel& model)
	: _S0(model._S0), _V0(model._V0), _rho(model._rho)
{
}
//Move constructor
StochasticVolatilityModel::StochasticVolatilityModel(StochasticVolatilityModel&& model) noexcept
	: _S0(std::move(model._S0)), _V0(std::move(model._V0)), _rho(std::move(model._rho))
{
}
//Copy assignment operator
StochasticVolatilityModel& StochasticVolatilityModel::operator=(const StochasticVolatilityModel& model)
{
	if (this != &model)
	{
		_S0 = model._S0;
		_V0 = model._V0;
		_rho = model._rho;
	}
	return *this;
}
//Move assignment operator
StochasticVolatilityModel& StochasticVolatilityModel::operator=(StochasticVolatilityModel&& model) noexcept
{
	if (this != &model)
	{
		_S0 = std::move(model._S0);
		_V0 = std::move(model._V0);
		_rho = std::move(model._rho);
	}
	return *this;
}

/*Implémentation des différents constructeurs de la classe du Modèle de Heston général*/

HestonModel::HestonModel(const double& S0, const double& kappa, const double& theta, const double& sigma, const double& rho, const double& V0)
	: StochasticVolatilityModel(S0, V0, rho), _kappa(kappa), _theta(theta), _sigma(sigma)
{
}

HestonModel::HestonModel(const HestonModel& model)
	: StochasticVolatilityModel(model), _kappa(model._kappa), _theta(model._theta), _sigma(model._sigma)
{
}

HestonModel::HestonModel(HestonModel&& model) noexcept
	: StochasticVolatilityModel(std::move(model)), _kappa(std::move(model._kappa)), _theta(std::move(model._theta)), _sigma(std::move(model._sigma))
{
}

HestonModel& HestonModel::operator=(const HestonModel& model)
{
	if (this != &model)
	{
		StochasticVolatilityModel::operator=(model);
		_kappa = model._kappa;
		_theta = model._theta;
		_sigma = model._sigma;
	}
	return *this;
}

HestonModel& HestonModel::operator=(HestonModel&& model) noexcept
{
	if (this != &model)
	{
		StochasticVolatilityModel::operator=(std::move(model));
		_kappa = std::move(model._kappa);
		_theta = std::move(model._theta);
		_sigma = std::move(model._sigma);
	}
	return *this;
}

/*Implémentation des différentes fonctions membres de la classe du Modèle de Heston général*/

double HestonModel::Psi(const double& Vt) const
{
	return sqrt(Vt);
}

double HestonModel::a(const double& time, const double& Vt) const
{
	return _kappa * (_theta - Vt);
}

double HestonModel::b(const double& time, const double& Vt) const
{
	return _sigma * sqrt(Vt);
}

/*Implémentation des différents constructeurs de la classe du Modèle de Heston particulier*/

HestonModel2::HestonModel2(const double& S0, const double& kappa, const double& theta, const double& sigma, const double& rho, const double& V0, const double& r)
	: HestonModel(S0, kappa, theta, sigma, rho, V0), _r(r)
{
}

HestonModel2::HestonModel2(const HestonModel2& model)
	: HestonModel(model), _r(model._r)
{
}

HestonModel2::HestonModel2(HestonModel2&& model) noexcept
	: HestonModel(std::move(model)), _r(std::move(model._r))
{
}

HestonModel2& HestonModel2::operator=(const HestonModel2& model)
{
	if (this != &model)
	{
		HestonModel::operator=(model);
		_r = model._r;
	}
	return *this;
}

HestonModel2& HestonModel2::operator=(HestonModel2&& model) noexcept
{
	if (this != &model)
	{
		HestonModel2::operator=(std::move(model));
		_r = std::move(model._r);
	}
	return *this;
}

/*Implémentation de la fonction membre r de la classe du Modèle de Heston particulier, on étudiera le cas où r est constant.*/

double HestonModel2::r(const double& time) const
{
	return _r;
}

//Implémentation de la méthode clone().

HestonModel2* HestonModel2::clone() const
{
	return new HestonModel2(*this);
}