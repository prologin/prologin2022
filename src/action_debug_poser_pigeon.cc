// SPDX-License-Identifier: GPL-2.0-or-later
// Copyright (c) 2012-2020 Association Prologin <association@prologin.org>

#include "actions.hh"
#include "position.hh"

int ActionDebugPoserPigeon::check(const GameState& st) const
{
    if (!st.is_init())
        return HORS_TOUR;
    if (!inside_map(pos_))
        return POSITION_INVALIDE;
    if (pigeon_ < 0 || (int)pigeon_ >= 4)
        return PIGEON_INVALIDE;
    return OK;
}

void ActionDebugPoserPigeon::apply_on(GameState* st) const
{
    auto player = st->get_player_ptr(player_id_);

    internal_action action;
    action.type = flag;
    action.flag.pos = pos_;
    action.flag.ctype = pigeon_;
    player->add_internal_action(action);
}
