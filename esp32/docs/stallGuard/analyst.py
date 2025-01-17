import pandas as pd
import numpy as np
import matplotlib.pyplot as plt
from mpl_toolkits.mplot3d import Axes3D  # Pour les graphiques 3D
import seaborn as sns  # Pour les heatmaps

# Charger les données depuis un fichier CSV
file_path = "stall_guard_data.csv"  # Remplacer par le chemin de ton fichier
df = pd.read_csv(file_path)
# Étape 1 : Calculer les min et max de stall_guard pour chaque couple (velocity, run_current)
sub_df = df.groupby(["velocity", "run_current"])["stall_guard"]

min_values = (
    sub_df
    .min()
    .reset_index()
    .rename(columns={"stall_guard": "stall_guard_min"})
)

max_values = (
    sub_df
    .max()
    .reset_index()
    .rename(columns={"stall_guard": "stall_guard_max"})
)

print("Résumé des valeurs min de stall_guard :")
print(min_values)

print("\nRésumé des valeurs max de stall_guard :")
print(max_values)


# Scatter plot pour MIN
# Étape 2 : Visualisation en 3D des relations pour les min et max sur le même graphique
fig = plt.figure(figsize=(12, 6))

# Scatter plot pour MIN
ax = fig.add_subplot(111, projection="3d")
ax.scatter(min_values["velocity"], min_values["run_current"], min_values["stall_guard_min"], 
           c='blue', label="Min Stall Guard", s=50)

# Scatter plot pour MAX
ax.scatter(max_values["velocity"], max_values["run_current"], max_values["stall_guard_max"], 
           c='red', label="Max Stall Guard", s=50)

# Ajouter les labels et le titre
ax.set_xlabel("Velocity")
ax.set_ylabel("Run Current")
ax.set_zlabel("Stall Guard")
ax.set_title("Min et Max Stall Guard en fonction de Velocity et Run Current")

# Ajouter une légende
ax.legend()

# Afficher le graphique
plt.tight_layout()
plt.show()


# Étape 3 : Heatmap pour identifier les tendances (min, max et ratio) sur la même figure

# Créer une figure avec plusieurs sous-graphes
plt.figure(figsize=(10, 6))
# Heatmap pour MIN
pivot_min = min_values.pivot("velocity", "run_current", "stall_guard_min")
sns.heatmap(pivot_min, annot=False, cmap="coolwarm", cbar_kws={'label': 'Min Stall Guard'})
plt.title("Heatmap des valeurs Min Stall Guard")
plt.xlabel("Run Current")
plt.ylabel("Velocity")

# Ajuster l'espacement entre les sous-graphes
plt.tight_layout()
plt.show()

# Affichage des résultats du ratio
# print("\nRésumé des ratios min / (max - min) :")
# print(merged_values)

# afficher un exemple avec velocity=19000 et run_current=70
# Filtrer les données pour velocity = 19000 et run_current = 70
filtered_df = df[(df["velocity"] == 19000) & (df["run_current"] == 70)]

# Vérifier si des données existent pour cette combinaison
if not filtered_df.empty:
    plt.figure(figsize=(10, 6))
    plt.plot(filtered_df["time"], filtered_df["stall_guard"], marker="o", label="Stall Guard")
    plt.title("Samples of stall_guard for velocity=19000 and run_current=70")
    plt.xlabel("Time(s)")
    plt.ylabel("Stall Guard")
    plt.legend()
    plt.grid()
    plt.show()
