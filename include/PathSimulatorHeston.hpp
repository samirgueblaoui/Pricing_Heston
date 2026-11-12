#pragma once

#include "ModelHeston.hpp"
#include <vector>

// D�finition d'une classe simulation des trajectoires pour le mod�le de Heston
// g�n�rale.

class PathSimulatorSVM {
public:
  // D�finition des diff�rents constructeurs

  PathSimulatorSVM(const HestonModel *model_ptr,
                   const std::vector<double> &time_points);
  PathSimulatorSVM(const HestonModel &model,
                   const std::vector<double> &time_points);

  /*D�finition des fonctions membres next_step1 (resp next_step2)
  qui � partir de St et Vt retourne St+delta_t (resp Vt+delta_t).*/

  virtual double next_step1(size_t current_time_idx, double St,
                            double Vt) const = 0;
  virtual double next_step2(size_t current_time_idx, double St,
                            double Vt) const = 0;

  /*D�finition de la fonction membre path() qui renvoie le vecteur contentant
  les St � chaque temps.*/

  std::vector<double> path() const;

  // Actualisation entre le premier et le dernier instant de la grille.
  double discount_factor() const;

  // D�finition de la m�thode clone et du destructeur.
  virtual ~PathSimulatorSVM();
  virtual PathSimulatorSVM *clone() const = 0;

protected:
  const HestonModel *_model_ptr;
  std::vector<double> _time_points;
};

// D�finition d'une classe fille utilisant le sh�ma Broadie-Kaya pour le mod�le
// de Heston g�n�rale.

class BroadieKayaPathSimulator : public PathSimulatorSVM {
public:
  // D�finition des diff�rents constructeurs

  BroadieKayaPathSimulator(const HestonModel *model_ptr,
                           const std::vector<double> &time_points);
  BroadieKayaPathSimulator(const HestonModel &model,
                           const std::vector<double> &time_points);
  BroadieKayaPathSimulator *clone() const override;

  /*D�finition des fonctions membres*/

  double next_step1(size_t current_time_idx, double St,
                    double Vt) const override;
  double next_step2(size_t current_time_idx, double St,
                    double Vt) const override;
};
