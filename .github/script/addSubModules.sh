#!/bin/bash

# Vérifier si on est sur la branche dev
CURRENT_BRANCH=$(git branch --show-current)
if [ "$CURRENT_BRANCH" != "dev" ]; then
    echo "❌ Vous devez être sur la branche 'dev' pour exécuter ce script."
    exit 1
fi

# Récupérer toutes les branches qui commencent par 'hardware/'
HARDWARE_BRANCHES=$(git branch -r | grep 'origin/hardware/' | sed 's/origin\///')

# Parcourir chaque branche et l'ajouter en tant que sous-module
for BRANCH in $HARDWARE_BRANCHES; do
    # Extraire le nom après 'hardware/'
    MODULE_NAME=$(echo $BRANCH | sed 's|hardware/||')
    FOLDER="$MODULE_NAME"
    
    echo "🔧 Ajout de la branche '$BRANCH' en tant que sous-module dans '$FOLDER'..."

    # Ajouter la branche comme sous-module à la racine
    git submodule add -b $BRANCH $(git remote get-url origin) $FOLDER

    # Ajouter le sous-module au suivi Git
    git add .gitmodules $FOLDER
    git commit -m "Ajout du sous-module pour la branche '$BRANCH' dans '$FOLDER'"
done

# Mise à jour des sous-modules
git submodule init
git submodule update --recursive

echo "✅ Toutes les branches 'hardware/' ont été ajoutées comme sous-modules à la racine avec succès."
