> [!WARNING]
> Répondre aux questions 1 & 2 avant la remise du TP.

# Système de gestion de bibliothèque - TP C++ 420-5G4-A26
Lapierre, Ian

## Fonctionnalités implémentées
- [x] **Interface et Expérience Utilisateur:** Détection des doublons plus intelligente (même titre, auteur, ISBN).
- [x] **Gestion des Données:** Import/Export CSV pour faciliter la gestion en lot (en plus du format actuel).
- [x] **Journal d'activités:** Fichier de journal de l'application conservé au fur et à mesure des sessions.
- [x] **Correction d'un bug:** Ne pas sauvegarder automatiquement à la sortie si --data-dir est absent.

## Questions 
### Question 1: C++
> Expliquez en détails une fonctionnalité / notion dans le code que ne nous avons pas ou peu vu en cours. Montrez ici l’exemple sorti du code du projet.

Le "logging" présent dans l'application présente une panoplie de notions du langage C++ n'ayant pas été consacré énormément de temps en cours. Notamment:
- La manipulation du temps (`<ctime>`)
- L'écriture dans un fichier (`ofstream`)
- Les enums

Tout d'abord, la première étape du processus de journalisation dans le programme est l'organisation de la date. Grâce à la librairie standard `<ctime>`, nous allons chercher la date actuelle sous forme de timestamp. Celle-ci est alors insérée lorsque nous ouvrons le fichier de log avec l'outil de flux de sortie présent dans le cadre de `<sstream>`. Pour être capable de catégoriser les types de journaux, une énum `LogType` est déclarée. Elle contient l'ensemble des possibilités d'opérations nécessitant une journalisation. Le log est alors appellé de cette manière:
```cpp
    Logger logger("app.log");
    logger.log(ADD_USER, name + "|" + userId); // ADD_USER = "AJOUT UTILISATEUR" dans l'enum
```

Dans cet exemple, Le flux de sortie pointe vers le fichier `app.log` où nous trouverons cette nouvelle entrée de journal.

### Question 2 : Options de développement possible
> Proposez une solution plus adaptée pour la gestion de bibliothèque et faisant appel éventuellement à une technologie autre que le C++, et expliquez comment vous interfaceriez ça avec le C++. Quelle solution technologique utiliseriez vous ? Pensez au futur de cette bibliothèque à Alexandrie qui pourrait éventuellement contenir des millions de livres.

Tout d'abord, il serait nécessaire de faire usage d'une base de données. L'état courant du stockage représente un réel danger à l'integrité des données. Notamment, il est impossible de s'assurer de la relations entre les entrées. Aussi, par la nature de la structure de l'application, une personne ayant accès à cette application a en main toutes les facettes administratives de celle-ci.

Ma solution serait de découpler cette application en 3: l'interface graphique (frontend), le backend et la base de données. 

- **Base de données:** PostgreSQL. Les clés étrangères et les contraintes d'unicité (ISBN, un seul emprunt actif par livre) garantissent l'intégrité des données. Les index permettent de gérer des millions de livres sans tout charger en mémoire.
- **Backend:** Une API REST qui contient le "business logic" (les opérations). Elle est la seule à parler à la base de données.
- **Frontend:** L'application C++ actuelle deviendrait un client de l'API. On garde son interface ou on peut la remplacer par une alternative comme Qt ou GTK.

Ainsi, un site web ou une application mobile pourraient utiliser le même backend, et la base de données pourrait être remplacée sans toucher au client. Même sans backend, la base données reste et ne peux brûler comme la bibliothèque d'Alexandrie. :)

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
