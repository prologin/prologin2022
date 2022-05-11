#include "test-helpers.hh"

TEST_F(ApiTest, ActionGrandir_TropGrandi)
{
    // Whatever the cost, this should be to much
    const int nb_loops = 100;
    for (int i = 0; i < nb_loops; i++)
    {
        for (const auto& player : players)
        {
            const auto& info = player.api->game_state().get_player(player.id);
            for (const auto& trp : info.troupes())
            {
                auto initial_size = trp.canards.size();
                auto inital_additional_size =
                    info.canards_additionnels(trp.id)->size();
                player.api->grandir(trp.id);
            }
        }
    }

    for (const auto& player : players)
    {
        const auto& info = player.api->game_state().get_player(player.id);
        for (const auto& trp : info.troupes())
        {
            auto initial_size = trp.canards.size();
            auto inital_additional_size =
                info.canards_additionnels(trp.id)->size();
            auto initial_pts = info.pts_actions(trp.id);
            auto err = player.api->grandir(trp.id);

            ASSERT_EQ(TROP_GRANDI, err);
            ASSERT_EQ(initial_size, trp.canards.size());
            ASSERT_EQ(inital_additional_size,
                      info.canards_additionnels(trp.id)->size());
            ASSERT_EQ(initial_pts, info.pts_actions(trp.id));
        }
    }
}

TEST_F(ApiTest, ActionGrandir_OK)
{
    for (const auto& player : players)
    {
        const auto& info = player.api->game_state().get_player(player.id);
        for (const auto& trp : info.troupes())
        {
            auto initial_size = trp.canards.size();
            auto inital_additional_size =
                info.canards_additionnels(trp.id)->size();
            auto initial_pts = info.pts_actions(trp.id);
            auto err = player.api->grandir(trp.id);

            ASSERT_EQ(OK, err);
            ASSERT_EQ(initial_size, trp.canards.size());
            ASSERT_EQ(inital_additional_size + 1,
                      info.canards_additionnels(trp.id)->size());
            ASSERT_EQ(initial_pts - COUT_CROISSANCE, info.pts_actions(trp.id));
        }
    }
}
