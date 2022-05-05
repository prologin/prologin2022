// SPDX-License-Identifier: GPL-2.0-or-later
// Copyright (c) 2012-2020 Association Prologin <association@prologin.org>

#include "actions.hh"
#include "map.hh"
#include "position.hh"
#include "troupe.hh"

int avancer_troupe(troupe *trp, const direction& dir, const Map& map)
{
    position new_pos = trp->maman + get_delta_pos(dir);
    if (!inside_map(new_pos))
        return POSITION_INVALIDE;

    return OK;
}

int ActionAvancer::check(const GameState& st) const
{
    auto player = st.get_player(player_id_);
    auto trp = player.get_troupe(id_);

    /*
     * Mais en fait, on a pas d'autres erreurs que les mauvais ID
     * et les mauvaises directions ? En effet, si la troupe va
     * vers une position invalide elle meurt :think: ?
     * */
    if (trp == nullptr)
        return TROUPE_INVALIDE;

    if (player.mouvements(trp->id) <= 0)
        return MOUVEMENTS_INSUFFISANTS;
    
   auto state = avancer_troupe(trp, dir_, st.get_map());

    if (state != OK)
        return state;
    return OK;
}

void ActionAvancer::apply_on(GameState* st) const
{
    auto player = st->get_player_ptr(player_id_);
    auto trp = player->get_troupe(id_);
    
    move_troupe(*trp, dir_, st->get_map()); //Gere la mort de la troupe
}
