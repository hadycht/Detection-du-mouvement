
![Logo](https://i.postimg.cc/qq8RDWxG/th5xamgrr6se0x5ro4g6-1.png)



# VTRJava: Visualisateur Trafic Reseau Java

Visualisation des flux de trafic réseau à partir d'un fichier trace au format .txt

## A propos du projet

Le projet consiste à visualiser avec un interface graphique les flux de trafic réseau Ethernet à partir d'un fichier .txt contenant une/plusieurs trames Ethernet.

L'application peut analyser les couches suivantes:

- Ethernet
- IPv4
- TCP
- HTTP

le fichier texte à donner en entrer doit contenir les octets représentant une/les trame(s) capturés sur le réseau Ethernet (sans préambule ni champ FCS).

Voici un exemple de fichier trace au format texte possible en entrée :

```texte
0000  14 0c 76 7d 67 ea a8 a1 59 63 4e 51 08 00 45 00   ..v}g...YcNQ..E.
0010  00 28 74 16 40 00 80 06 00 00 c0 a8 00 0d a2 9f   .(t.@...........
0020  87 ea 04 00 01 bb 1d cc 5e b3 08 66 03 a0 50 10   ........^..f..P.
0030  04 00 eb 59 00 00                                 ...Y..
```

## Fait avec

<img src="https://img.shields.io/badge/Java-ED8B00?style=for-the-badge&logo=java&logoColor=white" data-canonical-src="https://img.shields.io/badge/Java-ED8B00?style=for-the-badge&logo=java&logoColor=white"/>

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
