#include "PathSimulatorHeston.hpp"

#include <cmath>
#include <random>
#include <stdexcept>

/*Impl�mentation des diff�rents constructeurs de la classe simulation des
 * trajectoires*/

PathSimulatorSVM::PathSimulatorSVM(const HestonModel *model_ptr,
                                   const std::vector<double> &time_points)
    : _model_ptr(model_ptr->clone()), _time_points(time_points) {}

PathSimulatorSVM::PathSimulatorSVM(const HestonModel &model,
                                   const std::vector<double> &time_points)
    : _model_ptr(model.clone()), _time_points(time_points) {}

double PathSimulatorSVM::discount_factor() const {
  if (_time_points.empty())
    throw std::invalid_argument("The time grid must not be empty.");

  double integrated_rate = 0.;
  for (size_t idx = 1; idx < _time_points.size(); ++idx) {
    const double previous_time = _time_points[idx - 1];
    const double current_time = _time_points[idx];
    const double delta_t = current_time - previous_time;
    if (!(delta_t > 0.))
      throw std::invalid_argument("The time grid must be strictly increasing.");

    // Integration du taux par la regle des trapezes.
    integrated_rate += 0.5 * (_model_ptr->r(previous_time) +
                             _model_ptr->r(current_time)) * delta_t;
  }
  return std::exp(-integrated_rate);
}

// Impl�mentation de la fonction membre path() qui retourne le vecteur de St.

std::vector<double> PathSimulatorSVM::path() const {
  std::vector<double> path1; // size = 0
  std::vector<double> path2;
  path1.push_back(_model_ptr->S0()); // size = 1
  path2.push_back(_model_ptr->V0());

  for (size_t idx = 0; idx < _time_points.size() - 1; ++idx) {
    path1.push_back(next_step1(idx, path1[idx], path2[idx])); // size idx + 2
    path2.push_back(next_step2(idx, path1[idx], path2[idx]));
  }
  return path1;
}

// impl�mentation du destructeur.

PathSimulatorSVM::~PathSimulatorSVM() { delete _model_ptr; }

/*Impl�mentation des diff�rents constructeurs de la classe fille Broadie-Kaya*/

BroadieKayaPathSimulator::BroadieKayaPathSimulator(
    const HestonModel *model_ptr, const std::vector<double> &time_points)
    : PathSimulatorSVM(model_ptr->clone(), time_points) {}

BroadieKayaPathSimulator::BroadieKayaPathSimulator(
    const HestonModel &model, const std::vector<double> &time_points)
    : PathSimulatorSVM(model.clone(), time_points) {}

BroadieKayaPathSimulator *BroadieKayaPathSimulator::clone() const {
  return new BroadieKayaPathSimulator(*this);
}

// Impl�mentation de la fonction membre next_step1 qui renvoi St+delta_t �
// partir de St et Vt

double BroadieKayaPathSimulator::next_step1(size_t current_time_idx, double St,
                                            double Vt) const {
  std::random_device rd1;
  std::mt19937 gen1(rd1());
  std::random_device rd2;
  std::mt19937 gen2(rd2());
  std::normal_distribution<double> distribution(0, 1.);

  double randomVariable1 = distribution(
      gen1); // g�n�re une issu d'une variable al�atoire normal centr� r�duite.
  double randomVariable2 = distribution(gen2);

  double delta_t =
      _time_points[current_time_idx + 1] -
      _time_points[current_time_idx]; // g�n�re le pas de temps entre deux
                                      // instant du vecteur time_point

  double m = _model_ptr->theta() +
             (Vt - _model_ptr->theta()) * exp(-_model_ptr->kappa() * delta_t);
  // m = m(t;delta_t) = E(Vt+delta_t)
  double s = (Vt * _model_ptr->sigma() * _model_ptr->sigma() *
              exp(-_model_ptr->kappa() * delta_t) / _model_ptr->kappa()) *
                 (1. - exp(-_model_ptr->kappa() * delta_t)) +
             (_model_ptr->theta() * _model_ptr->sigma() * _model_ptr->sigma() /
              (2. * _model_ptr->kappa())) *
                 (1. - exp(-_model_ptr->kappa() * delta_t)) *
                 (1. - exp(-_model_ptr->kappa() * delta_t));
  // s = s�(t;delta_t) = Var(Vt+delta_t)

  // On vas utiliser ici le sch�ma Exponentiel Quadratique [QE] et supposer que
  // Vt+delta_t = a(b + Z)� avec Z~N(0;1)
  double q = s / (m * m);
  double b = sqrt((2 / q) - 1 + sqrt((2 / q)) * sqrt((2 / q) - 1));
  double a = m / (1 + b * b);
  double Xt = log(St);
  double Vt2 = a * (b + randomVariable2) * (b + randomVariable2);

  /*On utilise ensuite le shema de Broadie - Kaya pour exprimer Xt+delta_t =
  ln(St+delta_t) � partir de Xt = ln(St), Vt et Vt+delta_t.*/
  double Xt2 =
      Xt +
      delta_t * (1. / 2.) *
          (_model_ptr->r(_time_points[current_time_idx]) +
           _model_ptr->r(_time_points[current_time_idx] + delta_t)) +
      (_model_ptr->rho() / _model_ptr->sigma()) *
          (Vt2 - Vt - _model_ptr->kappa() * _model_ptr->theta() * delta_t) +
      (((_model_ptr->kappa() * _model_ptr->rho()) / _model_ptr->sigma()) -
       (1. / 2.)) *
          delta_t * (1. / 2.) * (Vt + Vt2) +
      std::sqrt((1. - (_model_ptr->rho() * _model_ptr->rho())) * delta_t *
                (1. / 2.) * (Vt + Vt2)) *
          randomVariable1;
  double St2 = exp(Xt2);
  return St2;
}

// Impl�mentation de la fonction membre next_step2 qui renvoi Vt+delta_t �
// partir de St et Vt.

double BroadieKayaPathSimulator::next_step2(size_t current_time_idx, double St,
                                            double Vt) const {
  std::random_device rd1;
  std::mt19937 gen1(rd1());
  std::random_device rd2;
  std::mt19937 gen2(rd2());
  std::normal_distribution<double> distribution(0, 1.);

  double randomVariable1 = distribution(gen1);
  double randomVariable2 = distribution(gen2);
  double delta_t =
      _time_points[current_time_idx + 1] - _time_points[current_time_idx];

  double m = _model_ptr->theta() +
             (Vt - _model_ptr->theta()) * exp(-_model_ptr->kappa() * delta_t);
  double s = (Vt * _model_ptr->sigma() * _model_ptr->sigma() *
              exp(-_model_ptr->kappa() * delta_t) / _model_ptr->kappa()) *
                 (1. - exp(-_model_ptr->kappa() * delta_t)) +
             (_model_ptr->theta() * _model_ptr->sigma() * _model_ptr->sigma() /
              (2. * _model_ptr->kappa())) *
                 (1. - exp(-_model_ptr->kappa() * delta_t)) *
                 (1. - exp(-_model_ptr->kappa() * delta_t));
  double q = s / (m * m);
  double b = sqrt((2 / q) - 1 + sqrt((2 / q)) * sqrt((2 / q) - 1));
  double a = m / (1 + b * b);
  double Xt = log(St);
  double Vt2 = a * (b + randomVariable2) * (b + randomVariable2);
  double Xt2 =
      Xt +
      delta_t * (1. / 2.) *
          (_model_ptr->r(_time_points[current_time_idx]) +
           _model_ptr->r(_time_points[current_time_idx] + delta_t)) +
      (_model_ptr->rho() / _model_ptr->sigma()) *
          (Vt2 - Vt - _model_ptr->kappa() * _model_ptr->theta() * delta_t) +
      (((_model_ptr->kappa() * _model_ptr->rho()) / _model_ptr->sigma()) -
       (1. / 2.)) *
          delta_t * (1. / 2.) * (Vt + Vt2) +
      std::sqrt((1. - (_model_ptr->rho() * _model_ptr->rho())) * delta_t *
                (1. / 2.) * (Vt + Vt2)) *
          randomVariable1;
  double St2 = exp(Xt2);
  return Vt2;
}
