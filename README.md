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


### 📬 Contact
Pour toute question ou clarification, n'hésitez pas à contacter l'équipe de développement ou à ouvrir une Issue sur le dépôt GitHub.

## Licence

Ping² © 2024 by Antoine, Antoine, Robin, Simon, Thomas is licensed under [Creative Commons Attribution-NonCommercial-NoDerivatives 4.0 International](https://creativecommons.org/licenses/by-nc-nd/4.0/)
