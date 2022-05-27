#include "troupe.hh"
#include "api.hh"
#include "constant.hh"
#include "player_info.hh"

int get_inv_max(int taille)
{
    if (taille < 0)
        return 0;
    return taille / 3;
}

int get_head(int index)
{
    return (index / 3) * 3;
}

int get_carrier(int index)
{
    return get_head(index) + 2;
}

void drop_bread_from(const int start, const int end, troupe& trp, Map& map,
				PlayerInfo& player_info)
{
    for (int i = get_carrier(start); i < end; i += 3)
    {
        if (trp.inventaire <= 0)
            continue;
        map.get_cell(trp.canards[i]).etat.nb_pains += 1;
        trp.inventaire--;

        // Log event
        internal_action action;
        action.type = spread_bread;
        action.action.action_pos = trp.canards[i];
        player_info.add_internal_action(action);
    }
}

void respawn(troupe& trp, PlayerInfo& player_info, Map& map)
{
	// We suppose that the bread has already been dropped

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
    action.action.action_dir = inverse_dir(dir);
    player_info.add_internal_action(action);

    trp.taille = 1;
    trp.canards = {trp.maman};
    trp.pts_actions = 0;

    *player_info.canards_additionnels(trp.id) = std::queue<position>();
    for (auto i = 1; i < TAILLE_DEPART; ++i)
        player_info.enfiler_canard(trp.id);
}

void die(troupe& trp, PlayerInfo& player, Map& map)
{
    map.delete_troupe(trp);
	drop_bread_from(0, trp.taille, trp, map, player);
    respawn(trp, player, map);
}

void prendre_pain(troupe& trp, Map& map, PlayerInfo& player)
{
    auto nb_pains = map.get_cell(trp.maman).etat.nb_pains;
	INFO("%d, %d, %d", trp.taille, trp.inventaire, get_inv_max(trp.taille));
    if (trp.inventaire < get_inv_max(trp.taille) && nb_pains > 0)
    {
        auto amount = nb_pains;
        if (get_inv_max(trp.taille) - trp.inventaire < amount)
            amount = get_inv_max(trp.taille) - trp.inventaire;

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
    {
        map.get_cell(trp.maman).nid = player.get_player_nid_id();

        // Log event
        internal_action action;
        action.type = capture_nest;
        action.action.action_pos = trp.maman;
        player.add_internal_action(action);
    }
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
                 PlayerInfo& player, internal_action action)
{
    // Action points shall always be removed - even if the troup dies
    player.remove_pts_actions(trp.id, 1);

    auto delta = get_delta_pos(dir);
    trp.dir = dir;
    if (!inside_map(trp.maman + delta) || map.case_mortelle(trp.maman + delta))
        die(trp, player, map);
    else
    {
        map.unmark_canard(trp.canards.back());
        // add the internal action only if the player can actually move
        player.add_internal_action(action);

        for (int i = trp.taille - 1; i > 0; --i)
            trp.canards[i] = trp.canards[i - 1];
        trp.canards[0] += delta;
        trp.maman = trp.canards[0];

        map.mark_canard(trp.canards.back(), player, trp.id);
        player.spawn_canard(trp.id, map);

        prendre_pain(trp, map, player);
        capturer_nid(trp, map, player);
        deposer_nid(trp, map, player);
    }
}

int troupe_get_splitting_index(troupe& trp, const position& canard)
{
    for (int i = 1; i < trp.taille; ++i)
        if (trp.canards[i] == canard)
            return i;
    return -1;
}


void troupe_split_at(const int troupe_id, PlayerInfo& player_info,
                     const position& canard, Map& map)
{
    troupe& trp = *(player_info.get_troupe(troupe_id));
    // Find the bad canard
    if (canard == trp.maman)
    {
        drop_bread_from(0, trp.taille, trp, map, player_info);
        die(trp, player_info, map);
        return;
    }

    int splitting_index = troupe_get_splitting_index(trp, canard);
    if (splitting_index == -1)
        return; // Split not anymore in troupe

    drop_bread_from(splitting_index, trp.taille, trp, map, player_info);

    for (int i = trp.taille - 1; i >= splitting_index; i--)
    {
        map.unmark_canard(trp.canards[i]);
        trp.canards.pop_back();
        trp.taille -= 1;
    }

    if (trp.taille < TAILLE_MIN)
        die(trp, player_info, map);
}
