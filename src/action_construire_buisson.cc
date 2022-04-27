// SPDX-License-Identifier: GPL-2.0-or-later
// Copyright (c) 2012-2020 Association Prologin <association@prologin.org>

#include "actions.hh"
#include "position.hh"

int ActionConstruireBuisson::check(const GameState& st) const
{
    if (!inside_map(pos_))
        return POSITION_INVALIDE;

    if (!st.get_map().get_cell(pos_).etat.est_constructible)
        return NON_CONSTRUCTIBLE;

    auto& player = st.get_player(player_id_);

    if (player.get_pains() < COUT_BUISSON)
        return PAINS_INSUFFISANTS;

    return OK;
}

void ActionConstruireBuisson::apply_on(GameState* st) const
{
    // FIXME
}
