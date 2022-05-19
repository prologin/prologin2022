#include "history.hh"

#include <iostream>

namespace
{
    InternalActionType action_to_internal_action(type_action action)
    {
        InternalActionType internal_action;

        switch (action)
        {
        case ACTION_AVANCER:
            internal_action = InternalActionType::action_avancer;
            break;
        case ACTION_GRANDIR:
            internal_action = InternalActionType::action_grandir;
            break;
        case ACTION_CONSTRUIRE:
            internal_action = InternalActionType::action_construire_buisson;
            break;
        case ACTION_CREUSER:
            internal_action = InternalActionType::action_creuser_tunnel;
            break;
        }

        return internal_action;
    }

    std::string pigeon_string(const pigeon_debug& pigeon)
    {
        std::string s;

        switch (pigeon)
        {
        case PAS_DE_PIGEON:
            s = "NONE";
            break;
        case PIGEON_BLEU:
            s = "BLEU";
            break;
        case PIGEON_JAUNE:
            s = "JAUNE";
            break;
        case PIGEON_ROUGE:
            s = "ROUGE";
            break;
        }

        return s;
    }

    std::string erreur_msg(const erreur& err)
    {
        std::string s;

        switch (err)
        {
        case OK:
            s = "L'action a été effectuée avec succès";
            break;
        case JOUEUR_INVALIDE:
            s = "Mauvais numéro de joueur";
            break;
        case TROUPE_INVALIDE:
            s = "Mauvais identifiant de troupe";
            break;
        case HORS_TOUR:
            s = "Aucune action n'est possible hors de joueur_tour";
            break;
        case MOUVEMENTS_INSUFFISANTS:
            s = "Il ne reste plus assez de points de mouvements";
            s += " pour effectuer l'action demandée";
            break;
        case TROP_GRANDI:
            s = "La troupe a déjà trop grandi pendant le tour";
            break;
        case TROP_CREUSE:
            s = "Trop de trous ont déjà été creusés pendant le tour";
            break;
        case NON_CREUSABLE:
            s = "Il n'est pas possible de creuser à la position demandée";
            break;
        case NON_CONSTRUCTIBLE:
            s = "La zone demandée n'est pas constructible";
            break;
        case SCORE_INSUFFISANT:
            s = "La troupe n'a pas assez de pains pour construire un buisson";
            break;
        case POSITION_INVALIDE:
            s = "La position demandée est hors du parc";
            break;
        }

        return s;
    }
} // namespace

InternalAction::InternalAction(InternalActionType type, int val1, int val2)
    : type_(type)
    , int_value1_(val1)
    , int_value2_(val2)
{}

InternalAction::InternalAction(InternalActionType type, int val1, int val2,
                               int val3)
    : type_(type)
    , int_value1_(val1)
    , int_value2_(val2)
    , int_value3_(val3)
{}

InternalAction::InternalAction(InternalActionType type, int val1, int val2,
                               direction dir)
    : type_(type)
    , direction_(dir)
    , int_value1_(val1)
    , int_value2_(val2)
{}

InternalAction::InternalAction(InternalActionType type, int val1, int val2,
                               position pos)
    : type_(type)
    , pos_(pos)
    , int_value1_(val1)
    , int_value2_(val2)
{}

InternalAction::InternalAction(InternalActionType type, int val1, int val2,
                               position pos, int val3)
    : type_(type)
    , pos_(pos)
    , int_value1_(val1)
    , int_value2_(val2)
    , int_value3_(val3)
{}

InternalAction::InternalAction(InternalActionType type, position pos)
    : type_(type)
    , pos_(pos)
{}

InternalAction::InternalAction(InternalActionType type, position pos,
                               pigeon_debug pigeon)
    : type_(type)
    , pos_(pos)
    , pigeon_(pigeon)
{}

InternalAction::InternalAction(InternalActionType type, int val, erreur err)
    : type_(type)
    , err_(err)
    , int_value1_(val)
{}

InternalAction::InternalAction(action_hist action)
    : type_(action_to_internal_action(action.action_type))
    , pos_(action.action_pos)
    , direction_(action.action_dir)
    , int_value1_(action.troupe_id)
{}

std::ostream& operator<<(std::ostream& os, const InternalAction& action)
{
    switch (action.type_)
    {
    case InternalActionType::action_avancer:
        os << "avancer"
           << ' ' << action.int_value1_ // joueur_id
           << ' ' << action.int_value2_ // troupe_id
           << ' ' << action.direction_; // direction
        break;
    case InternalActionType::action_grandir:
        os << "grandir"
           << ' ' << action.int_value1_  // joueur_id
           << ' ' << action.int_value2_; // troupe_id
        break;
    case InternalActionType::action_construire_buisson:
        os << "buisson"
           << ' ' << action.int_value1_  // joueur_id
           << ' ' << action.int_value2_  // troupe_id
           << ' ' << action.pos_.colonne // col
           << ' ' << action.pos_.ligne;  // ligne
        break;
    case InternalActionType::action_creuser_tunnel:
        os << "creuser"
           << ' ' << action.pos_.colonne // col
           << ' ' << action.pos_.ligne;  // ligne
        break;
    case InternalActionType::pigeon_debug:
        os << "debug"
           << ' ' << action.pos_.colonne            // col
           << ' ' << action.pos_.ligne              // ligne
           << ' ' << action.pos_.niveau             // niveau
           << ' ' << pigeon_string(action.pigeon_); // pigeon
        break;
    case InternalActionType::error:
        os << "erreur"
           << " " << action.int_value1_       // joueur_id
           << ' ' << erreur_msg(action.err_); // type_erreur
        break;
    case InternalActionType::info_score:
        os << "score"
           << ' ' << action.int_value1_  // joueur_id
           << ' ' << action.int_value2_; // nouvelle_valeur
        break;
    case InternalActionType::info_spawn:
        os << "spawn"
           << ' ' << action.int_value1_  // joueur_id
           << ' ' << action.int_value2_  // troupe_id
           << ' ' << action.pos_.colonne // col
           << ' ' << action.pos_.ligne   // ligne
           << ' ' << action.int_value3_; // taille
        break;
    case InternalActionType::info_recup_pains:
        os << "recuperer"
           << ' ' << action.int_value1_  // joueur_id
           << ' ' << action.int_value2_  // troupe_id
           << ' ' << action.pos_.colonne // col
           << ' ' << action.pos_.ligne   // ligne
           << ' ' << action.pos_.niveau; // niveau
        break;
    case InternalActionType::info_deposer_pains:
        os << "deposer"
           << ' ' << action.int_value1_  // joueur_id
           << ' ' << action.int_value2_  // troupe_id
           << ' ' << action.pos_.colonne // col
           << ' ' << action.pos_.ligne   // ligne
           << ' ' << action.pos_.niveau; // niveau
        break;
    case InternalActionType::info_recup_nid:
        os << "nid"
           << ' ' << action.int_value1_  // joueur_id
           << ' ' << action.int_value2_  // troupe_id
           << ' ' << action.pos_.colonne // col
           << ' ' << action.pos_.ligne;  // ligne
        break;
    case InternalActionType::info_lacher_pain:
        os << "lacher"
           << ' ' << action.int_value1_  // joueur_id
           << ' ' << action.int_value2_  // troupe_id
           << ' ' << action.pos_.colonne // col
           << ' ' << action.pos_.ligne   // ligne
           << ' ' << action.pos_.niveau; // niveau
        break;
    case InternalActionType::info_ouvrir_barriere:
        os << "ouvrir"
           << ' ' << action.pos_.colonne // col
           << ' ' << action.pos_.ligne;  // ligne
        break;
    case InternalActionType::info_fermer_barriere:
        os << "fermer"
           << ' ' << action.pos_.colonne // col
           << ' ' << action.pos_.ligne;  // ligne
        break;
    case InternalActionType::info_division:
        os << "diviser"
           << ' ' << action.int_value1_  // joueur_id
           << ' ' << action.int_value2_  // troupe_id
           << ' ' << action.int_value3_; // nouvelle_taille
        break;
    case InternalActionType::info_dispersion:
        os << "disperser"
           << ' ' << action.int_value1_  // joueur_id
           << ' ' << action.int_value2_; // troupe_id
        break;
    }

    return os;
}