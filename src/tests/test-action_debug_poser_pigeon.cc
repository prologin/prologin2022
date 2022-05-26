#include "../constant.hh"
#include "../position.hh"
#include "test-helpers.hh"

TEST_F(ApiTest, ActionDebugPoserPigeon_PositionInvalide)
{
    const position positions[] = {
        {0, HAUTEUR, 0}, {LARGEUR, 0, 0}, {0, 0, 1},
        {-1, 0, 0},      {0, -1, 0},      {0, 0, -2},
    };

    for (const auto& player : players)
    {
        for (const auto& position : positions)
        {
            auto err = player.api->debug_poser_pigeon(position, PIGEON_BLEU);
            ASSERT_EQ(POSITION_INVALIDE, err);
        }
    }
}

TEST_F(ApiTest, ActionDebugPoserPigeon_Ok)
{
    const position positions[] = {
        {0, 0, 0}, {24, 24, 0}, {LARGEUR - 1, HAUTEUR - 1, 0}};

    const pigeon_debug pigeons[] = {PAS_DE_PIGEON, PIGEON_BLEU, PIGEON_JAUNE,
                                    PIGEON_ROUGE};

    for (const auto& player : players)
    {
        const auto& gs = player.api->game_state();

        for (const auto& position : positions)
        {
            for (const auto& pigeon : pigeons)
            {
                auto err = player.api->debug_poser_pigeon(position, pigeon);
                auto act =
                    gs.get_player(player.id).get_internal_history().back();
                ASSERT_EQ(OK, err);
                ASSERT_EQ(flag, act.type);
                ASSERT_EQ(position, act.flag.pos);
                ASSERT_EQ(pigeon, act.flag.ctype);
            }
        }
    }
}
