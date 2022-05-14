#include "troupe.hh"

#define INV_MAX(A) A - 1

void respawn(troupe& trp, PlayerInfo& player_info, Map& map)
{
	// Dropping the bread
	for (auto i = 0; i < trp.inventaire; ++i)
		map.get_cell(trp.canards[trp.taille - 1 - i]).etat.nb_pains += 1;
	trp.inventaire = 0;

	// Determining the spawn_point
	trp.maman = map.get_spawn_toward(trp.dir);
	trp.canards = { trp.maman };
	std::queue<position> *q = player_info.canards_additionnels(trp.id);
	for (auto i = 1; i < TAILLE_DEPART; ++i)
		player_info.enfiler_canard(trp.id);
}

void prendre_pain(troupe& trp, Map& map)
{
	auto nb_pains = map.get_cell(trp.maman).etat.nb_pains; 
	if (trp.inventaire < INV_MAX(trp.taille) && nb_pains > 0)
	{
		auto amount = nb_pains;
		if (INV_MAX(trp.taille) - trp.inventaire < amount)
			amount = INV_MAX(trp.taille) - trp.inventaire;
		trp.inventaire += amount;
		map.get_cell(trp.maman).etat.nb_pains -= amount;
	}	
}

void capturer_nid(troupe& trp, Map& map)
{
	if (map.get_cell(trp.maman).nid != LIBRE)
		;	
}	

void deposer_nid(troupe& trp, Map& map)
{
	//TODO
}

void move_troupe(troupe& trp, const direction& dir, Map& map,
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
		
		prendre_pain(trp, map);
		capturer_nid(trp, map);
		deposer_nid(trp, map);
    }
}
