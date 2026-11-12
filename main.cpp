#include "MonteCarloHeston.hpp"
#include <algorithm>
#include <cmath>
#include <cstdlib>
#include <fstream>
#include <functional>
#include <iomanip>
#include <iostream>
#include <iterator>
#include <numeric>
#include <random>
#include <vector>

int main() {

  std::vector<double> time_point;
  for (size_t idx = 0; idx < 2001; ++idx) {
    time_point.push_back(idx / 1000.);
  }
  // time_point = {0=t_0,t_1,...,t_n=2}, delta_t = 1/1000

  /*option europ�en de strike 105 de maturit� 2 sur un
  mod�le de Heston tel que S0=100, kappa=3, theta=0.3, sigma=0.2, rho=0.5,
  V0=0.1, r=0.1*/

  HestonModel2 model1(100.0, 3.0, 0.3, 0.2, 0.5, 0.1, 0.1);
  BroadieKayaPathSimulator psim(model1, time_point);
  CALL_PUT call = CALL_PUT::CALL;
  CALL_PUT put = CALL_PUT::PUT;
  EuropeanOptionPayoffH pof(105.0, call);
  EuropeanOptionPayoffH pof2(105.0, put);
  MonteCarloH Mont(100, psim, pof);
  MonteCarloH Mont2(100, psim, pof2);

  std::cout << "Prix du CALL Europeen: ";
  std::cout << Mont.price() << std::endl;
  std::cout << "Prix du PUT Europeen: ";
  std::cout << Mont2.price() << std::endl;

  /*On stocke ensuite les vecteurs path() g�n�r� et le vecteur time_point
  dans un fichier.txt (pathsimul.txt) pour tracer les trajectoire en python*/

  std::ofstream output("pathsimul.txt");
  if (output.is_open()) {

    for (size_t i = 0; i < 20; ++i) {
      BroadieKayaPathSimulator psim2(model1, time_point);
      for (const auto &value : psim2.path()) {
        output << value << std::endl;
      }
      output << '\n' << std::endl;
    }
    output.close();
  } else {
    return 1;
  }

  /*On stocke ensuite aussi le vecteur time_point
  dans un fichier.txt (pathsimul2.txt) pour tracer les trajectoire en python*/

  std::ofstream output2("pathsimul2.txt");
  if (output2.is_open()) {
    for (const auto &value : time_point) {
      output2 << value << std::endl;
    }
    output2.close();
  } else {
    return 1;
  }

  return 0;
}
