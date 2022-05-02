// SPDX-License-Identifier: GPL-2.0-or-later
// Copyright (c) 2012-2020 Association Prologin <association@prologin.org>

#include "actions.hh"
#include "constant.hh"

int ActionCreuserTunnel::check(const GameState& st) const
{
    if (!inside_map(pos_))
        return POSITION_INVALIDE;

    if (!st.get_map().get_cell(pos_).etat.contenu != TERRE)
        return NON_CREUSABLE;

    if (st.get_player(player_id_).get_tunnels() >= FREQ_TUNNEL)
        return TROP_CREUSE;

    return OK;
}

void ActionCreuserTunnel::apply_on(GameState* st) const
{
    st->get_player(player_id_).increment_tunnels();
    st->get_map().get_cell(pos_).etat.contenu = VIDE;
}
