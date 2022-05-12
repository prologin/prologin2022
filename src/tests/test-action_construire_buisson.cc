#include "test-helpers.hh"

namespace
{
const int PAINS_INIT = 2 * COUT_BUISSON;

void init_pains(const std::array<ApiTest::Player, 2>& players)
{
    for (auto& player : players)
    {
        auto& player_info = player.api->game_state().get_player(player.id);
        for (int i = 0; i < PAINS_INIT; ++i)
            player_info.increment_pains();
    }
}
} // namespace

TEST_F(ApiTest, ActionConstruireBuisson_PainsInsuffisants)
{
    const position pos{5, 2, 0};
    for (const auto& player : players)
    {
        auto err = player.api->construire_buisson(pos);
        ASSERT_EQ(SCORE_INSUFFISANT, err);
    }
}

TEST_F(ApiTest, ActionConstruireBuisson_PositionInvalide)
{
    init_pains(players);

    const position positions[] = {
        {0, HAUTEUR, 0}, {LARGEUR, 0, 0}, {0, 0, 1},
        {-1, 0, 0},      {0, -1, 0},      {0, 0, -2},
    };
    for (const auto& player : players)
    {
        for (const auto& position : positions)
        {
            auto err = player.api->construire_buisson(position);
            ASSERT_EQ(POSITION_INVALIDE, err);
        }
        ASSERT_EQ(PAINS_INIT,
                  player.api->game_state().get_player(player.id).get_pains());
    }
}

TEST_F(ApiTest, ActionConstruireBuisson_NonConstructible)
{
    init_pains(players);

    position positions[] = {
        {0, 0, 0},
        {1, 1, 0},
        {5, 2, -1},
        {5, 2, 0},
    };
    for (auto& player : players)
        player.api->game_state()
            .get_player(players[0].id)
            .troupes()[0]
            .canards[0] = position{5, 2, 0};

    for (auto& player : players)
    {
        for (const auto& position : positions)
        {
            auto err = player.api->construire_buisson(position);
            ASSERT_EQ(NON_CONSTRUCTIBLE, err);
        }
        ASSERT_EQ(PAINS_INIT,
                  player.api->game_state().get_player(player.id).get_pains());
    }
}

TEST_F(ApiTest, ActionConstruireBuisson_Ok)
{
    init_pains(players);

    position positions[] = {
        {5, 2, 0},
        {53, 52, 0},
    };

    for (auto& player : players)
    {
        const auto& gs = player.api->game_state();
        auto pains_init = PAINS_INIT;
        for (const auto& pos : positions)
        {
            auto err = player.api->construire_buisson(pos);
            pains_init -= COUT_BUISSON;

            ASSERT_EQ(OK, err);
            ASSERT_EQ(BUISSON, gs.get_map().get_cell(pos).etat.contenu);
            ASSERT_FALSE(gs.get_map().get_cell(pos).etat.est_constructible);
            ASSERT_EQ(pains_init, gs.get_player(player.id).get_pains());
        }
    }
}
