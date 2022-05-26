#include <gtest/gtest.h>

#include "../constant.hh"
#include "../map.hh"

TEST(CasePraticable, BarriereOuverte)
{
    Case cell = {
        .etat =
            {
                .pos = {0, 0, 0},
                .contenu = BARRIERE,
                .est_constructible = false,
                .nb_pains = 0,
            },
        .barriere = OUVERTE,
        .nid = PAS_DE_NID,
        .papy_tours_restants = 0,
        .point_spawn = false,
        .canard_sur_case = false,
    };

    ASSERT_TRUE(cell.case_praticable());
}

TEST(CasePraticable, NidLibre)
{
    Case cell = {
        .etat =
            {
                .pos = {0, 0, 0},
                .contenu = NID,
                .est_constructible = false,
                .nb_pains = 0,
            },
        .barriere = OUVERTE,
        .nid = LIBRE,
        .papy_tours_restants = 0,
        .point_spawn = false,
        .canard_sur_case = false,
    };

    ASSERT_TRUE(cell.case_praticable());
}

TEST(CasePraticable, NidJ1)
{
    Case cell = {
        .etat =
            {
                .pos = {0, 0, 0},
                .contenu = NID,
                .est_constructible = false,
                .nb_pains = 0,
            },
        .barriere = PAS_DE_BARRIERE,
        .nid = JOUEUR_0,
        .papy_tours_restants = 0,
        .point_spawn = false,
        .canard_sur_case = false,
    };

    ASSERT_TRUE(cell.case_praticable());
}

TEST(CasePraticable, NidJ2)
{
    Case cell = {
        .etat =
            {
                .pos = {0, 0, 0},
                .contenu = NID,
                .est_constructible = false,
                .nb_pains = 0,
            },
        .barriere = PAS_DE_BARRIERE,
        .nid = JOUEUR_1,
        .papy_tours_restants = 0,
        .point_spawn = false,
        .canard_sur_case = false,
    };

    ASSERT_TRUE(cell.case_praticable());
}

TEST(CasePraticable, BUISSON)
{
    Case cell = {
        .etat =
            {
                .pos = {0, 0, 0},
                .contenu = BUISSON,
                .est_constructible = false,
                .nb_pains = 0,
            },
        .barriere = PAS_DE_BARRIERE,
        .nid = LIBRE,
        .papy_tours_restants = 0,
        .point_spawn = false,
        .canard_sur_case = false,
    };

    ASSERT_FALSE(cell.case_praticable());
}

TEST(CasePraticable, BarriereFermee)
{
    Case cell = {
        .etat =
            {
                .pos = {0, 0, 0},
                .contenu = BARRIERE,
                .est_constructible = false,
                .nb_pains = 0,
            },
        .barriere = FERMEE,
        .nid = LIBRE,
        .papy_tours_restants = 0,
        .point_spawn = false,
        .canard_sur_case = false,
    };

    ASSERT_FALSE(cell.case_praticable());
}
