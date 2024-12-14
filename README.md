# PING² - Branche de développement Raspberry Pi

Cette branche contient le code de développement pour le Raspberry Pi, chargé de la gestion des interfaces utilisateur et de l'estimation de la position de la balle via la reconnaissance d'images.

## Fonctionnalités à développer

- [ ] Estimation de la position de la balle
- [ ] Reconnaissance d'images avec filtre de couleurs
- [ ] Gestion des manettes et boutons de paramétrage
- [ ] Gestion des indicateurs visuels (LED communicantes) et sonores
- [ ] Gestion des conditions de fin d'activité

## Structure des dossiers

...


## Installation et exécution

1. Cloner le dépôt et se positionner sur la branche `raspberry-dev` :
   ```bash
   git checkout raspberry-dev
   ```

2. Installer les dépendances :
   ```bash
   pip install -r requirements.txt
   ```

3. Exécuter le script principal :
   ```bash
   python main.py
   ```

## Contribution

### Pour les contributeurs du projet

Voici comment vous pouvez participer :

1. **Forker le dépôt** : Cliquez sur "Fork" en haut de cette page pour créer une copie de ce dépôt sur votre compte.
2. **Cloner votre fork** : Clonez votre fork sur votre machine avec la commande suivante :
   ```bash
   git clone https://github.com/votre-utilisateur/PING2.git
3. **Créer une nouvelle branche** : Pour ajouter une fonctionnalité ou corriger un bug, créez une nouvelle branche :
   ```bash
   git checkout -b ma-nouvelle-branche
4. **Faire vos modifications** : Apportez vos changements et validez-les dans votre branche.
5. **Soumettre une pull request** : Une fois les changements terminés, poussez votre branche vers votre fork et soumettez une pull request sur ce dépôt principal pour que nous puissions réviser votre travail.

Nous apprécions toute suggestion ou amélioration qui pourrait rendre PING² plus utile et performant. Pour discuter, un espace commentaire est disponible sur ce dépôt.

### Convention d'écriture
Chaque script doit être écrit en anglais : pas de franglais ! Ils doivent également contenir, au tout début, la mention de copyright.

Le choix des noms de variables ou d’objets est crucial. Un nom plus long, composé de mots bien choisis, est préférable à des abréviations. Afin de garantir un code propre, lisible et homogène, nous avons défini des conventions de nommage à respecter pour tout nouveau code ajouté au projet. Ces conventions s’appliquent aussi bien au code en C++ qu’en Python. Merci de privilégier la programmation orientée objet (POO) tout en respectant les conventions suivantes :

1 - Noms des classes et structures :
- Convention : ```PascalCase```. Tous les mots commencent par une majuscule.
- Exemple : ```MyClass```, ```Motors```.
   
2 - Noms des attributs et variables :
- Convention : ```camelCase```. Tous les mots, sauf le premier, commencent par une majuscule.
- Exemple : ```myAttribut```, ```currentSpeed```.

3 - Noms des méthodes et fonctions :
- Convention : ```snake_case```. Tous les mots sont en minuscules et séparés par des underscores.
- Exemple : ```compute_speed```, ```set_speed```.

4 - Noms des constantes :
- Convention : ```UPPER_SNAKE_CASE```. Tous les mots sont en majuscules et séparés par des underscores.
- Exemple : ```MAX_SPEED```, ```TIME_STEP```.

5 - Noms des fichiers :
- Convention : ```camelCase```. Comme pour les attributs et variables.
- Exemple : ```linearActuator.cpp```, ```ball_tracker.py```.
    
### Exemple :

Dans un fichier ```myClass.hpp``` :
```hpp
"""
This file is part of the PING² project.
Copyright (c) 2024 PING² Team

This code is licensed under the Attribution-NonCommercial-NoDerivatives 4.0 International (CC BY-NC-ND 4.0).
You may share this file as long as you credit the original author.

RESTRICTIONS:
- Commercial use is prohibited.
- No modifications or adaptations are allowed.
- See the full license at: https://creativecommons.org/licenses/by-nc-nd/4.0/

For inquiries, contact us at: projet.ping2@gmail.com
"""

#define MY_CONSTANT 100

class MyClass{
int myAttribut;
void my_methode();
};
```

## Licence

Ping²© 2024 by Antoine, Antoine, Robin, Simon, Thomas is licensed under [Creative Commons Attribution-NonCommercial-NoDerivatives 4.0 International](https://creativecommons.org/licenses/by-nc-nd/4.0/)
