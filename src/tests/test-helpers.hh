#include <array>
#include <fstream>
#include <gtest/gtest.h>
#include <rules/player.hh>

#include "../api.hh"
#include "../game_state.hh"
#include "../player_info.hh"

namespace
{
static const auto test_map_path = "src/tests/map.txt";

static rules::Players make_players(int id_p1, int id_p2)
{
    // Create two players (no spectator)
    rules::Players players;
    players.add(std::make_shared<rules::Player>(id_p1, rules::PLAYER));
    players.add(std::make_shared<rules::Player>(id_p2, rules::PLAYER));
    return players;
}

static GameState* make_test_gamestate(std::string map_path,
                                      const rules::Players& players)
{
    std::ifstream map_stream(map_path);
    if (!map_stream)
        FATAL("Could not open the test map");
    return new GameState(map_stream, players);
}
} // namespace

class ApiTest : public ::testing::Test
{
public:
    struct Player
    {
        int id;
        PlayerInfo* info;
        std::unique_ptr<Api> api;
    };

protected:
    virtual void SetUp()
    {
        // Players values are not 0 and 1, because that would be too simple
        int player_id_1 = 1337;
        int player_id_2 = 42;

        utils::Logger::get().level() = utils::Logger::DEBUG_LEVEL;
        auto gs_players = make_players(player_id_1, player_id_2);
        std::unique_ptr<GameState> st(
            make_test_gamestate(test_map_path, gs_players));

        auto state_copy = st->copy();

        players[0].id = player_id_1;
        players[0].api = std::make_unique<Api>(
            std::unique_ptr<GameState>(state_copy), gs_players[0]);
        players[0].info = state_copy->get_player_ptr(players[0].id);

        state_copy = st->copy();
        players[1].id = player_id_2;
        players[1].api = std::make_unique<Api>(
            std::unique_ptr<GameState>(state_copy), gs_players[1]);
        players[1].info = state_copy->get_player_ptr(players[1].id);
    }

    std::array<Player, 2> players;
};
