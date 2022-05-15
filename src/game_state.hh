// SPDX-License-Identifier: GPL-2.0-or-later
// Copyright (c) 2015 Association Prologin <association@prologin.org>

#pragma once

#include <rules/game-state.hh>
#include <rules/player.hh>

#include "constant.hh"
#include "map.hh"
#include "player_info.hh"

class GameState final : public rules::GameState
{
public:
    GameState(std::istream& map_stream, const rules::Players& players);
    GameState(const GameState& st);
    ~GameState();

    GameState* copy() const override;

    // Player infos
    const PlayerInfo& get_player(int id) const;
    PlayerInfo& get_player(int id);
    const PlayerInfo& get_other(int id) const;
    PlayerInfo& get_other(int id);

    PlayerInfo* get_player_ptr(int id);

    // Map
    const Map& get_map() const;
    Map& get_map();

    // Rounds
    int get_round() const;
    void next_round();
    bool is_finished() const;

    bool is_init() const;
    void set_init(bool init);

private:
    void decrementer_papy();

    Map map_;
    std::array<PlayerInfo, 2> players_; // Troupes, scores, etc.
    int round_;                         // Note : 1 round = 2 tours
    bool init_;
};
