// SPDX-License-Identifier: GPL-2.0-or-later
// Copyright (c) 2012-2020 Association Prologin <association@prologin.org>

#pragma once

#include <rules/action.hh>

#include "actions.hh"
#include "constant.hh"
#include "game_state.hh"

class ActionAvancer : public rules::Action<GameState>
{
public:
    ActionAvancer(int id, direction dir, int player_id)
        : id_(id)
        , dir_(dir)
        , player_id_(player_id)
    {
    }
    ActionAvancer() {} // for register_action()

    int check(const GameState& st) const override;
    void apply_on(GameState* st) const override;

    void handle_buffer(utils::Buffer& buf) override
    {
        buf.handle(id_);
        buf.handle(dir_);
        buf.handle(player_id_);
    }

    uint32_t player_id() const override { return player_id_; };
    uint32_t id() const override { return ID_ACTION_AVANCER; }

private:
    int id_;
    direction dir_;
    int player_id_;
};

