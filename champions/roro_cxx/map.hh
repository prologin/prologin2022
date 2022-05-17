#pragma once

#include "api.hh"
#include <functional>

namespace map
{
void apply_on_map(std::function<void(position)> fx);
}
