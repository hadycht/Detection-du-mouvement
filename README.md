# Detection-de-mouvements

Détection du mouvement à partir d'une séquence d'images (correspondante à une vidéo) en noir et blanc. 

## A propos du projet

Le projet consiste en deux étapes. Le première étape étant la détectiond du mouvement, et la deuxième est le Traitement du bruit.
Pour l'algorithme de détection de mouvement, on a utilisé l'agorithme Sigma-Delta (SD ou Σ∆). Ce dernier est détaillé dans le rapport du projet, n'hésitez à le voir. 
Pour le traitement du bruit, on a utilisé les algorithmes de morphologie: Erosion, Dilatation, Ouverture et Fermeture. Eux aussi sont détaillés dans le rapport. 
Pour implémenter les algorithmes de morphologie, on a utilisé plusieurs optimisations : Optimisations Domain Specific : fusion / factorisation d’opérateurs, Optimisations logicielles : pipeline d’opérateurs et Optimisation des formats de calcul et de stockage en mémoire. Celles ci sont détaillées dans le rapport et même dans l'énoncé du projet. 


## Fait avec

<img src="https://img.shields.io/badge/C-ED8B00?style=for-the-badge&logo=&logoColor=white" data-canonical-src="https://img.shields.io/badge/C-ED8B00?style=for-the-badge&logo=java&logoColor=white"/>

## Pour Commencer

Voici les étapes pour lancer le projet localement sur votre machine. Récuperer une copie du projet et suivez les différentes étapes ci-dessous.

### Prerequisites

OpenJDK (ou Java) doit être installer sur votre machine la version OpenJDK 17 minimum (ou Java 8 mais peut poser problème sur Windows [non recommandé] ).

#### Directement depuis le site web:

* #### Java : https://www.java.com/fr/

* #### OpenJDK : https://jdk.java.net/

#### Depuis le terminale:

* Debian/Ubuntu
  ```sh
  sudo apt update && sudo apt upgrade -y
  sudo apt-get install openjdk-17-jre
  ```
* Arch Linux
  ```sh
  sudo pacman -sS java | grep jre
  sudo pacman -S jre-openjdk-17-jre
  ```
* Windows (Powershell)
  ```sh
  winget install Microsoft.OpenJDK.17
  ```
* MacOS
  ```sh
  brew install openjdk@17
  ```

### Installation

#### Interface graphique

Décompresser et récuperer l'application .jar du fichier compresser.

#### Terminale de commande

Décompresser et extraire les fichiers dans un dossier.
## Utilisation/Exemples

#### Depuis l'interface graphique

Lancer l'application avec cette commande dans un terminal.
```shell
java -jar nomApplication.jar nomdufichiertrace.txt
```
Cliquer sur "Open File..." puis ouvrir le fichier trace au format texte à analyser. Ensuite lancer la visualisation en appuiant sur "Visualise Traffic".

#### Depuis le terminal

Ouvrir le fichier trace en argument dans le terminal lors du lancement du fichier contenant l'application de sortie terminal comme ci-dessous.

```shell
javac Test.java
java Test nomdufichiertrace.txt
```
#### Exemple

Un exemple de fichier trace est disponible pour tester l'application nommé trace.txt. Sous l'application avec l'interface graphique ouvrir directement le fichier et sous un terminal exécuter cette commande.

```shell
java Test trace.txt
```

## Capture d'écran

![Capture d'ecran](https://i.postimg.cc/Hkc4tRfr/Capture-d-cran-du-2023-06-03-14-27-18.png)
![Capture d'ecran 2](https://i.postimg.cc/DzZ1jDJV/Capture-d-cran-du-2023-06-03-14-32-44.png)

## Auteurs

- [@hadycht](https://github.com/hadycht)
- [@KyroFrCode](https://github.com/KyroFrCode)
