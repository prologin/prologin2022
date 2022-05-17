// SPDX-License-Identifier: GPL-2.0-or-later
// Copyright (c) 2012-2020 Association Prologin <association@prologin.org>

#pragma once

#include <memory>
#include <string>
#include <vector>

#include <rules/api.hh>
#include <rules/player.hh>

#include "actions.hh"
#include "constant.hh"
#include "game_state.hh"

/**
 * The methods of this class are exported through 'interface.cc'
 * to be called by the clients
 */
class Api final : public rules::Api<GameState, erreur>
{
public:
    Api(std::unique_ptr<GameState> game_state,
        std::shared_ptr<rules::Player> player);
    ~Api() {}

    /// La troupe avance d'une case vers une direction donnée
    ApiActionFunc<ActionAvancer> avancer{this};

    /// La troupe grandit
    ApiActionFunc<ActionGrandir> grandir{this};

    /// Construit un buisson à la position donnée
    ApiActionFunc<ActionConstruireBuisson> construire_buisson{this};

    /// Creuse un tunnel à la position donnée
    ApiActionFunc<ActionCreuserTunnel> creuser_tunnel{this};

    /// Renvoie les informations concernant une case
    etat_case info_case(position pos);

    /// Renvoie les informations d'état d'une barrière
    etat_barriere info_barriere(position pos);

    /// Renvoie les informations d'état d'un nid
    etat_nid info_nid(position pos);

    /// Renvoie le nombre de tours restants avant qu'un papy dépose une miche
    /// de pain. Retourne -1 si aucun papy ne se trouve à la position demandée
    int papy_tours_restants(position pos);

    /// Renvoie les troupes d'un joueur. Si le joueur est invalide, tous les
    /// champs valent -1.
    std::vector<troupe> troupes_joueur(int id_joueur);

    /// Renvoie la position des pains récupérables
    std::vector<position> pains();

    /// Pose un pigeon de debug sur la case indiquée
    ApiActionFunc<ActionDebugPoserPigeon> debug_poser_pigeon{this};

    /// Renvoie la liste des actions effectuées par l'adversaire durant son
    /// tour, dans l'ordre chronologique. Les actions de débug n'apparaissent
    /// pas dans cette liste.
    std::vector<action_hist> historique();

    /// Renvoie le gain en score que le nombre de pains passé en entrée
    /// rapporterait s'ils étaient tous déposés d'un coup dans un nid
    int gain(int nb_pains);

    /// Renvoie votre numéro de joueur.
    int moi();

    /// Renvoie le numéro du joueur adverse.
    int adversaire();

    /// Renvoie le score du joueur `id_joueur`. Renvoie -1 si le joueur est
    /// invalide.
    int score(int id_joueur);

    /// Annule la dernière action. Renvoie faux quand il n'y a pas d'action à
    /// annuler ce tour-ci
    bool annuler();

    /// Retourne le numéro du tour actuel.
    int tour_actuel();

    /// Affiche le contenu d'une valeur de type erreur
    void afficher_erreur(erreur v);

    /// Affiche le contenu d'une valeur de type direction
    void afficher_direction(direction v);

    /// Affiche le contenu d'une valeur de type type_case
    void afficher_type_case(type_case v);

    /// Affiche le contenu d'une valeur de type etat_barriere
    void afficher_etat_barriere(etat_barriere v);

    /// Affiche le contenu d'une valeur de type etat_nid
    void afficher_etat_nid(etat_nid v);

    /// Affiche le contenu d'une valeur de type pigeon_debug
    void afficher_pigeon_debug(pigeon_debug v);

    /// Affiche le contenu d'une valeur de type type_action
    void afficher_type_action(type_action v);

    /// Affiche le contenu d'une valeur de type position
    void afficher_position(position v);

    /// Affiche le contenu d'une valeur de type troupe
    void afficher_troupe(troupe v);

    /// Affiche le contenu d'une valeur de type etat_case
    void afficher_etat_case(etat_case v);

    /// Affiche le contenu d'une valeur de type action_hist
    void afficher_action_hist(action_hist v);
};

int gain(int nb_pains);
