> [!WARNING]
> Répondre aux questions 1 & 2 avant la remise du TP.

# Système de gestion de bibliothèque - TP C++ 420-5G4-A26
Lapierre, Ian

## Fonctionnalités implémentées
- **Interface et Expérience Utilisateur:** Détection des doublons plus intelligente (même titre, auteur, ISBN).
- **Gestion des Données:** Import/Export CSV pour faciliter la gestion en lot (en plus du format actuel).
- **Journal d'activités:** Fichier de journal de l'application conservé au fur et à mesure des sessions.
- **Correction d'un bug:** Ne pas sauvegarder automatiquement à la sortie si --data-dir est absent.

## Questions 
### Question 1: C++
*TODO*

### Question 2 : Options de développement possible
*TODO*

## Construire le projet
Voici les lignes de commandes pour compiler le projet:
```bash
mkdir build
cd build
cmake ..
make
```

## Répertoire data
L'application nécessite un répertoire lui permettant de stocker les informations de la bibliothèque. Celui-ci doit contenir les fichiers suivants:
- `books.txt` > Contient les livres.
- `users.txt` > Contient les utilisateurs.

Pour initialiser votre répertoire de données, exécuter:
```bash
mkdir data
touch data/books.txt data/users.txt
```

Pour lancer l'application tout en spécifiant un répertoire, exécuter:
```bash
bibliotheque -d data
```
