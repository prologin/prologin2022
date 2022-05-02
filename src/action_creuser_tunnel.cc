// SPDX-License-Identifier: GPL-2.0-or-later
// Copyright (c) 2012-2020 Association Prologin <association@prologin.org>

#include "actions.hh"

int ActionCreuserTunnel::check(const GameState& st) const
{
    // TODO :
    // `TROP_CREUSE`
    // Erreur si déjà creusé `FREQ_TUNNEL` tunnels ce tour-ci

    if (!inside_map(pos_))
        return POSITION_INVALIDE;

    if (!st.get_map().get_cell(pos_).etat.contenu != TERRE)
        return NON_CREUSABLE;

    PlayerInfo players[] = {
        st.get_player(player_id_),
        st.get_other(player_id_),
    };

    return OK;
}

void ActionCreuserTunnel::apply_on(GameState* st) const
{
    // FIXME
    // TERRE -> VIDE si tout est bon
    // TODO :
    // Ajouter 1 au compte de creusage du joueur

    st->get_map().get_cell(pos_).etat.contenu = VIDE;

}
