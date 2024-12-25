# 🚀 Branche dev – Intégration et Tests des Codes Hardware
## 📚 Description de la Branche
La branche ```dev``` sert de plateforme d'intégration et de test pour tous les codes destinés aux différents matériels (hardware) du projet PING².

Elle regroupe les programmes à flasher sur chaque composant matériel du projet (ESP32, Raspberry Pi, Arduino Nano, etc.), tout en offrant un environnement centralisé pour les tests fonctionnels et la validation globale du système.

## 🛠️ Organisation de la Branche
Chaque matériel dispose de son propre dossier à la racine de cette branche, contenant le code spécifique à flasher et les instructions de déploiement :

```bash
dev/
├── esp32/      # Code pour l'ESP32
├── raspberry/  # Code pour le Raspberry Pi
├── UICorner/   # Code pour le coin d'interface utilisateur
└── README.md   # Ce fichier explicatif (ici)
```

## ✅ Objectif de la Branche ```dev```
- Centraliser les développements matériels.
- Assurer une intégration fonctionnelle entre les différents matériels.
- Valider la stabilité et la fiabilité du système avant un déploiement final.
  
## 🔄 Workflow de Développement
- Développement Fonctionnel : Chaque branche ```hardware/*``` est dédiée au développement et à l'amélioration du code pour un matériel spécifique.
- Intégration dans dev : Les branches ```hardware/*``` sont ajoutées en tant que sous-modules à la branche dev. **Aucune modification du code source ne doit être effectué depuis la branche ```dev```.**
- Phase de Test : Les codes sont testés ensemble pour s'assurer qu'ils fonctionnent de manière cohérente sur l'ensemble du système.
- Validation : Une fois les tests concluants, la branche dev est fusionnée (merge) dans la branche main, qui représente alors la version stable et prête pour la production du projet.

### 🎯 En Résumé :
dev = Branche de test et d'intégration du code hardware.

main = Branche stable pour la version finale.

## 📄 Instructions pour les Contributeurs
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
- Exemple : ```linearActuator.cpp```, ```ballTracker.py```.
    
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

### 📬 Contact
Pour toute question ou clarification, n'hésitez pas à contacter l'équipe de développement ou à ouvrir une Issue sur le dépôt GitHub.

## Licence

Ping² © 2024 by Antoine, Antoine, Robin, Simon, Thomas is licensed under [Creative Commons Attribution-NonCommercial-NoDerivatives 4.0 International](https://creativecommons.org/licenses/by-nc-nd/4.0/)
