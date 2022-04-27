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

    // make sure that there is not already something built
    if (st.get_map().get_cell(pos_).etat.contenu != VIDE)
        return NON_CONSTRUCTIBLE;

    PlayerInfo players[] = {
        st.get_player(player_id_),
        st.get_other(player_id_),
    };

    // make sure that the position does not overlap with a duck
    for (const auto& player : players)
        for (auto& troupe : player.troupes())
            for (auto& canard : troupe.canards)
                if (canard == pos_)
                    return NON_CONSTRUCTIBLE;

    if (players[0].get_pains() < COUT_BUISSON)
        return PAINS_INSUFFISANTS;

    return OK;
}

void ActionConstruireBuisson::apply_on(GameState* st) const
{
    // FIXME
}
