// SPDX-License-Identifier: GPL-2.0-or-later
// Copyright (c) 2012-2020 Association Prologin <association@prologin.org>

#include "actions.hh"
#include "position.hh"

int ActionConstruireBuisson::check(const GameState& st) const
{
    if (!st.is_init())
        return HORS_TOUR;

    if (!inside_map(pos_))
        return POSITION_INVALIDE;

    if (!st.get_map().get_cell(pos_).etat.est_constructible)
        return NON_CONSTRUCTIBLE;

    // make sure that there is not already something built
    if (st.get_map().get_cell(pos_).etat.contenu != GAZON)
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

    if (players[0].get_score() < COUT_BUISSON)
        return SCORE_INSUFFISANT;

    return OK;
}

void ActionConstruireBuisson::apply_on(GameState* st) const
{
    auto player = st->get_player_ptr(player_id_);

    // Change the state of the player
    player->decrease_score(COUT_BUISSON);

    // Change the state of the cell
    st->get_map().get_cell(pos_).etat.contenu = BUISSON;
    st->get_map().get_cell(pos_).etat.est_constructible = false;

    // Log action.
    internal_action action;
    action.type = standard_action;
    action.action.action_type = ACTION_CONSTRUIRE;
    action.action.troupe_id = -1;
    // TODO init action.action.action_dir;
    action.action.action_pos = pos_;
    player->add_internal_action(action);
}
