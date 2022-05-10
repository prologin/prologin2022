#pragma once

#include <string>

#include "constant.hh"

enum class InternalActionType
{
    /* Actions */
    action_avancer,            // avancer troupe_id direction
    action_grandir,            // grandir troupe_id
    action_construire_buisson, // buisson col ligne
    action_creuser_tunnel,     // creuser col ligne
    action_debug_poser_pigeon, // debug col ligne niveau
    /* Everything that can happen */
    info_score,           // score player_id new_value
    info_spawn,           // spawn joueur_id troupe_id col ligne taille
    info_recup_pains,     // recuperer col ligne niveau
    info_deposer_pains,   // deposer col ligne niveau
    info_recup_nid,       // nid col ligne
    info_lacher_pain,     // lacher col ligne niveau
    info_ouvrir_barriere, // ouvrir col ligne
    info_fermer_barriere, // fermer col ligne
    info_division,        // diviser troupe_id nouvelle_taille
    info_dispersion,      // disperser troupe_id
};

class InternalAction
{
public:
    friend std::ostream& operator<<(std::ostream&, const InternalAction&);

    InternalAction(InternalActionType act, int id, direction direction);
    InternalAction(InternalActionType act, int id);
    InternalAction(InternalActionType act, position pos);
    InternalAction(InternalActionType act, int id, int value);
    InternalAction(InternalActionType act, int player_id, int troupe_id,
                   int value, position pos);

private:
    InternalActionType type_;

    position pos_;
    direction direction_;
    // Those values can be either player_id, troupe_id, taille, etc.
    // It's kind of ugly but it works...
    int int_value1_;
    int int_value2_;
    int int_value3_;
};

std::ostream& operator<<(std::ostream& os, const InternalAction& action);
