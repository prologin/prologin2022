#pragma once

struct Case
{
    etat_case etat;          ///< Position, type, constructible et pain
    etat_barriere barriere;  ///< Informations d'état d'une barrière
    etat_nid nid;            ///< Informations d'état d'un nid
    pigeon_debug pigeon;     ///< Informations d'état du pigeon de debug
    int papy_tours_restants; ///< Tours restants avant la pose d'un pain
};
