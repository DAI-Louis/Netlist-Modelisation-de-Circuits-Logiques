# Visualiseur de Cellules VLSI (Netlist Viewer)

Un outil graphique développé en **C++** et **Qt** permettant de visualiser des structures de circuits intégrés (Netlist). Ce projet affiche graphiquement des cellules, leurs instances, leurs symboles, ainsi que les interconnexions (Nets) et les terminaux (Pins).

## Fonctionnalités

* **Rendu Graphique (Immediate Mode avec QPainter) :**
  * Affichage des formes géométriques des instances (`BoxShape`, `EllipseShape`, `ArcShape`, `LineShape`).
  * Visualisation des connexions matérielles (fils/nets et nœuds).
* **Symbolique Intelligente des Terminaux :**
  * Rendu directionnel des terminaux (Entrées `<` vs Sorties `>`).
  * Affichage dynamique des noms des pins (ex: `sout`, `a`, `b`) positionnés par rapport aux fils de connexion.
  * Couleurs spécifiques pour distinguer les types de signaux (`Direction::In`, `Out`, `Tristate`, etc.).
* **Interface Utilisateur :**
  * Boîte de dialogue dynamique (`OpenCellDialog`) pour charger des cellules par leur nom.
  * Gestion du redimensionnement et de la navigation spatiale (traduction des coordonnées Modèle $\leftrightarrow$ Écran).

## Prérequis

Pour compiler et exécuter ce projet, vous aurez besoin de :

* **C++** (Support du standard C++11 minimum, C++17 recommandé)
* **Framework Qt** (Qt 5 ou Qt 6) - Modules: `Core`, `Gui`, `Widgets`
* **Make** ou **CMake** (selon votre système de build)
* Compilateur GCC 

## Compilation et Installation

### Avec QMake (Standard Qt)
```bash
# Clonez le dépôt
git clone [https://github.com/](https://github.com/)DAI-Louis/Netlist-Mod-lisation-de-Circuits-Logiques.git
cd Netlist-Mod-lisation-de-Circuits-Logiques

# Générez le Makefile
cd build 
cmake ../src

# Compilez
make install

# Exécutez
cd ../work 
./tme810

# Ou sinon utiliser le fichier exec.sh dans build pour compiler et exécuter

