// SPDX-License-Identifier: GPL-2.0-or-later
// Copyright (c) 2012-2020 Association Prologin <association@prologin.org>

#include "api.hh"
#include "position.hh"

#include <memory>
#include <utility>

// global used in interface.cc
Api* api;

Api::Api(std::unique_ptr<GameState> game_state,
         std::shared_ptr<rules::Player> player)
    : rules::Api<GameState, erreur>(std::move(game_state), player)
{
    api = this;
}

etat_case Api::info_case(position pos)
{
    if (!inside_map(pos))
    {
        const etat_case etatCase = {
            .pos = { -1, -1, -1 },
            .contenu = static_cast<type_case>(-1),
            .est_constructible = false,
            .nb_pains = false
        };

        return etatCase;
    }
    return game_state_->get_map().get_cell(pos).etat;
}
etat_barriere Api::info_barriere(position pos)
{
    if (!inside_map(pos))
        return PAS_DE_BARRIERE;

    return game_state_->get_map().get_cell(pos).barriere;
}
etat_nid Api::info_nid(position pos)
{
    if (!inside_map(pos))
        return PAS_DE_NID;

    return game_state_->get_map().get_cell(pos).nid;
}
int Api::papy_tours_restants(position pos)
{
    if (!inside_map(pos))
        return -1;

    return game_state_->get_map().get_cell(pos).papy_tours_restants;
}
std::vector<troupe> Api::troupes_joueur(int id_joueur)
{
    auto player = game_state_ -> get_player_ptr(id_joueur);
    std::vector<troupe> troupes;
    if (player == nullptr)
        return troupes;
    std::array<troupe, NB_TROUPES> troupesArr = player -> troupes();
    for (auto troupe: troupesArr)
        troupes.push_back(troupe);
    return troupes;
}
std::vector<position> Api::pains()
{
    // TODO
    abort();
}
std::vector<action_hist> Api::historique()
{
    // TODO
    abort();
}
int Api::gain(int nb_pains)
{
    return nb_pains * nb_pains;
}
int Api::moi()
{
    return player_->id;
}
int Api::adversaire()
{
    auto other_player = game_state_->get_other(player_->id);
    return other_player.get_key();
}
int Api::score(int id_joueur)
{
    auto player = game_state_->get_player_ptr(id_joueur);
    return player == nullptr ? -1 : player->get_score();
}
bool Api::annuler()
{
    return cancel();
}
int Api::tour_actuel()
{
    return game_state_->get_round();
}

std::ostream& operator<<(std::ostream& os, erreur v);
void Api::afficher_erreur(erreur v)
{
    std::cerr << v << std::endl;
}

std::ostream& operator<<(std::ostream& os, direction v);
void Api::afficher_direction(direction v)
{
    std::cerr << v << std::endl;
}

std::ostream& operator<<(std::ostream& os, type_case v);
void Api::afficher_type_case(type_case v)
{
    std::cerr << v << std::endl;
}

std::ostream& operator<<(std::ostream& os, etat_barriere v);
void Api::afficher_etat_barriere(etat_barriere v)
{
    std::cerr << v << std::endl;
}

std::ostream& operator<<(std::ostream& os, etat_nid v);
void Api::afficher_etat_nid(etat_nid v)
{
    std::cerr << v << std::endl;
}

std::ostream& operator<<(std::ostream& os, pigeon_debug v);
void Api::afficher_pigeon_debug(pigeon_debug v)
{
    std::cerr << v << std::endl;
}

std::ostream& operator<<(std::ostream& os, type_action v);
void Api::afficher_type_action(type_action v)
{
    std::cerr << v << std::endl;
}

std::ostream& operator<<(std::ostream& os, position v);
void Api::afficher_position(position v)
{
    std::cerr << v << std::endl;
}

std::ostream& operator<<(std::ostream& os, troupe v);
void Api::afficher_troupe(troupe v)
{
    std::cerr << v << std::endl;
}

std::ostream& operator<<(std::ostream& os, etat_case v);
void Api::afficher_etat_case(etat_case v)
{
    std::cerr << v << std::endl;
}

std::ostream& operator<<(std::ostream& os, action_hist v);
void Api::afficher_action_hist(action_hist v)
{
    std::cerr << v << std::endl;
}
