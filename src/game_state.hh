// SPDX-License-Identifier: GPL-2.0-or-later
// Copyright (c) 2015 Association Prologin <association@prologin.org>

#pragma once

#include <rules/game-state.hh>
#include <rules/player.hh>

#include <constant.hh>
#include <player_info.hh>
#include <map.hh>

class GameState final : public rules::GameState
{
public:
    // FIXME
    // additional parameters? for instance map
    GameState(const rules::Players& players);
    GameState(const GameState& st);
    ~GameState();

    GameState* copy() const override;
private:
    Map map_;
    std::array<PlayerInfo, 2> players_; // Troupes, scores, etc.
    int round_; // Note : 1 round = 2 tours
};
