#pragma once

#include "constant.hh"
#include "utils.hh"


class Case
{
public:
    Case();
    etat_case etat() const;
    etat_nid nid() const;
    etat_barriere barriere() const;
};
