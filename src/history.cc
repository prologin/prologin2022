#include "history.hh"

InternalAction::InternalAction(InternalActionType type, int val1, int val2)
    : type_(type)
    , int_value1_(val1)
    , int_value2_(val2)
{}

InternalAction::InternalAction(InternalActionType type, int val1, int val2,
                               int val3)
    : type_(type)
    , int_value1_(val1)
    , int_value2_(val2)
    , int_value3_(val3)
{}

InternalAction::InternalAction(InternalActionType type, int val1, int val2,
                               direction dir)
    : type_(type)
    , direction_(dir)
    , int_value1_(val1)
    , int_value2_(val2)
{}

InternalAction::InternalAction(InternalActionType type, int val1, int val2,
                               position pos)
    : type_(type)
    , pos_(pos)
    , int_value1_(val1)
    , int_value2_(val2)
{}

InternalAction::InternalAction(InternalActionType type, int val1, int val2,
                               position pos, int val3)
    : type_(type)
    , pos_(pos)
    , int_value1_(val1)
    , int_value2_(val2)
    , int_value3_(val3)
{}

InternalAction::InternalAction(InternalActionType type, position pos)
    : type_(type)
    , pos_(pos)
{}

InternalAction::InternalAction(InternalActionType type, position pos,
                               pigeon_debug pigeon)
    : type_(type)
    , pos_(pos)
    , pigeon_(pigeon)
{}

InternalAction::InternalAction(InternalActionType type, int val, erreur err)
    : type_(type)
    , err_(err)
    , int_value1_(val)
{}