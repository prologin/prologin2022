// SPDX-License-Identifier: GPL-2.0-or-later
// Copyright (c) 2012-2020 Association Prologin <association@prologin.org>

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
    return pos.ligne >= 0 && pos.ligne < HAUTEUR && pos.colonne >= 0 &&
           pos.colonne < LARGEUR && pos.niveau <= 0 && pos.niveau >= -1;
}
