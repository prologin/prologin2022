// SPDX-License-Identifier: GPL-2.0-or-later
// Copyright (c) 2012-2020 Association Prologin <association@prologin.org>

#include "api.hh"

#include <memory>
#include <utility>

// global used in interface.cc
Api* api;

Api::Api(std::unique_ptr<GameState> game_state,
         std::shared_ptr<rules::Player> player)
    : rules::Api<GameState, error>(std::move(game_state), player)
{
    api = this;
}

etat_case Api::info_case(position pos)
{
    // TODO
    abort();
}
etat_barriere Api::info_barriere(position pos)
{
    // TODO
    abort();
}
etat_nid Api::info_nid(position pos)
{
    // TODO
    abort();
}
int Api::papy_tours_restants(position pos)
{
    // TODO
    abort();
}
std::vector<troupe> Api::troupes_joueur(int id_joueur)
{
    // TODO
    abort();
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
    // TODO
    abort();
}
int Api::moi()
{
    // TODO
    abort();
}
int Api::adversaire()
{
    // TODO
    abort();
}
int Api::score(int id_joueur)
{
    // TODO
    abort();
}
bool Api::annuler()
{
    // TODO
    abort();
}
int Api::tour_actuel()
{
    // TODO
    abort();
}
void Api::afficher_erreur(erreur v)
{
    // TODO
    abort();
}
void Api::afficher_direction(direction v)
{
    // TODO
    abort();
}
void Api::afficher_type_case(type_case v)
{
    // TODO
    abort();
}
void Api::afficher_etat_barriere(etat_barriere v)
{
    // TODO
    abort();
}
void Api::afficher_etat_nid(etat_nid v)
{
    // TODO
    abort();
}
void Api::afficher_pigeon_debug(pigeon_debug v)
{
    // TODO
    abort();
}
void Api::afficher_type_action(type_action v)
{
    // TODO
    abort();
}
void Api::afficher_position(position v)
{
    // TODO
    abort();
}
void Api::afficher_troupe(troupe v)
{
    // TODO
    abort();
}
void Api::afficher_etat_case(etat_case v)
{
    // TODO
    abort();
}
void Api::afficher_action_hist(action_hist v)
{
    // TODO
    abort();
}
