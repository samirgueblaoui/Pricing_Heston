# Pricing Modele de Heston

Projet C++ de simulation du modèle de Heston à volatilité stochastique et d'estimation par Monte-Carlo des prix d'options européennes **call** et **put**. Un script Python permet de visualiser les trajectoires du sous-jacent exportées par le programme.

## Fonctionnalités

- Définition d'un modèle de Heston avec taux d'intérêt constant.
- Simulation des trajectoires du prix du sous-jacent et de sa variance ; seules les trajectoires de prix sont exportées.
- Calcul des payoffs européens : `max(S_T - K, 0)` pour un call et `max(K - S_T, 0)` pour un put.
- Estimation des prix par Monte-Carlo avec actualisation au taux du modèle.
- Export des trajectoires et de la grille de temps vers des fichiers texte, puis visualisation avec Matplotlib.

## Structure du projet

```text
Pricing_Model_Heston/
├── main.cpp                       # Paramètres et programme de démonstration
├── include/
│   ├── ModelHeston.hpp             # Classes de modèles à volatilité stochastique
│   ├── PathSimulatorHeston.hpp     # Interfaces et simulateur de trajectoires
│   ├── PayoffHeston.hpp            # Payoffs des options européennes
│   └── MonteCarloHeston.hpp        # Estimateur Monte-Carlo
├── src/
│   ├── ModelHeston.cpp
│   ├── PathSimulatorHeston.cpp
│   ├── PayoffHeston.cpp
│   └── MonteCarloHeston.cpp
├── tests/
│   └── test_discounting.cpp        # Tests de l'actualisation sur des payoffs connus
├── GrapheHeston.py                 # Lecture et tracé des trajectoires
├── pathsimul.txt                   # Trajectoires de prix, séparées par des lignes vides
├── pathsimul2.txt                  # Grille de temps commune aux trajectoires
└── README.md
```



## Prérequis

- Un compilateur C++ compatible **C++11**.
- **Python** et **Matplotlib** pour les graphiques uniquement.

## Compilation et exécution

### 1. Compiler

Depuis la racine du projet :

```sh
mkdir -p build
c++ -std=c++11 -O2 -Iinclude main.cpp src/*.cpp -o build/pricing_heston
```


### 2. Lancer la simulation

Toujours depuis la racine du projet :

```sh
./build/pricing_heston
```

Le programme affiche deux lignes, `Prix du CALL Europeen:` et `Prix du PUT Europeen:`, correspondant aux estimations des prix actualisés au premier instant de la grille, ici `t = 0`.


## Visualisation avec Python

Créer un environnement virtuel et installer Matplotlib depuis la racine du projet, sous macOS ou Linux :

```sh
python3 -m venv .venv
source .venv/bin/activate
python -m pip install matplotlib
python GrapheHeston.py
```

Le script lit `pathsimul.txt` et `pathsimul2.txt` dans le répertoire courant, puis ouvre une fenêtre affichant les trajectoires `S_t` en fonction du temps.



## Paramètres de l'exemple

Tous les paramètres sont définis dans [`main.cpp`](main.cpp).

```cpp
HestonModel2 model1(100.0, 3.0, 0.3, 0.2, 0.5, 0.1, 0.1);
//                  S0   kappa theta sigma rho   V0    r
```

| Paramètre | Signification | Valeur |
| --- | --- | --- |
| `S0` | Prix initial du sous-jacent | `100.0` |
| `kappa` | Vitesse de retour de la variance vers sa moyenne | `3.0` |
| `theta` | Niveau de variance à long terme | `0.3` |
| `sigma` | Coefficient de diffusion de la variance | `0.2` |
| `rho` | Paramètre de corrélation du modèle | `0.5` |
| `V0` | Variance initiale | `0.1` |
| `r` | Taux d'intérêt constant | `0.1` |
| `K` | Prix d'exercice des deux options | `105.0` |
| `T` | Maturité, dernier instant de la grille | `2.0` |
| `delta_t` | Pas de temps | `0.001` |
| Simulations Monte-Carlo | Nombre de trajectoires par option | `100` |
| Trajectoires exportées | Nombre de courbes pour le graphique | `20` |




