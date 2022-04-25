// SPDX-License-Identifier: GPL-2.0-or-later
// Copyright (c) 2012-2020 Association Prologin <association@prologin.org>

#include "api.hh"

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