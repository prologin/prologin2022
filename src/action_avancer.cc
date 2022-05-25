// SPDX-License-Identifier: GPL-2.0-or-later
// Copyright (c) 2012-2020 Association Prologin <association@prologin.org>

#include "actions.hh"
#include "map.hh"
#include "position.hh"
#include "troupe.hh"

int ActionAvancer::check(const GameState& st) const
{
    if (!st.is_init())
        return HORS_TOUR;

    auto player = st.get_player(player_id_);
    auto trp = player.get_troupe(id_);

    if (trp == nullptr)
        return TROUPE_INVALIDE;

    if (player.pts_actions(trp->id) <= 0)
        return MOUVEMENTS_INSUFFISANTS;

    if (dir_ < 0 || (int) dir_ >= 6)
        return DIRECTION_INVALIDE;


    return OK;
}

void ActionAvancer::apply_on(GameState* st) const
{
    auto player = st->get_player_ptr(player_id_);
    auto trp = player->get_troupe(id_);

    // Gere la mort de la troupe
    move_troupe(*trp, dir_, st->get_map(), *player);

    // Log action.
    internal_action action;
    action.type = standard_action;
    action.action.action_type = ACTION_AVANCER;
    action.action.troupe_id = id_;
    action.action.action_dir = dir_;
    action.action.action_pos = {-1, -1, -1};
    player->add_internal_action(action);
}
