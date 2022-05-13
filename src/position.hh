// SPDX-License-Identifier: GPL-2.0-or-later
// Copyright (c) 2012-2020 Association Prologin <association@prologin.org>
#pragma once

#include <utils/log.hh>

#include "constant.hh"

inline bool operator==(const position& lhs, const position& rhs)
{
    return lhs.ligne == rhs.ligne && lhs.colonne == rhs.colonne &&
           lhs.niveau == rhs.niveau;
}

inline bool operator!=(const position& lhs, const position& rhs)
{
    return !(lhs == rhs);
}

inline position& operator+=(position& lhs, position rhs)
{
    lhs.colonne += rhs.colonne;
    lhs.ligne += rhs.ligne;
    lhs.niveau += rhs.niveau;
    return lhs;
}

inline position operator+(position lhs, position rhs)
{
    return lhs += rhs;
}

inline bool inside_map(const position& pos)
{
    INFO("%d, %d, %d", pos.ligne, pos.colonne, pos.niveau);
    return pos.ligne >= 0 && pos.ligne < HAUTEUR && pos.colonne >= 0 &&
           pos.colonne < LARGEUR && pos.niveau <= 0 && pos.niveau >= -1;
}

inline position get_delta_pos(const direction& dir)
{
    position delta = {.colonne = 0, .ligne = 0, .niveau = 0};
    switch (dir)
    {
    case NORD:
        delta.ligne = 1;
        break;
    case SUD:
        delta.ligne = -1;
        break;
    case EST:
        delta.colonne = 1;
        break;
    case OUEST:
        delta.colonne = -1;
        break;
    case HAUT:
        delta.niveau = 1;
        break;
    case BAS:
        delta.niveau -= 1;
        break;
    }
    return delta;
}

inline direction inverse_dir(direction dir)
{
    switch (dir)
    {
    case NORD:
        return SUD;
    case SUD:
        return NORD;
    case EST:
        return OUEST;
    case OUEST:
        return EST;
    case HAUT:
        return BAS;
    case BAS:
        return HAUT;
    }

    __builtin_unreachable();
}

inline direction clockwise_dir(direction dir)
{
    switch (dir)
    {
    case NORD:
        return EST;
    case SUD:
        return OUEST;
    case EST:
        return SUD;
    case OUEST:
        return NORD;
    case HAUT:
        return BAS;
    case BAS:
        return HAUT;
    }

    __builtin_unreachable();
}
