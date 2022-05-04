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
    auto delta = get_delta_pos(dir);
    for (int i = trp.taille - 1 ; i > 0; --i)
    {
        trp.canards[i] = trp.canards[i - 1];
    }
    trp.maman += delta;
}

inline void respawn(troupe& trp)
{
    // TODO
}
