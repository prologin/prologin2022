#!/usr/bin/env python3

from sys import exit, stderr

LONGUEUR = 40
INTERVALLE_DISTRIB = 5

grid = []
while True:
    try:
        line = list(input())
        grid.append(line)
    except EOFError:
        break

# Vérification :
# La carte est carrée

size = len(grid)
for i, line in enumerate(grid):
    longueur = len(line)
    if longueur != size:
        exit(f"La carte n'est pas carrée. La ligne {i} fait {longueur} caractère(s), alors que la carte a une hauteur de {size}.")

# Vérification : 
# La taille de la carte est la bonne

if size != LONGUEUR:
    exit(f"La carte donnée a une taille de {size}, et devrait avoir une taille de {LONGUEUR}.")

# Vérification :
# Aucune case inconnue

allowed = {'.', '#', '0', '1', '2', '3', '4', '5', '6', '7', '8', '9', 'X', 'N', 'S', ' ', 'b', 'B'}
for y in range(size):
    for x in range(size):
        if not grid[y][x] in allowed:
            exit(f"Caractère ligne {y} colonne {x} ({grid[y][x]}) inconnu.")

# Vérification :
# Aucun papy de phase trop élevée
digits = {str(i) for i in range(10)}
for y in range(size):
    for x in range(size):
        char = grid[y][x]
        if char in digits:
            if int(char) >= INTERVALLE_DISTRIB:
                exit(f"Papy ligne {y} colonne {x}, phase {char} supérieure à l'intervalle de distribution {INTERVALLE_DISTRIB}.")

# Vérification :
# Bordure composée exclusivement de murs ou point d'apparition
allowed = {'#', 'S'}
for y in range(size):
    if grid[y][0] not in allowed:
        exit(f"Caractère ligne {y} colonne 0 ({grid[y][0]}) interdit en bordure.")
    if grid[y][size-1] not in allowed:
        exit(f"Caractère ligne {y} colonne {size-1} ({grid[y][size-1]}) interdit en bordure.")
for x in range(size):
    if grid[0][x] not in allowed:
        exit(f"Caractère ligne 0 colonne {x} ({grid[0][x]}) interdit en bordure.")
    if grid[size-1][x] not in allowed:
        exit(f"Caractère ligne {size-1} colonne {x} ({grid[size-1][x]}) interdit en bordure.")

# Vérification :
# Pas de point d'apparition au cœur de la carte
for y in range(1, size-1):
    for x in range(1, size-1):
        if grid[y][x] == 'S':
            exit(f"Point d'apparition au centre de la carte ligne {y} colonne {x}.")

# Vérification :
# Pas de point d'apparition dans les coins de la carte
for y in [0, size-1]:
    for x in [0, size-1]:
        if grid[y][x] == 'S':
            exit(f"Point d'apparition en coin de la carte ligne {y} colonne {x}.")

# Vérification :
# Précisément 4 points d'apparitions présents
count = 0
for y in range(size):
    for x in range(size):
        if grid[y][x] == 'S':
            count += 1
if count != 4:
    exit(f"4 points d'apparitions sont attendus, {count} ont été trouvés.")

# Vérification :
# Précisément 1 point d'apparition par bordure
sx = None
for x in range(1, size-1):
    if grid[0][x] == 'S':
        if sx is not None:
            exit(f"Ligne 0, deux points d'apparitions sont trouvés colonne {sx} et {x}, alors qu'un seul est attendu.")
        else:
            sx = x
if sx is None:
    exit(f"Aucun point d'apparition n'a été trouvé ligne 0")

sx = None
for x in range(1, size-1):
    if grid[size-1][x] == 'S':
        if sx is not None:
            exit(f"Ligne {size-1}, deux points d'apparitions sont trouvés colonne {sx} et {x}, alors qu'un seul est attendu.")
        else:
            sx = x
if sx is None:
    exit(f"Aucun point d'apparition n'a été trouvé ligne {size-1}")

sy = None
for y in range(1, size-1):
    if grid[y][0] == 'S':
        if sy is not None:
            exit(f"Colonne 0, deux points d'apparitions sont trouvés ligne {sy} et {y}, alors qu'un seul est attendu.")
        else:
            sy = y
if sy is None:
    exit(f"Aucun point d'apparition n'a été trouvé colonne 0")

sy = None
for y in range(1, size-1):
    if grid[y][size-1] == 'S':
        if sy is not None:
            exit(f"Colonne {size-1}, deux points d'apparitions sont trouvés ligne {sy} et {y}, alors qu'un seul eset attendu.")
        else:
            sy = y
if sy is None:
    exit(f"Aucun point d'apparition n'a été trouvé colonne {size-1}")


