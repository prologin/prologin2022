// SPDX-License-Identifier: GPL-2.0-or-later
// Copyright (c) 2012-2020 Association Prologin <association@prologin.org>

#pragma once


/// Nombre de lignes dans la carte
#define HAUTEUR 80

/// Nombre de colonnes dans la carte
#define LARGEUR 80

/// Nombre de tours à jouer avant la fin de la partie
#define NB_TOURS 400

/// Taille de départ d'une troupe
#define TAILLE_DEPART 5

/// Taille minimale qu'une troupe peut avoir avant de se disperser
#define TAILLE_MIN 3

/// Nombre de troupes que chaque joueur controle
#define NB_TROUPES 2

/// Intervalle de distribution de pains par les papys
#define INTERVALLE_DISTRIB 4

/// Nombre de tunnels qu'un joueur peut creuser par tour
#define FREQ_TUNNEL 1

/// Nombre de déplacements que peut faire une troupe en un tour
#define PTS_MOUVEMENTS 20

/// Nombre de points de mouvement requis pour incrémenter la taille
#define COUT_CROISSANCE 10

/// Coût en pains de la pose de buisson
#define COUT_BUISSON 3

/// Tour au moment duquel les barrières s'ouvrent ou se ferment
#define TOUR_FERMETURE 100


/// Erreurs possibles après avoir effectué une action
typedef enum erreur
{
    OK, ///< L'action a été effectuée avec succès
    HORS_TOUR, ///< Aucune action n'est possible hors de joueur_tour
    MOUVEMENTS_INSUFFISANTS, ///< Il ne reste plus assez de points de mouvements pour effectuer l'action demandée
    TROP_GRANDI, ///< La troupe a déjà trop grandi pendant le tour
    TROP_CREUSE, ///< Trop de trous ont déjà été creusés pendant le tour
    NON_CREUSABLE, ///< Il n'est pas possible de creuser à la position demandée
    NON_CONSTRUCTIBLE, ///< La zone demandée n'est pas constructible
    PAINS_INSUFFISANTS, ///< La troupe n'a pas assez de pains pour construire un buisson
    POSITION_INVALIDE, ///< La position demandée est hors du parc
} erreur;

/// Directions possibles
typedef enum direction
{
    NORD, ///< Sens positif pour les lignes
    SUD, ///< Sens négatif pour les lignes
    EST, ///< Sens positif pour les colonnes
    OUEST, ///< Sens négatif pour les colonnes
    HAUT, ///< Sens positif pour le niveau
    BAS, ///< Sens négatif pour le niveau
} direction;

/// Type de l'élément présent sur une case
typedef enum type_case
{
    VIDE, ///< Absence d'élément
    BUISSON, ///< Obstacle impossible à traverser
    BARRIERE, ///< Élément pouvant être ouvert ou fermé. Une barrière fermée est infranchissable alors qu'une barrière ouverte est analogue à une case vide
    NID, ///< Élément traversable permettant à la troupe de déposer son inventaire en échange de points
    PAPY, ///< Élément traversable générant de manière périodique des miches de pain
    TROU, ///< Interface entre le niveau principal est le niveau souterrain
    TUNNEL, ///< Bloc du souterrain ayant été creusé
    TERRE, ///< Bloc du souterrain n'ayant pas encore été creusé
} type_case;

/// État d'une barrière, soit ouvert, soit fermé, soit non-applicable
typedef enum etat_barriere
{
    OUVERTE, ///< La barrière est ouverte
    FERMEE, ///< La barrière est fermée
    PAS_DE_BARRIERE, ///< L'élément dont on requiert l'état n'est pas une barrière
} etat_barriere;

/// Joueur auquel appartient un nid
typedef enum etat_nid
{
    LIBRE, ///< Le nid n'a pas été attribué
    JOUEUR_0, ///< Joueur 0
    JOUEUR_1, ///< Joueur 1
    PAS_DE_NID, ///< L'élément dont on requiert l'état n'est pas un nid
} etat_nid;

/// Type de pigeon de debug
typedef enum pigeon_debug
{
    AUCUN_PIGEON, ///< Aucun pigeon, enlève le pigeon présent
    PIGEON_BLEU, ///< Pigeon bleu
    PIGEON_JAUNE, ///< Pigeon jaune
    PIGEON_ROUGE, ///< Pigeon rouge
} pigeon_debug;

/// Types d'actions
typedef enum type_action
{
    ACTION_AVANCER, ///< Action ``avancer``
    ACTION_GRANDIR, ///< Action ``grandir``
    ACTION_CONSTRUIRE, ///< Action ``construire buisson``
    ACTION_CREUSER, ///< Action ``creuser tunnel``
} type_action;


/// Position dans la carte, donnée par trois coordonnées
typedef struct position
{
    int colonne; ///< Abscisse
    int ligne; ///< Ordonnée
    int niveau; ///< Niveau
} position;

/// Une troupe, composée de la maman canard et de ses canetons
typedef struct troupe
{
    position maman; ///< Position de la maman canard
    std::vector<position> canards; ///< Position des différents canards de la troupe, incluant la maman en première position
    int taille; ///< Taille de la troupe
    direction dir; ///< Direction de la troupe
    int id; ///< Identifiant de la troupe
} troupe;

/// Élément constituant le parc
typedef struct etat_case
{
    position pos; ///< Position de la case. Le niveau vaut nécessairement 0
    type_case contenu; ///< Type de la case
    bool est_constructible; ///< La case est constructible
    bool contient_pain; ///< La case contient une miche de pain
} etat_case;

/// Action représentée dans l'historique
typedef struct action_hist
{
    type_action action_type; ///< Type de l'action
    int troupe_id; ///< Identifiant de la troupe
    direction action_dir; ///< Direction de l'action
    position action_pos; ///< Position de l'action
} action_hist;
