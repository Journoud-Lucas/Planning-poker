Français | [English](README.md)

# 🃏 **Planning Poker**
![License](https://img.shields.io/badge/License-UNLICENSE-red) ![Latest Version](https://img.shields.io/badge/Version-1.0.0-blue) ![OS](https://img.shields.io/badge/OS-Windows%2FmacOS%2FLinux-green)

Le planning poker est une façon ludique de produire des estimations sur l'effort de développement de fonctionnalités. L'interface simple et épurée, permet facilement aux équipes d'estimer le temps des tâches voulues.

## 🎓 Projet d'école

Il s'agit d'un petit projet fait pour [l'Université Lumière Lyon 2](https://www.univ-lyon2.fr/).
En effet, nommé sous le nom de code projet CAPI, nous devions faire dans le langage désiré une application de planning projet tout en mettant en œuvre des pratiques de conception agile.

## 🔧 Fonctionnalités

- **Interface Utilisateur Intuitive :** Une interface simple ergonomique facilitant la gestion des sessions.
- **Sélection de cartes :** Choississez parmie une séléction de différentes cartes vous permettant de voter. La carte café est aussi présente, permettant de mettre en pause la session.
- **Modes de session :** 2 mode de jeu sont actuellement présent. "Strict" attendant que les joueurs sont tous d'accord sur l'estimation des tâches et le mode "Average", le premier tous étant en mode strict puis ont fait la moyenne des votes des joueurs.
- **JSON : ** Le format JSON est utilisé pour stocker les données. L'avantage d'un format comme celui-ci est qu'il pourrait être compatible avec d'autres applications dans le futur.
- **Visualisateur JSON : ** Un visualisateur JSON est intégré, permettant de visualiser le résultat de nos parties de planning poker facilement.

## 🚀 Utilisation

1. **Création de fichier JSON:** Créer un fichier JSON en choisissant son nom ainsi que les différentes tâches qui le compose.
2. **Sélection de fichiers JSON :** Choisissez un fichier JSON. S'il s'agit d'un JSON déjà finie (la session de planning poker a déjà été effectuée et est terminée), on ne pourra pas lancer de session.
3. **Lancement de sessions :** Commencez une session, et entrez les participants.
4. **Estimation des tâches :** Les joueurs font leur estimation tour a tour pour chaque tâches. Ils peuvent utiliser a tout moment la carte café pour mettre en pause le programme.
5. **Fin de la session :** Lorsqu'un concensus est atteint, la moyenne (ou autre dépendamment du mode choisi) est enregistré dans le JSON. Quand l'estimation a eu lieu pour chaque tâche, le JSON est terminé.

## 🛠️ Configuration de l'Environnement de Compilation

Pour compiler et exécuter l'application Planning Poker sur votre ordinateur, vous aurez besoin de certains outils et bibliothèques. Voici un guide pour vous aider à configurer votre environnement de développement :

### Prérequis

1. **Système d'exploitation requis :**
   - Windows
   - Le programme devrait fonctionner sous MAC et Linux, mais aucun test n'as été effectué dessus.

2. **Qt Framework :**
   - L'application utilise le framework Qt pour l'interface utilisateur. Vous devez installer Qt version 6.7.3. Vous pouvez le télécharger depuis [qt.io](https://www.qt.io/download).

3. **Compilateur C++ :**
   - Assurez-vous d'avoir un compilateur compatible avec C++17.
     J'ai utilisé MSVC 2022 v6.7.3 64bit, séléctionnés lors de l'installation de Qt.

4. **Modules Qt :**
   - Seuls les modules QT Core, QT GUI et QT Widgets qui sont dans l'installation de base de Qt ont été utilisés.

5. **Visual Studio :**
   - Afin d'utiliser Qt sous Visual Studio, vous devez installer l'extension Qt Visual Studio Tools, puis dans les paramètres de l'extension, allez dans `Versions` et ajoutez le chemin vers votre version de Qt. Exemple: `C:\Qt\6.7.3\msvc2022_64`

### Instructions de Compilation
1. **Compiler le projet :**
   - Sur **Windows** avec Visual Studio :
     Ouvrez le fichier `.sln` généré dans Visual Studio et construisez la solution.

2. **Exécuter l'application :**
   Une fois la compilation réussie, vous pouvez exécuter l'application.

## 📚 Documentation
Pour une documentation détaillée sur le projet, visitez le site [Documentation Planning Poker](https://journoud-lucas.github.io/Planning-poker).

## 🔢 Versionnage

Ce projet suit un schéma de [versionnement sémantique](https://semver.org/).

## 🤝 Crédits
Ce projet a été créé par:
- BESSAD Tarek
- JOURNOUD Lucas

## 📄 Licence

Ce logiciel est sous [Unlicense](https://unlicense.org/), dont les termes sont disponibles dans [UNLICENSE.txt](UNLICENSE.txt).

## 📜 Remerciements

Nous remercions tout particulièrement notre professeur de conception agile de projets informatiques, monsieur Valentin Lachand-Pascal.
