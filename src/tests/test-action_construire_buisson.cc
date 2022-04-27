#include "test-helpers.hh"

namespace
{
const int PAINS_INIT = 2 * COUT_BUISSON;

void init_pains(const std::array<ApiTest::Player, 2>& players, GameState& st)
{
    for (auto& player : players)
    {
        int player_id = player.id;
        auto& player_info = st.get_player(player_id);
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
        ASSERT_EQ(PAINS_INSUFFISANTS, err);
    }
}

TEST_F(ApiTest, ActionConstruireBuisson_PositionInvalide)
{
    init_pains(players, *st);

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
    }
}

TEST_F(ApiTest, ActionConstruireBuisson_NonConstructible)
{
    init_pains(players, *st);

    const position positions[] = {
        {0, 0, 0},
        {1, 1, 0},
        {5, 2, -1},
    };
    for (auto& player : players)
        for (auto& player : players)
        {
            for (const auto& position : positions)
            {
                auto err = player.api->construire_buisson(position);
                ASSERT_EQ(NON_CONSTRUCTIBLE, err);
            }
        }
}
