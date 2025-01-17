# """open the .uml and use plantuml.preview command or shortcut to see the diagram"""


import os
import hpp2plantuml

folder2watch = "include"
output_folder = os.path.join(os.path.dirname(os.path.abspath(__file__)), "..", "..", "docs", "classes")
os.makedirs(output_folder, exist_ok=True)  # Créer le dossier pour les fichiers individuels

hppFiles = [os.path.join(root, name) for root, dirs, files in os.walk(folder2watch) for name in files if name.endswith(".hpp")]

# Générer un fichier PlantUML global
global_puml_file = os.path.join(output_folder, "classDiagram.puml")
hpp2plantuml.CreatePlantUMLFile(hppFiles, global_puml_file)

# ouvrir le fichier PlantUML global comme un seul string
with open(global_puml_file, "r") as file:
    global_puml = file.read()
for hppFile in hppFiles:
    puml_file_path = os.path.join(output_folder, os.path.basename(hppFile).replace(".hpp", ".puml"))
    #exclude the files without classes
    hpp_file_content = open(hppFile, "r").read()
    if "class" in hpp_file_content:
        hpp2plantuml.CreatePlantUMLFile([hppFile], puml_file_path)

    

# Ajouter les options pour cacher les membres et méthodes après @startuml
with open(global_puml_file, "r") as file:
    lines = file.readlines()

# Trouver la ligne contenant @startuml
for i, line in enumerate(lines):
    if "@startuml" in line:
        lines.insert(i + 1, "hide members\nhide methods\n")  # Insérer juste après @startuml
        break

# Réécrire le fichier avec les modifications
with open(global_puml_file, "w") as file:
    file.writelines(lines)
