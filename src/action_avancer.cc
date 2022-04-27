// SPDX-License-Identifier: GPL-2.0-or-later
// Copyright (c) 2012-2020 Association Prologin <association@prologin.org>

#include "actions.hh"
#include "map.hh"
#include "position.hh"

int avancer_troupe(const position& troupe, const direction& dir, const Map& map)
{
    position new_pos = troupe + get_delta_pos(dir);
    if (!inside_map(new_pos) || !map.case_praticable(new_pos))
        return POSITION_INVALIDE;
       
    return OK; 
}

int ActionAvancer::check(const GameState& st) const
{
    auto player = st.get_player(player_id_); 
    for (auto& troupe : player.troupes())
    {
        auto state = avancer_troupe(troupe, dir_, st.get_map());
        if (state != OK)
            return state;
    }
    return OK;
}

void ActionAvancer::apply_on(GameState* st) const
{
    // FIXME
}
