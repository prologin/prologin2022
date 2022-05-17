#pragma once

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

std::vector<grandpa> grandpas;
std::vector<nest> nests;

void fetch_state();
} // namespace state
