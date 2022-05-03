#pragma once

#include "constant.hh"
#include "map.hh"

inline bool is_dead(troupe& trp, Map& map)
{
    for (int i = 1; i < trp.taille; ++i)
    {
        if (trp.maman == trp.canards[i])
            return true;
    }
 
    return !map.case_praticable(trp.maman);
}

inline void move_troupe(troupe& trp, const direction& dir)
{
    // Better than doing it in player_info 
    auto delta = get_delta_pos(dir);
    for (auto& canard : trp.canards)
        canard += delta;
}

inline void respawn(troupe& trp)
{
    // TODO
}
