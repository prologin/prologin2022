// SPDX-License-Identifier: GPL-2.0-or-later
// Copyright (c) 2020 Association Prologin <association@prologin.org>

// This file contains the code to call the API functions from the C++ language.
// This file was generated by stechec2-generator. DO NOT EDIT.

#include "api.hh"

// This file only contains simple C++ wrappers around the API, that are
// basically already C++ functions, but with an `extern "C"`.

extern "C" erreur api_avancer(int id, direction dir);
erreur avancer(int id, direction dir)
{
    return api_avancer(id, dir);
}

extern "C" erreur api_grandir(int id);
erreur grandir(int id)
{
    return api_grandir(id);
}

extern "C" erreur api_construire_buisson(position pos);
erreur construire_buisson(position pos)
{
    return api_construire_buisson(pos);
}

extern "C" erreur api_creuser_tunnel(position pos);
erreur creuser_tunnel(position pos)
{
    return api_creuser_tunnel(pos);
}

extern "C" etat_case api_info_case(position pos);
etat_case info_case(position pos)
{
    return api_info_case(pos);
}

extern "C" etat_barriere api_info_barriere(position pos);
etat_barriere info_barriere(position pos)
{
    return api_info_barriere(pos);
}

extern "C" etat_nid api_info_nid(position pos);
etat_nid info_nid(position pos)
{
    return api_info_nid(pos);
}

extern "C" int api_papy_tours_restants(position pos);
int papy_tours_restants(position pos)
{
    return api_papy_tours_restants(pos);
}

extern "C" std::vector<troupe> api_troupes_joueur(int id_joueur);
std::vector<troupe> troupes_joueur(int id_joueur)
{
    return api_troupes_joueur(id_joueur);
}

extern "C" std::vector<position> api_pains();
std::vector<position> pains()
{
    return api_pains();
}

extern "C" erreur api_debug_poser_pigeon(position pos, pigeon_debug pigeon);
erreur debug_poser_pigeon(position pos, pigeon_debug pigeon)
{
    return api_debug_poser_pigeon(pos, pigeon);
}

extern "C" std::vector<action_hist> api_historique();
std::vector<action_hist> historique()
{
    return api_historique();
}

extern "C" int api_gain(int nb_pains);
int gain(int nb_pains)
{
    return api_gain(nb_pains);
}

extern "C" int api_moi();
int moi()
{
    return api_moi();
}

extern "C" int api_adversaire();
int adversaire()
{
    return api_adversaire();
}

extern "C" int api_score(int id_joueur);
int score(int id_joueur)
{
    return api_score(id_joueur);
}

extern "C" bool api_annuler();
bool annuler()
{
    return api_annuler();
}

extern "C" int api_tour_actuel();
int tour_actuel()
{
    return api_tour_actuel();
}

extern "C" void api_afficher_erreur(erreur v);
void afficher_erreur(erreur v)
{
    api_afficher_erreur(v);
}

extern "C" void api_afficher_direction(direction v);
void afficher_direction(direction v)
{
    api_afficher_direction(v);
}

extern "C" void api_afficher_type_case(type_case v);
void afficher_type_case(type_case v)
{
    api_afficher_type_case(v);
}

extern "C" void api_afficher_etat_barriere(etat_barriere v);
void afficher_etat_barriere(etat_barriere v)
{
    api_afficher_etat_barriere(v);
}

extern "C" void api_afficher_etat_nid(etat_nid v);
void afficher_etat_nid(etat_nid v)
{
    api_afficher_etat_nid(v);
}

extern "C" void api_afficher_pigeon_debug(pigeon_debug v);
void afficher_pigeon_debug(pigeon_debug v)
{
    api_afficher_pigeon_debug(v);
}

extern "C" void api_afficher_type_action(type_action v);
void afficher_type_action(type_action v)
{
    api_afficher_type_action(v);
}

extern "C" void api_afficher_position(position v);
void afficher_position(position v)
{
    api_afficher_position(v);
}

extern "C" void api_afficher_troupe(troupe v);
void afficher_troupe(troupe v)
{
    api_afficher_troupe(v);
}

extern "C" void api_afficher_etat_case(etat_case v);
void afficher_etat_case(etat_case v)
{
    api_afficher_etat_case(v);
}

extern "C" void api_afficher_action_hist(action_hist v);
void afficher_action_hist(action_hist v)
{
    api_afficher_action_hist(v);
}
