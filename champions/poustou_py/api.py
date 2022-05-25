# This file was generated by stechec2-generator. DO NOT EDIT.

from enum import IntEnum
from typing import NamedTuple, List, Tuple  # noqa: F401

from _api import *


# Nombre de lignes dans la carte
HAUTEUR = 40

# Nombre de colonnes dans la carte
LARGEUR = 40

# Nombre de tours à jouer avant la fin de la partie
NB_TOURS = 400

# Taille de départ d'une troupe
TAILLE_DEPART = 5

# Taille minimale qu'une troupe peut avoir avant de se disperser
TAILLE_MIN = 3

# Nombre de troupes que chaque joueur controle
NB_TROUPES = 2

# Intervalle de distribution de pains par les papys
INTERVALLE_DISTRIB = 5

# Nombre de tunnels qu'un joueur peut creuser par tour
FREQ_TUNNEL = 1

# Nombre de déplacements que peut faire une troupe en un tour
PTS_ACTION = 20

# Nombre de points de mouvement requis pour incrémenter la taille
COUT_CROISSANCE = 11

# Coût en score de la pose de buisson
COUT_BUISSON = 3

# Tour au moment duquel les barrières s'ouvrent ou se ferment
TOUR_FERMETURE = 100


# Erreurs possibles après avoir effectué une action
class erreur(IntEnum):
    OK = 0  # <- L'action a été effectuée avec succès
    JOUEUR_INVALIDE = 1  # <- Mauvais numéro de joueur
    TROUPE_INVALIDE = 2  # <- Mauvais identifiant de troupe
    HORS_TOUR = 3  # <- Aucune action n'est possible hors de joueur_tour
    MOUVEMENTS_INSUFFISANTS = 4  # <- Il ne reste plus assez de points de mouvements pour effectuer l'action demandée
    TROP_GRANDI = 5  # <- La troupe a déjà trop grandi pendant le tour
    TROP_CREUSE = 6  # <- Trop de trous ont déjà été creusés pendant le tour
    NON_CREUSABLE = 7  # <- Il n'est pas possible de creuser à la position demandée
    NON_CONSTRUCTIBLE = 8  # <- La zone demandée n'est pas constructible
    SCORE_INSUFFISANT = 9  # <- Le joueur n'a pas assez de points pour construire un buisson
    POSITION_INVALIDE = 10  # <- La position demandée est hors du parc
    DIRECTION_INVALIDE = 11  # <- La direction spécifiée n'existe pas.
    PIGEON_INVALIDE = 12  # <- Le pigeon spécifié n'existe pas.


# Directions possibles
class direction(IntEnum):
    NORD = 0  # <- Sens positif pour les lignes
    SUD = 1  # <- Sens négatif pour les lignes
    EST = 2  # <- Sens positif pour les colonnes
    OUEST = 3  # <- Sens négatif pour les colonnes
    HAUT = 4  # <- Sens positif pour le niveau
    BAS = 5  # <- Sens négatif pour le niveau


# Type de l'élément présent sur une case
class type_case(IntEnum):
    GAZON = 0  # <- Absence d'élément
    BUISSON = 1  # <- Obstacle impossible à traverser
    BARRIERE = 2  # <- Élément pouvant être ouvert ou fermé. Une barrière fermée est infranchissable alors qu'une barrière ouverte est analogue à une case vide
    NID = 3  # <- Élément traversable permettant à la troupe de déposer son inventaire en échange de points
    PAPY = 4  # <- Élément traversable générant de manière périodique des miches de pain
    TROU = 5  # <- Interface entre le niveau principal est le niveau souterrain
    TUNNEL = 6  # <- Bloc du souterrain ayant été creusé
    TERRE = 7  # <- Bloc du souterrain n'ayant pas encore été creusé


# État d'une barrière, soit ouvert, soit fermé, soit non-applicable
class etat_barriere(IntEnum):
    OUVERTE = 0  # <- La barrière est ouverte
    FERMEE = 1  # <- La barrière est fermée
    PAS_DE_BARRIERE = 2  # <- L'élément dont on requiert l'état n'est pas une barrière


# Joueur auquel appartient un nid
class etat_nid(IntEnum):
    LIBRE = 0  # <- Le nid n'a pas été attribué
    JOUEUR_0 = 1  # <- Joueur 0
    JOUEUR_1 = 2  # <- Joueur 1
    PAS_DE_NID = 3  # <- L'élément dont on requiert l'état n'est pas un nid


# Type de pigeon de debug
class pigeon_debug(IntEnum):
    PAS_DE_PIGEON = 0  # <- Aucun pigeon, enlève le pigeon présent
    PIGEON_BLEU = 1  # <- Pigeon bleu
    PIGEON_JAUNE = 2  # <- Pigeon jaune
    PIGEON_ROUGE = 3  # <- Pigeon rouge


# Types d'actions
class type_action(IntEnum):
    ACTION_AVANCER = 0  # <- Action ``avancer``
    ACTION_GRANDIR = 1  # <- Action ``grandir``
    ACTION_CONSTRUIRE = 2  # <- Action ``construire buisson``
    ACTION_CREUSER = 3  # <- Action ``creuser tunnel``


# Une troupe, composée de la maman canard et de ses canetons
class troupe(NamedTuple):
    maman: Tuple[int, int, int]  # Position de la maman canard
    canards: List[Tuple[int, int, int]]  # Position des différents canards de la troupe, incluant la maman en première position
    taille: int  # Taille de la troupe
    dir: direction  # Direction de la troupe
    inventaire: int  # Nombre de pains de la troupe
    pts_action: int  # Nombre de points d'action de la troupe
    id: int  # Identifiant de la troupe


# Élément constituant le parc
class etat_case(NamedTuple):
    pos: Tuple[int, int, int]  # Position de la case. Le niveau vaut nécessairement 0
    contenu: type_case  # Type de la case
    est_constructible: bool  # La case est constructible
    nb_pains: int  # Nombre de pains contenus sur la case


# Action représentée dans l'historique
class action_hist(NamedTuple):
    action_type: type_action  # Type de l'action
    troupe_id: int  # Identifiant de la troupe
    action_dir: direction  # Direction de l'action
    action_pos: Tuple[int, int, int]  # Position de l'action


