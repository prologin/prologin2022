#include <gtest/gtest.h>

#include "../constant.hh"
#include "../map.hh"

TEST(CasePraticable, BarriereOuverte)
{
    Case cell = {.etat = {.contenu = BARRIERE},
                .barriere = OUVERTE,
                .nid = PAS_DE_NID,};

    ASSERT_TRUE(cell.case_praticable());
}

TEST(CasePraticable, NidLibre)
{
    Case cell = {.etat = {.contenu = NID},
                .barriere = OUVERTE,
                .nid = LIBRE,};

    ASSERT_TRUE(cell.case_praticable());
}

TEST(CasePraticable, NidJ1)
{
    Case cell = {.etat = {.contenu = NID},
                .barriere = PAS_DE_BARRIERE,
                .nid = JOUEUR_0,};

    ASSERT_TRUE(cell.case_praticable());
}

TEST(CasePraticable, NidJ2)
{
    Case cell = {.etat = {.contenu = NID},
                .barriere = PAS_DE_BARRIERE,
                .nid = JOUEUR_1,};

    ASSERT_TRUE(cell.case_praticable());
}

TEST(CasePraticable, BUISSON)
{
    Case cell = {.etat = {.contenu = BUISSON},
                .barriere = PAS_DE_BARRIERE,
                .nid = LIBRE,};

    ASSERT_FALSE(cell.case_praticable());
}

TEST(CasePraticable, BarriereFermee)
{
    Case cell = {.etat = {.contenu = BARRIERE},
                .barriere = FERMEE,
                .nid = LIBRE,};

    ASSERT_FALSE(cell.case_praticable());
}
