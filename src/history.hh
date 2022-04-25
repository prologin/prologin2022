#pragma once

#include <string>

#include "constant.hh"

enum internal_action_id
{
   INT_ACTION_AVANCER,
   INT_ACTION_GRANDIR,
   INT_ACTION_CONSTRUIRE_BUISSON,
   INT_ACTION_CREUSER_TUNNEL,
   INT_ACTION_DEBUG_POSER_PIGEON,
};

class InternalAction
{

public:
    InternalAction(internal_action_id id, position pos);
    InternalAction(internal_action_id id, int colonne, int ligne, int niveau);

    //Getters
    internal_action_id get_id() const;

    //Methods
    std::string to_json();

private:
    internal_action_id id_;
    position pos_;
};
