#include "troupe.hh"

#include "api.hh"

#define INV_MAX(A) ((A)-1)

void respawn(troupe& trp, PlayerInfo& player_info, Map& map)
{
    // Dropping the bread
    for (auto i = 0; i < trp.inventaire; ++i)
    {
        map.get_cell(trp.canards[trp.taille - 1 - i]).etat.nb_pains += 1;

        // Log event
        internal_action action;
        action.type = spread_bread;
        action.action.action_pos = trp.canards[trp.taille - 1 - i];
        player_info.add_internal_action(action);
    }
    trp.inventaire = 0;

    // Determining the spawn_point
    direction dir = trp.dir;
    do
    {
        trp.maman = map.get_spawn_toward(dir);
        dir = clockwise_dir(dir);
    } while (map.get_cell(trp.maman).canard_sur_case);

    // Mark the new position
    map.get_cell(trp.maman).canard_sur_case = true;

    // Log event
    internal_action action;
    action.type = troupe_respawn;
    action.action.troupe_id = trp.id;
    action.action.action_pos = trp.maman;
    player_info.add_internal_action(action);

    trp.taille = 1;
    trp.canards = {trp.maman};
    trp.pts_actions = 0;

    *player_info.canards_additionnels(trp.id) = std::queue<position>();
    for (auto i = 1; i < TAILLE_DEPART; ++i)
        player_info.enfiler_canard(trp.id);
}

void prendre_pain(troupe& trp, Map& map, PlayerInfo& player)
{
    auto nb_pains = map.get_cell(trp.maman).etat.nb_pains;
    if (trp.inventaire < INV_MAX(trp.taille) && nb_pains > 0)
    {
        auto amount = nb_pains;
        if (INV_MAX(trp.taille) - trp.inventaire < amount)
            amount = INV_MAX(trp.taille) - trp.inventaire;

        if (amount)
        {
            // Log event
            internal_action action;
            action.type = take_bread;
            action.action.troupe_id = trp.id;
            player.add_internal_action(action);
        }

        trp.inventaire += amount;
        map.get_cell(trp.maman).etat.nb_pains -= amount;
    }
}

void capturer_nid(troupe& trp, Map& map, PlayerInfo& player)
{
    if (map.get_cell(trp.maman).nid == LIBRE)
        map.get_cell(trp.maman).nid = player.get_player_nid_id();
}

void deposer_nid(troupe& trp, Map& map, PlayerInfo& player)
{
    if (map.get_cell(trp.maman).nid == player.get_player_nid_id())
    {
        if (trp.inventaire)
        {
            // Log event
            internal_action action;
            action.type = leave_bread;
            action.action.troupe_id = trp.id;
            player.add_internal_action(action);
        }
        player.increase_score(gain(trp.inventaire));
        trp.inventaire = 0;
    }
}

void move_troupe(troupe& trp, const direction& dir, Map& map,
                 PlayerInfo& player)
{
    // Action points shall always be removed - even if the troup dies
    player.remove_pts_actions(trp.id, 1);

    auto delta = get_delta_pos(dir);
    trp.dir = dir;
    if (!inside_map(trp.maman + delta) || map.case_mortelle(trp.maman + delta))
    {
        map.delete_troupe(trp);
        respawn(trp, player, map);
    }
    else
    {
        map.get_cell(trp.canards.back()).canard_sur_case = false;
        for (int i = trp.taille - 1; i > 0; --i)
            trp.canards[i] = trp.canards[i - 1];
        trp.canards[0] += delta;
        trp.maman = trp.canards[0];

        map.get_cell(trp.maman).canard_sur_case = true;
        player.spawn_canard(trp.id, map);

        prendre_pain(trp, map, player);
        capturer_nid(trp, map, player);
        deposer_nid(trp, map, player);
    }
}
