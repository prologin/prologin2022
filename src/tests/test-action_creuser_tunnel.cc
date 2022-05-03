#include "test-helpers.hh"

namespace
{
const int TUNNELS_INIT = FREQ_TUNNEL - 1;

void init_tunnels(const std::array<ApiTest::Player, 2>& players)
{
    for (auto& player : players)
    {
        auto& player_info = player.api->game_state().get_player(player.id);
        for (int i = 0; i < TUNNELS_INIT; ++i)
            player_info.increment_tunnels();
    }
}
} // namespace

TEST_F(ApiTest, ActionCreuserTunnel_TropCreuse)
{
    init_tunnels(players);

    for (auto& player : players)
    {
        auto& player_info = player.api->game_state().get_player(player.id);
        player_info.increment_tunnels();
    }

    const position pos{5, 2, -1};
    for (const auto& player : players)
    {
        auto err = player.api->creuser_tunnel(pos);
        ASSERT_EQ(TROP_CREUSE, err);
    }
}

TEST_F(ApiTest, ActionCreuserTunnel_PositionInvalide)
{
    init_tunnels(players);

    const position positions[] = {
        {0, HAUTEUR, 0}, {LARGEUR, 0, 0}, {0, 0, 1},
        {-1, 0, 0},      {0, -1, 0},      {0, 0, -2},
    };
    for (const auto& player : players)
    {
        for (const auto& position : positions)
        {
            auto err = player.api->creuser_tunnel(position);
            ASSERT_EQ(POSITION_INVALIDE, err);
        }
        ASSERT_EQ(TUNNELS_INIT,
                  player.api->game_state().get_player(player.id).get_tunnels());
    }
}

TEST_F(ApiTest, ActionCreuserTunnel_NonCreusable)
{
    init_tunnels(players);

    position positions[] = {
        {0, 0, 0},
        {1, 1, 0},
        {5, 2, 0},
    };

    for (auto& player : players)
    {
        for (const auto& position : positions)
        {
            auto err = player.api->creuser_tunnel(position);
            ASSERT_EQ(NON_CREUSABLE, err);
        }
        ASSERT_EQ(TUNNELS_INIT,
                  player.api->game_state().get_player(player.id).get_tunnels());
    }
}

TEST_F(ApiTest, ActionCreuserTunnel_Ok)
{
    init_tunnels(players);

    position positions[] = {
        {5, 2, -1},
        //{53, 52, -1},
    };

    for (auto& player : players)
    {
        INFO("player %d", player.id);
        const auto& gs = player.api->game_state();
        auto tunnels_init = TUNNELS_INIT;
        for (const auto& pos : positions)
        {
            auto err = player.api->creuser_tunnel(pos);
            tunnels_init += 1;

            ASSERT_EQ(OK, err);
            ASSERT_EQ(VIDE, gs.get_map().get_cell(pos).etat.contenu);
            ASSERT_EQ(tunnels_init, gs.get_player(player.id).get_tunnels());
        }
    }
}
