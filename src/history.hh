#pragma once

#include <string>

#include "constant.hh"

enum class InternalActionType
{
    /* Actions */
    action_avancer,            // avancer joueur_id troupe_id direction
    action_grandir,            // grandir joueur_id troupe_id
    action_construire_buisson, // buisson joueur_id troupe_id col ligne
    action_creuser_tunnel,     // creuser col ligne
    /* Debugging and errors */
    pigeon_debug, // debug col ligne niveau pigeon
    error,        // erreur joueur_id type_erreur
    /* Everything that can happen */
    info_score,           // score joueur_id nouvelle_valeur
    info_spawn,           // spawn joueur_id troupe_id col ligne taille
    info_recup_pains,     // recuperer joueur_id troupe_id col ligne niveau
    info_deposer_pains,   // deposer joueur_id troupe_id col ligne niveau
    info_recup_nid,       // nid joueur_id troupe_id col ligne
    info_lacher_pain,     // lacher joueur_id troupe_id col ligne niveau
    info_ouvrir_barriere, // ouvrir col ligne
    info_fermer_barriere, // fermer col ligne
    info_division,        // diviser joueur_id troupe_id nouvelle_taille
    info_dispersion,      // disperser joueur_id troupe_id
};

class InternalAction
{
public:
    friend std::ostream& operator<<(std::ostream&, const InternalAction&);

    InternalAction(InternalActionType, int, int, direction);
    InternalAction(InternalActionType, int, int);
    InternalAction(InternalActionType, int, int, position);
    InternalAction(InternalActionType, position);
    InternalAction(InternalActionType, position, pigeon_debug);
    InternalAction(InternalActionType, int, erreur);
    InternalAction(InternalActionType, int, int);
    InternalAction(InternalActionType, int, int, int);

private:
    InternalActionType type_;

    position pos_;
    direction direction_;
    pigeon_debug pigeon_;
    erreur err_;
    // Those values can be either player_id, troupe_id, taille, etc.
    // It's kind of ugly but it works...
    int int_value1_;
    int int_value2_;
    int int_value3_;
    int int_value4_;
};

std::ostream& operator<<(std::ostream& os, const InternalAction& action);
