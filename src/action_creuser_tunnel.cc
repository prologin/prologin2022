// SPDX-License-Identifier: GPL-2.0-or-later
// Copyright (c) 2012-2020 Association Prologin <association@prologin.org>

#include "actions.hh"
#include "constant.hh"
#include "position.hh"

int ActionCreuserTunnel::check(const GameState& st) const
{
    if (!st.is_init())
        return HORS_TOUR;

    if (!inside_map(pos_))
        return POSITION_INVALIDE;

    if (st.get_map().get_cell(pos_).etat.contenu != TERRE)
        return NON_CREUSABLE;

    if (st.get_player(player_id_).get_tunnels() >= FREQ_TUNNEL)
        return TROP_CREUSE;

    return OK;
}

void ActionCreuserTunnel::apply_on(GameState* st) const
{
    auto player = st->get_player_ptr(player_id_);

    player->increment_tunnels();
    st->get_map().get_cell(pos_).etat.contenu = TUNNEL;

    // Log action.
    internal_action action;
    action.type = standard_action;
    action.action.action_type = ACTION_CREUSER;
    action.action.troupe_id = -1;
    // TODO init action.action.action_dir;
    action.action.action_pos = pos_;
    player->add_internal_action(action);
}
