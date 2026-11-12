#include "MonteCarloHeston.hpp"

#include <cmath>
#include <iostream>
#include <stdexcept>
#include <string>
#include <vector>

// Des trajectoires constantes donnent des payoffs connus, sans bruit Monte-Carlo.
class FixedPathSimulator : public PathSimulatorSVM {
public:
  FixedPathSimulator(const HestonModel &model,
                     const std::vector<double> &time_points)
      : PathSimulatorSVM(model, time_points) {}

  double next_step1(size_t, double St, double) const override { return St; }
  double next_step2(size_t, double, double Vt) const override { return Vt; }

  FixedPathSimulator *clone() const override {
    return new FixedPathSimulator(*_model_ptr, _time_points);
  }
};

class LinearRateModel : public HestonModel2 {
public:
  LinearRateModel() : HestonModel2(120., 3., 0.3, 0.2, 0.5, 0.1, 0.02) {}
  double r(const double &time) const override { return 0.02 + 0.03 * time; }
  LinearRateModel *clone() const override { return new LinearRateModel(*this); }
};

void expect_close(const std::string &label, double actual, double expected) {
  if (!std::isfinite(actual) ||
      std::abs(actual - expected) > 1e-12 * (1. + std::abs(expected))) {
    throw std::runtime_error(label + ": expected " + std::to_string(expected) +
                             ", got " + std::to_string(actual));
  }
}

void check_prices(const std::string &label, const HestonModel &model,
                  const std::vector<double> &times, double discount) {
  FixedPathSimulator simulator(model, times);
  EuropeanOptionPayoffH call(100., CALL_PUT::CALL);
  EuropeanOptionPayoffH put(150., CALL_PUT::PUT);
  EuropeanOptionPayoffH otm_call(150., CALL_PUT::CALL);
  EuropeanOptionPayoffH otm_put(100., CALL_PUT::PUT);

  expect_close(label + " call payoff", call.path_price({120.}), 20.);
  expect_close(label + " put payoff", put.path_price({120.}), 30.);
  expect_close(label + " call", MonteCarloH(7, simulator, call).price(),
               20. * discount);
  expect_close(label + " put", MonteCarloH(7, simulator, put).price(),
               30. * discount);
  expect_close(label + " OTM call", MonteCarloH(7, simulator, otm_call).price(),
               0.);
  expect_close(label + " OTM put", MonteCarloH(7, simulator, otm_put).price(),
               0.);
}

void check_invalid_grid(const HestonModel &model,
                        const std::vector<double> &times) {
  FixedPathSimulator simulator(model, times);
  EuropeanOptionPayoffH call(100., CALL_PUT::CALL);
  try {
    MonteCarloH(1, simulator, call).price();
  } catch (const std::invalid_argument &) {
    return;
  }
  throw std::runtime_error("An invalid time grid was accepted.");
}

int main() {
  try {
    HestonModel2 positive_rate(120., 3., 0.3, 0.2, 0.5, 0.1, 0.1);
    HestonModel2 zero_rate(120., 3., 0.3, 0.2, 0.5, 0.1, 0.);
    HestonModel2 negative_rate(120., 3., 0.3, 0.2, 0.5, 0.1, -0.02);

    std::vector<double> default_grid;
    for (size_t idx = 0; idx < 2001; ++idx)
      default_grid.push_back(idx / 1000.);

    check_prices("default grid", positive_rate, default_grid, std::exp(-0.2));
    check_prices("zero rate", zero_rate, {0., 0.4, 2.}, 1.);
    check_prices("negative rate", negative_rate, {0., 2.}, std::exp(0.04));
    check_prices("different maturity", positive_rate, {0., 0.7},
                 std::exp(-0.07));
    check_prices("nonzero start", positive_rate, {1., 1.4, 3.},
                 std::exp(-0.2));
    check_prices("zero duration", positive_rate, {2.}, 1.);

    LinearRateModel linear_rate;
    // Integral of 0.02 + 0.03*t from 0 to 2: 0.04 + 0.06 = 0.10.
    check_prices("linear rate", linear_rate, {0., 0.1, 0.8, 2.},
                 std::exp(-0.10));
    // Integral from 1 to 3: 0.04 + 0.015*(9 - 1) = 0.16.
    check_prices("linear rate with nonzero start", linear_rate, {1., 1.2, 3.},
                 std::exp(-0.16));

    check_invalid_grid(positive_rate, {});
    check_invalid_grid(positive_rate, {0., 2., 1.});
    check_invalid_grid(positive_rate, {0., 0., 2.});
    std::cout << "Discounting tests passed.\n";
  } catch (const std::exception &error) {
    std::cerr << error.what() << '\n';
    return 1;
  }
}
