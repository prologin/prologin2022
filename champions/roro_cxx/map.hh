#pragma once

#include "api.hh"
#include <functional>
#include <optional>

namespace map
{
bool inside_map(position pos);
bool is_obstacle(position pos);
void apply_on_map(std::function<void(position)> fx);
position operator+(position pos, direction dir);

std::optional<std::vector<direction>>
find_path(position start, std::function<bool(position)> predicate);
} // namespace map
