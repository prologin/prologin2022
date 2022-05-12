#include <utils/log.hh>

#include "../constant.hh"
#include "../position.hh"
#include "test-helpers.hh"

namespace
{
void assert_troupe_moved(troupe& trp_after, troupe& trp_before, direction dir,
                         int line)
{
    auto delta = get_delta_pos(dir);
    ASSERT_EQ(trp_before.maman + delta, trp_after.maman)
        << "First Duck did not move accordingly line: " << line;

    for (int i = trp_after.taille - 1; i > 0; --i)
    {
        ASSERT_EQ(trp_before.canards[i - 1], trp_after.canards[i])
            << "Duck number " << i
            << ", did not move accordingly, line: " << line;
    }
}

erreur test_move_troupe(PlayerInfo* player, std::unique_ptr<Api>& api, int id,
                        direction dir, bool moved, int line)
{
    auto before = *(player->get_troupe(id));

    auto err = api->avancer(id, dir);

    troupe after = *(player->get_troupe(id));

    // ASSERT_EQ(err == OK, moved) << "failed line: " << line;
    if (moved)
        assert_troupe_moved(after, before, dir, line);
    return err;
}

void mark_troupe_on_map(troupe* trp, Map& map)
{
    for (auto& pos : trp->canards)
        map.get_cell(pos).canard_sur_case = true;
}

void troup_hardcoded_setup(troupe* trp, Map& map)
{
    map.delete_troupe(*trp);
    trp->canards[0] = {.colonne = 50, .ligne = 78, .niveau = 0};
    trp->canards[1] = {.colonne = 49, .ligne = 78, .niveau = 0};
    trp->canards[2] = {.colonne = 48, .ligne = 78, .niveau = 0};
    trp->canards[3] = {.colonne = 47, .ligne = 78, .niveau = 0};
    trp->canards[4] = {.colonne = 46, .ligne = 78, .niveau = 0};
    trp->maman = trp->canards[0];
    mark_troupe_on_map(trp, map);
}
} // namespace

TEST_F(ApiTest, ActionAvancerAutorisee)
{
    auto& player = players[0];
    troup_hardcoded_setup(player.info->get_troupe(1),
                          player.api->game_state().get_map());

    auto err =
        test_move_troupe(player.info, player.api, 1, EST, true, __LINE__);
    ASSERT_EQ(OK, err);
}

TEST_F(ApiTest, ActionAvancerInvalidTroupe)
{
    auto& player = players[0];
    direction dir = EST;
    auto err = player.api->avancer(NB_TROUPES + 42, dir);

    ASSERT_EQ(TROUPE_INVALIDE, err);
}

TEST_F(ApiTest, ActionAvancerTueLaTroupe)
{
    auto& player = players[0];

    std::vector<position> former = player.info->get_troupe(1)->canards;
    auto err = player.api->avancer(1, EST);
    ASSERT_EQ(OK, err);

    for (auto& pos : former)
        ASSERT_EQ(
            false,
            player.api->game_state().get_map().get_cell(pos).canard_sur_case);
}
