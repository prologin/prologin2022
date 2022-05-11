// SPDX-License-Identifier: GPL-2.0-or-later
// Copyright (c) 2012-2020 Association Prologin <association@prologin.org>

#include "actions.hh"

int ActionGrandir::check(const GameState& st) const
{
    auto player = st.get_player(player_id_);
    auto trp = player.get_troupe(id_);

    if (trp == nullptr)
        return TROUPE_INVALIDE;

    if (player.pts_actions(trp->id) < COUT_CROISSANCE)
        return TROP_GRANDI;

    return OK;
}

void ActionGrandir::apply_on(GameState* st) const
{
    auto& player = st->get_player(player_id_);
    player.remove_pts_actions(id_, COUT_CROISSANCE);
    player.enfiler_canard(id_);
}
