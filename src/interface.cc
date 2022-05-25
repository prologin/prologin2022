// SPDX-License-Identifier: GPL-2.0-or-later
// Copyright (c) 2012-2020 Association Prologin <association@prologin.org>

#include <iostream>
#include <sstream>
#include <vector>

#include "api.hh"

// from api.cc
extern Api* api;

template <typename T>
std::ostream& operator<<(std::ostream& os, const std::vector<T>& arr)
{
    os << "[";
    typename std::vector<T>::const_iterator it;
    for (it = arr.begin(); it != arr.end(); ++it)
    {
        if (it != arr.begin())
            os << ", ";
        os << *it;
    }
    os << "]";
    return os;
}

extern "C" erreur api_avancer(int id, direction dir)
{
    return api->avancer(id, dir);
}

extern "C" erreur api_grandir(int id)
{
    return api->grandir(id);
}

extern "C" erreur api_construire_buisson(position pos)
{
    return api->construire_buisson(pos);
}

extern "C" erreur api_creuser_tunnel(position pos)
{
    return api->creuser_tunnel(pos);
}

extern "C" etat_case api_info_case(position pos)
{
    return api->info_case(pos);
}

extern "C" etat_barriere api_info_barriere(position pos)
{
    return api->info_barriere(pos);
}

extern "C" etat_nid api_info_nid(position pos)
{
    return api->info_nid(pos);
}

extern "C" int api_papy_tours_restants(position pos)
{
    return api->papy_tours_restants(pos);
}

extern "C" std::vector<troupe> api_troupes_joueur(int id_joueur)
{
    return api->troupes_joueur(id_joueur);
}

extern "C" std::vector<position> api_pains()
{
    return api->pains();
}

extern "C" erreur api_debug_poser_pigeon(position pos, pigeon_debug pigeon)
{
    return api->debug_poser_pigeon(pos, pigeon);
}

extern "C" std::vector<action_hist> api_historique()
{
    return api->historique();
}

extern "C" int api_gain(int nb_pains)
{
    return api->gain(nb_pains);
}

extern "C" int api_moi()
{
    return api->moi();
}

extern "C" int api_adversaire()
{
    return api->adversaire();
}

extern "C" int api_score(int id_joueur)
{
    return api->score(id_joueur);
}

extern "C" bool api_annuler()
{
    return api->annuler();
}

extern "C" int api_tour_actuel()
{
    return api->tour_actuel();
}

extern "C" void api_afficher_erreur(erreur v)
{
    api->afficher_erreur(v);
}

extern "C" void api_afficher_direction(direction v)
{
    api->afficher_direction(v);
}

extern "C" void api_afficher_type_case(type_case v)
{
    api->afficher_type_case(v);
}

extern "C" void api_afficher_etat_barriere(etat_barriere v)
{
    api->afficher_etat_barriere(v);
}

extern "C" void api_afficher_etat_nid(etat_nid v)
{
    api->afficher_etat_nid(v);
}

extern "C" void api_afficher_pigeon_debug(pigeon_debug v)
{
    api->afficher_pigeon_debug(v);
}

extern "C" void api_afficher_type_action(type_action v)
{
    api->afficher_type_action(v);
}

extern "C" void api_afficher_position(position v)
{
    api->afficher_position(v);
}

extern "C" void api_afficher_troupe(troupe v)
{
    api->afficher_troupe(v);
}

extern "C" void api_afficher_etat_case(etat_case v)
{
    api->afficher_etat_case(v);
}

extern "C" void api_afficher_action_hist(action_hist v)
{
    api->afficher_action_hist(v);
}

std::ostream& operator<<(std::ostream& os, erreur v)
{
    switch (v)
    {
    case OK:
        os << "OK";
        break;
    case HORS_TOUR:
        os << "HORS_TOUR";
        break;
    case MOUVEMENTS_INSUFFISANTS:
        os << "MOUVEMENTS_INSUFFISANTS";
        break;
    case JOUEUR_INVALIDE:
        os << "JOUEUR_INVALIDE";
        break;
    case TROUPE_INVALIDE:
        os << "TROUPE_INVALIDE";
        break;
    case TROP_GRANDI:
        os << "TROP_GRANDI";
        break;
    case TROP_CREUSE:
        os << "TROP_CREUSE";
        break;
    case NON_CREUSABLE:
        os << "NON_CREUSABLE";
        break;
    case NON_CONSTRUCTIBLE:
        os << "NON_CONSTRUCTIBLE";
        break;
    case SCORE_INSUFFISANT:
        os << "SCORE_INSUFFISANT";
        break;
    case POSITION_INVALIDE:
        os << "POSITION_INVALIDE";
        break;
    case DIRECTION_INVALIDE:
        os << "DIRECTION_INVALIDE";
        break;
    case PIGEON_INVALIDE:
        os << "PIGEON_INVALIDE";
        break;
    }
    return os;
}

std::ostream& operator<<(std::ostream& os, direction v)
{
    switch (v)
    {
    case NORD:
        os << "NORD";
        break;
    case SUD:
        os << "SUD";
        break;
    case EST:
        os << "EST";
        break;
    case OUEST:
        os << "OUEST";
        break;
    case HAUT:
        os << "HAUT";
        break;
    case BAS:
        os << "BAS";
        break;
    }
    return os;
}

std::ostream& operator<<(std::ostream& os, type_case v)
{
    switch (v)
    {
    case GAZON:
        os << "GAZON";
        break;
    case BUISSON:
        os << "BUISSON";
        break;
    case BARRIERE:
        os << "BARRIERE";
        break;
    case NID:
        os << "NID";
        break;
    case PAPY:
        os << "PAPY";
        break;
    case TROU:
        os << "TROU";
        break;
    case TUNNEL:
        os << "TUNNEL";
        break;
    case TERRE:
        os << "TERRE";
        break;
    }
    return os;
}

std::ostream& operator<<(std::ostream& os, etat_barriere v)
{
    switch (v)
    {
    case OUVERTE:
        os << "OUVERTE";
        break;
    case FERMEE:
        os << "FERMEE";
        break;
    case PAS_DE_BARRIERE:
        os << "PAS_DE_BARRIERE";
        break;
    }
    return os;
}

std::ostream& operator<<(std::ostream& os, etat_nid v)
{
    switch (v)
    {
    case LIBRE:
        os << "LIBRE";
        break;
    case JOUEUR_0:
        os << "JOUEUR_0";
        break;
    case JOUEUR_1:
        os << "JOUEUR_1";
        break;
    case PAS_DE_NID:
        os << "PAS_DE_NID";
        break;
    }
    return os;
}

std::ostream& operator<<(std::ostream& os, pigeon_debug v)
{
    switch (v)
    {
    case PAS_DE_PIGEON:
        os << "PAS_DE_PIGEON";
        break;
    case PIGEON_BLEU:
        os << "PIGEON_BLEU";
        break;
    case PIGEON_JAUNE:
        os << "PIGEON_JAUNE";
        break;
    case PIGEON_ROUGE:
        os << "PIGEON_ROUGE";
        break;
    }
    return os;
}

std::ostream& operator<<(std::ostream& os, type_action v)
{
    switch (v)
    {
    case ACTION_AVANCER:
        os << "ACTION_AVANCER";
        break;
    case ACTION_GRANDIR:
        os << "ACTION_GRANDIR";
        break;
    case ACTION_CONSTRUIRE:
        os << "ACTION_CONSTRUIRE";
        break;
    case ACTION_CREUSER:
        os << "ACTION_CREUSER";
        break;
    }
    return os;
}

std::ostream& operator<<(std::ostream& os, position v)
{
    os << "{ ";
    os << "colonne"
       << "=" << v.colonne;
    os << ", ";
    os << "ligne"
       << "=" << v.ligne;
    os << ", ";
    os << "niveau"
       << "=" << v.niveau;
    os << " }";
    return os;
}

std::ostream& operator<<(std::ostream& os, troupe v)
{
    os << "{ ";
    os << "maman"
       << "=" << v.maman;
    os << ", ";
    os << "canards"
       << "=" << v.canards;
    os << ", ";
    os << "taille"
       << "=" << v.taille;
    os << ", ";
    os << "dir"
       << "=" << v.dir;
    os << ", ";
    os << "id"
       << "=" << v.id;
    os << " }";
    return os;
}

std::ostream& operator<<(std::ostream& os, etat_case v)
{
    os << "{ ";
    os << "pos"
       << "=" << v.pos;
    os << ", ";
    os << "contenu"
       << "=" << v.contenu;
    os << ", ";
    os << "est_constructible"
       << "=" << v.est_constructible;
    os << ", ";
    os << "nb_pains"
       << "=" << v.nb_pains;
    os << " }";
    return os;
}

std::ostream& operator<<(std::ostream& os, action_hist v)
{
    os << "{ ";
    os << "action_type"
       << "=" << v.action_type;
    os << ", ";
    os << "troupe_id"
       << "=" << v.troupe_id;
    os << ", ";
    os << "action_dir"
       << "=" << v.action_dir;
    os << ", ";
    os << "action_pos"
       << "=" << v.action_pos;
    os << " }";
    return os;
}
