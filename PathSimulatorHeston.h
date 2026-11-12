#pragma once

#include <vector>
#include "ModelHeston.h"

// Définition d'une classe simulation des trajectoires pour le modèle de Heston générale.

class PathSimulatorSVM
{
public:

	//Définition des différents constructeurs

	PathSimulatorSVM(const HestonModel* model_ptr, const std::vector<double>& time_points);
	PathSimulatorSVM(const HestonModel& model, const std::vector<double>& time_points);

	/*Définition des fonctions membres next_step1 (resp next_step2) 
	qui à partir de St et Vt retourne St+delta_t (resp Vt+delta_t).*/

	virtual double next_step1(size_t current_time_idx, double St, double Vt ) const = 0;
	virtual double next_step2(size_t current_time_idx, double St, double Vt) const = 0;

	/*Définition de la fonction membre path() qui renvoie le vecteur contentant 
	les St à chaque temps.*/

	std::vector<double> path() const;

	//Définition de la méthode clone et du destructeur. 
	virtual ~PathSimulatorSVM();
	virtual PathSimulatorSVM* clone() const = 0;

protected:
	const HestonModel* _model_ptr;
	std::vector<double> _time_points; 

};

// Définition d'une classe fille utilisant le shéma Broadie-Kaya pour le modèle de Heston générale.

class BroadieKayaPathSimulator : public PathSimulatorSVM
{
public:

	//Définition des différents constructeurs

	BroadieKayaPathSimulator(const HestonModel* model_ptr, const std::vector<double>& time_points);
	BroadieKayaPathSimulator(const HestonModel& model, const std::vector<double>& time_points);
	BroadieKayaPathSimulator* clone() const override;

	/*Définition des fonctions membres*/

	double next_step1(size_t current_time_idx, double St, double Vt) const override;
	double next_step2(size_t current_time_idx, double St, double Vt) const override;
};
