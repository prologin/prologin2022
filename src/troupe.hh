#pragma once

#include "constant.hh"
#include "player_info.hh"
#include "map.hh"
#include "position.hh"


inline void respawn(troupe& trp, PlayerInfo& player_info, Map& map)
{
	// Dropping the bread
	for (auto i = 0; i < trp.inventaire; ++i)
		map.get_cell(trp.canards[i]).etat.nb_pains = true;
	trp.inventaire = 0;

	// Determining the spawn_point
	trp.maman = map.get_spawn_toward(trp.dir);
	trp.canards = { trp.maman };
	std::queue<position> *q = player_info.canards_additionnels(trp.id);
	for (auto i = 1; i < 5; ++i)
		player_info.enfiler_canard(trp.id);
}

inline void move_troupe(troupe& trp, const direction& dir, Map& map,
                        PlayerInfo& player)
{
    auto delta = get_delta_pos(dir);
    if (map.case_mortelle(trp.maman + delta))
    {
        map.delete_troupe(trp);
        respawn(trp, player, map);
    }
    else
    {
        map.get_cell(trp.canards[trp.taille - 1]).canard_sur_case = false;
        for (int i = trp.taille - 1; i > 0; --i)
            trp.canards[i] = trp.canards[i - 1];
        trp.maman += delta;
        player.remove_pts_actions(trp.id, 1);
        map.get_cell(trp.maman).canard_sur_case = true;
        player.spawn_canard(trp.id, map);
    }
}
