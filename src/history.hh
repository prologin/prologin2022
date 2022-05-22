// SPDX-License-Identifier: GPL-2.0-or-later
// Copyright (c) 2022 Association Prologin <association@prologin.org>

#pragma once

#include <iostream>
#include <string>

#include "constant.hh"

enum internal_action_type
{
    flag,
    standard_action,
};

typedef struct flag_info
{
    position pos;
    pigeon_debug ctype;
} flag_info;

// We do not want unused debug flag info to appear in the public API, so using
// only action_hist is not enough to store the complete internal history.
typedef struct internal_action
{
    internal_action_type type;
    std::string json;

    union
    {
        flag_info flag;
        action_hist action;
    };
} internal_action;
