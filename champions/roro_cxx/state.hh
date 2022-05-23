#pragma once

#include <functional>
#include <optional>

#include "api.hh"

namespace state
{
struct grandpa
{
    position pos;
    int turns_left;
};

struct nest
{
    position pos;
    etat_nid state;
};

inline std::vector<grandpa> grandpas;
inline std::vector<nest> nests;

inline int me = -1;
inline int other = -1;

inline std::array<std::optional<std::function<bool(position)>>, NB_TROUPES>
    current_goals = {
        std::nullopt,
        std::nullopt,
};

void update_state();
void update_goals(bool force = false);
} // namespace state
