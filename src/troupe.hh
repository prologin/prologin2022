#pragma once

#include "constant.hh"
#include "map.hh"

inline void respawn(troupe& trp)
{
    // TODO
}

inline void move_troupe(troupe& trp, const direction& dir, Map& map)
{
    auto delta = get_delta_pos(dir);
    if (map.case_mortelle(trp.maman + delta))
    {
        map.delete_troupe(trp);
        respawn(trp);
    }
    else
    {
         map.get_cell(trp.canards[trp.taille - 1]).canard_sur_case = false;
         for (int i = trp.taille - 1 ; i > 0; --i)
             trp.canards[i] = trp.canards[i - 1];
         trp.maman += delta;
         map.get_cell(trp.maman).canard_sur_case = true;
    }
}

