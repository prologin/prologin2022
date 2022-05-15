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

    return OK;
}

void ActionDebugPoserPigeon::apply_on(GameState* st) const
{
    st->get_map().get_cell(pos_).pigeon = pigeon_;
}
