#pragma once

#include <array>
#include <istream>
#include <vector>

#include "constant.hh"

typedef struct Case
{
    etat_case etat;          ///< Position, type, constructible et pain
    etat_barriere barriere;  ///< Informations d'état d'une barrière
    etat_nid nid;            ///< Informations d'état d'un nid
    pigeon_debug pigeon;     ///< Informations d'état du pigeon de debug
    int papy_tours_restants; ///< Tours restants avant la pose d'un pain
    bool point_spawn;        ///< La case est un point de spawn
} Case;

class Map
{
public:
    Map(std::istream& stream);
    Map(const Map& map);

private:
    void load_map_cells(std::istream& stream);

    std::array<Case, 2 * LARGEUR * HAUTEUR> map_;
};
