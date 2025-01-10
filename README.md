# PING² - Branche de développement Raspberry Pi

Cette branche contient le code de développement pour le Raspberry Pi, chargé de la gestion des interfaces utilisateur et de l'estimation de la position de la balle via la reconnaissance d'images.

## Fonctionnalités à développer

- [ ] Estimation de la position de la balle
- [ ] Reconnaissance d'images avec filtre de couleurs
- [ ] Gestion des manettes et boutons de paramétrage
- [ ] Gestion des indicateurs visuels (LED communicantes) et sonores
- [ ] Gestion des conditions de fin d'activité

## Structure des dossiers
![image](https://github.com/user-attachments/assets/63302ffd-5dc2-40ce-90e0-7ddd2f351778)

- doc : tous les documents explicatifs de cette branche.
- src/config/ : fichiers de configuration nécessaires au fonctionnement du Raspberry Pi.
- src/init/ : trois fichiers principaux :
      
  - init.py : Script exécuté au démarrage du Raspberry Pi.
  - index.html : La page web utilisée pour permettre à l'utilisateur de connecter la plateforme à Internet. L'utilisateur entre les informations du réseau Wi-Fi via cette interface.
  - style.css : Le fichier CSS associé à la page HTML, qui permet de personnaliser l'apparence de l'interface utilisateur.

## Installation et exécution

Pour installer ces codes, il faut simplement mettre l'image du logiciel dans la carte micro SD de la Raspberry-pi.


## Licence

Ping²© 2024 by Antoine, Antoine, Robin, Simon, Thomas is licensed under [Creative Commons Attribution-NonCommercial-NoDerivatives 4.0 International](https://creativecommons.org/licenses/by-nc-nd/4.0/)
