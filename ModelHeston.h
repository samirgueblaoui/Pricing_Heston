#pragma once
#include <vector>


// définition d'une classe mère Modèle à volatilité stochastique

class StochasticVolatilityModel
{
public:

	/*Définition des différent constructeurs(parameters constructor, 
	copy constructor, move constructor, copy assignment operator, move assignment operator, etc...)*/

	StochasticVolatilityModel(const double& S0, const double& V0, const double& rho);
	StochasticVolatilityModel(const StochasticVolatilityModel& model);
	StochasticVolatilityModel(StochasticVolatilityModel&& model) noexcept;
	StochasticVolatilityModel& operator=(const StochasticVolatilityModel& model);
	StochasticVolatilityModel& operator=(StochasticVolatilityModel&& model) noexcept;
	virtual ~StochasticVolatilityModel() = default;

	/*Défintition des fonctions membres r, psi, a et b qui caractérise le MVS*/

	virtual double r(const double& time) const = 0;
	virtual double Psi(const double& Vt) const = 0;
	virtual double a(const double& time, const double& Vt) const = 0;
	virtual double b(const double& time, const double& Vt) const = 0;

	//définition de la méthode clone().

	virtual StochasticVolatilityModel* clone() const = 0;
	
	inline double S0() const
	{
		return _S0;
	}
	inline double V0() const
	{
		return _V0;
	}
	inline double rho() const
	{
		return _rho;
	}

protected:
	double _S0; 
	double _V0;
	double _rho;
};

// définition d'une classe Modèle de Heston en tant que classe fille du MVS.

class HestonModel : public StochasticVolatilityModel
{
public:

	/*Définition des différents constructeurs(parameters constructor,
	copy constructor, move constructor, copy assignment operator, move assignment operator, etc...)*/

	HestonModel(const double& S0, const double& kappa, const double& theta, const double& sigma, const double& rho, const double& V0);
	HestonModel(const HestonModel& model);
	HestonModel(HestonModel&& model) noexcept;
	HestonModel& operator=(const HestonModel& model);
	HestonModel& operator=(HestonModel&& model) noexcept;
	virtual ~HestonModel() = default;

	/*Défintition des fonctions membres r, psi, a et b qui caractérise le Moèle de Heston.
	r reste virtuelle pure car elle n'est pas défini explicitement dans le Modèle de Heston général,
	on pourra définir une classe fille de cette classe qui représentera un modèle de Heston plus particulier.*/

	virtual double r(const double& time) const =0;
	double Psi(const double& Vt) const override;
	double a(const double& time, const double& Vt) const override;
	double b(const double& time, const double& Vt) const override;

	//définition de la méthode clone().

	virtual HestonModel* clone() const =0;

	inline double kappa() const
	{
		return _kappa;
	}
	inline double theta() const
	{
		return _theta;
	}
	inline double sigma() const
	{
		return _sigma;
	}
	
protected:
	double _kappa; 
	double _theta;
	double _sigma;
};

// définition de la classe Modèle de Heston plus particulière en tant que classe fille du Modèle de Heston général.

class HestonModel2 : public HestonModel

{
public:

	/*Définition des différent constructeurs(parameters constructor,
	copy constructor, move constructor, copy assignment operator, move assignment operator, etc...)*/

	HestonModel2(const double& S0, const double& kappa, const double& theta, const double& sigma, const double& rho, const double& V0, const double& r);
	HestonModel2(const HestonModel2& model);
	HestonModel2(HestonModel2&& model) noexcept;
	HestonModel2& operator=(const HestonModel2& model);
	HestonModel2& operator=(HestonModel2&& model) noexcept;
	virtual ~HestonModel2() = default;

	/*Défintition de la fonction membre r qui caractérise le Moèle de Heston particulier.*/

	double r(const double& time) const override;

	//définition de la méthode clone().

	HestonModel2* clone() const override;

private:
	double _r;
};
