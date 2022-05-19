#pragma once

#include "constant.hh"
#include "map.hh"
#include "player_info.hh"
#include "position.hh"

void move_troupe(troupe& trp, const direction& dir, Map& map,
                 PlayerInfo& player);
