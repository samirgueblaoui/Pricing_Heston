#pragma once
#include <vector>

// d�finition d'une classe m�re Mod�le � volatilit� stochastique

class StochasticVolatilityModel {
public:
  /*D�finition des diff�rent constructeurs(parameters constructor,
  copy constructor, move constructor, copy assignment operator, move assignment
  operator, etc...)*/

  StochasticVolatilityModel(const double &S0, const double &V0,
                            const double &rho);
  StochasticVolatilityModel(const StochasticVolatilityModel &model);
  StochasticVolatilityModel(StochasticVolatilityModel &&model) noexcept;
  StochasticVolatilityModel &operator=(const StochasticVolatilityModel &model);
  StochasticVolatilityModel &
  operator=(StochasticVolatilityModel &&model) noexcept;
  virtual ~StochasticVolatilityModel() = default;

  /*D�fintition des fonctions membres r, psi, a et b qui caract�rise le MVS*/

  virtual double r(const double &time) const = 0;
  virtual double Psi(const double &Vt) const = 0;
  virtual double a(const double &time, const double &Vt) const = 0;
  virtual double b(const double &time, const double &Vt) const = 0;

  // d�finition de la m�thode clone().

  virtual StochasticVolatilityModel *clone() const = 0;

  inline double S0() const { return _S0; }
  inline double V0() const { return _V0; }
  inline double rho() const { return _rho; }

protected:
  double _S0;
  double _V0;
  double _rho;
};

// d�finition d'une classe Mod�le de Heston en tant que classe fille du MVS.

class HestonModel : public StochasticVolatilityModel {
public:
  /*D�finition des diff�rents constructeurs(parameters constructor,
  copy constructor, move constructor, copy assignment operator, move assignment
  operator, etc...)*/

  HestonModel(const double &S0, const double &kappa, const double &theta,
              const double &sigma, const double &rho, const double &V0);
  HestonModel(const HestonModel &model);
  HestonModel(HestonModel &&model) noexcept;
  HestonModel &operator=(const HestonModel &model);
  HestonModel &operator=(HestonModel &&model) noexcept;
  virtual ~HestonModel() = default;

  /*D�fintition des fonctions membres r, psi, a et b qui caract�rise le Mo�le de
  Heston. r reste virtuelle pure car elle n'est pas d�fini explicitement dans le
  Mod�le de Heston g�n�ral, on pourra d�finir une classe fille de cette classe
  qui repr�sentera un mod�le de Heston plus particulier.*/
  virtual double r(const double &time) const = 0;
  double Psi(const double &Vt) const override;
  double a(const double &time, const double &Vt) const override;
  double b(const double &time, const double &Vt) const override;

  // d�finition de la m�thode clone().

  virtual HestonModel *clone() const = 0;
  inline double kappa() const { return _kappa; }
  inline double theta() const { return _theta; }
  inline double sigma() const { return _sigma; }

protected:
  double _kappa;
  double _theta;
  double _sigma;
};

// d�finition de la classe Mod�le de Heston plus particuli�re en tant que classe
// fille du Mod�le de Heston g�n�ral.

class HestonModel2 : public HestonModel

{
public:
  /*D�finition des diff�rent constructeurs(parameters constructor,
  copy constructor, move constructor, copy assignment operator, move assignment
  operator, etc...)*/

  HestonModel2(const double &S0, const double &kappa, const double &theta,
               const double &sigma, const double &rho, const double &V0,
               const double &r);
  HestonModel2(const HestonModel2 &model);
  HestonModel2(HestonModel2 &&model) noexcept;
  HestonModel2 &operator=(const HestonModel2 &model);
  HestonModel2 &operator=(HestonModel2 &&model) noexcept;
  virtual ~HestonModel2() = default;

  /*D�fintition de la fonction membre r qui caract�rise le Mo�le de Heston
   * particulier.*/

  double r(const double &time) const override;

  // d�finition de la m�thode clone().

  HestonModel2 *clone() const override;

private:
  double _r;
};
