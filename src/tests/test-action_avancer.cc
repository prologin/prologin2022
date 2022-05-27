#include <cstddef>
#include <utils/log.hh>

#include "../constant.hh"
#include "../position.hh"
#include "../troupe.hh"
#include "test-helpers.hh"

namespace
{
void assert_troupe_moved(troupe& trp_after, troupe& trp_before, direction dir,
                         int line)
{
    auto delta = get_delta_pos(dir);
    ASSERT_EQ(trp_before.maman + delta, trp_after.maman)
        << "First Duck did not move accordingly line: " << line;

    for (int i = trp_before.taille - 1; i > 0; --i)
    {
        ASSERT_EQ(trp_before.canards[i - 1], trp_after.canards[i])
            << "Duck number " << i
            << ", did not move accordingly, line: " << line;
    }
}

void assert_troupe_died(erreur err, GameState& game_state, PlayerInfo& player,
                        std::vector<position>& former, troupe* trp,
                        int former_inv, int line)
{
    ASSERT_EQ(OK, err) << "failed line " << line;

	ASSERT_EQ(trp->taille, trp->canards.size()); // Coherance test

    for (auto& pos : former)
        ASSERT_FALSE(game_state.get_map().get_cell(pos).canard_sur_case)
            << line;
	for (auto i = 0lu; i < former.size(); ++i)
	{
		INFO("%d, %d", i, get_carrier(i));
		if (former_inv > 0 && i == static_cast<size_t>(get_carrier(i)))
		{
			ASSERT_EQ(1, game_state.get_map().get_cell(former[i]).etat.nb_pains)
            	<< line;
			former_inv--;
		}
		else	
        	ASSERT_EQ(0, game_state.get_map().get_cell(former[i]).etat.nb_pains)
            	<< line;
	}

	ASSERT_EQ(0, former_inv);
    ASSERT_EQ(0, trp->inventaire) << line;
    ASSERT_EQ(static_cast<size_t>(1), player.get_troupe(1)->canards.size())
        << line;
    ASSERT_EQ(static_cast<size_t>(TAILLE_DEPART - 1),
              player.canards_additionnels(trp->id)->size())
        << line;
}

void place_trp(troupe* trp, std::vector<position> pos, Map& map,
				PlayerInfo& player)
{
    map.delete_troupe(*trp);
    trp->canards.clear();
    trp->taille = pos.size();
    for (size_t i = 0; i < pos.size(); ++i)
        trp->canards.push_back(pos[i]);
    trp->maman = trp->canards[0];
    map.mark_troupe(*trp, player);
}

erreur test_move_troupe(PlayerInfo* player, std::unique_ptr<Api>& api, int id,
                        direction dir, bool moved, int line)
{
    auto before = *(player->get_troupe(id));

    auto err = api->avancer(id, dir);

    troupe after = *(player->get_troupe(id));

    // ASSERT_EQ(err == OK, moved) << "failed line: " << line;
    if (moved)
        assert_troupe_moved(after, before, dir, line);
    return err;
}

void troup_hardcoded_setup(troupe* trp, PlayerInfo& player, Map& map)
{
    map.delete_troupe(*trp);
    trp->canards.reserve(5);
    trp->canards[0] = {.colonne = 27, .ligne = 38, .niveau = 0};
    trp->canards[1] = {.colonne = 26, .ligne = 38, .niveau = 0};
    trp->canards[2] = {.colonne = 25, .ligne = 38, .niveau = 0};
    trp->canards[3] = {.colonne = 24, .ligne = 38, .niveau = 0};
    trp->canards[4] = {.colonne = 23, .ligne = 38, .niveau = 0};
    trp->maman = trp->canards[0];
    map.mark_troupe(*trp, player);
}

} // namespace

TEST_F(ApiTest, ActionAvancerAutorisee)
{
    auto& player = players[0];
    auto trp = player.info->get_troupe(1);
    int original = trp->pts_actions;
    auto former_size = trp->canards.size();

    troup_hardcoded_setup(trp, *player.info, player.api->game_state().get_map());

    auto err =
        test_move_troupe(player.info, player.api, 1, EST, true, __LINE__);
    ASSERT_EQ(OK, err);
    ASSERT_EQ(original - 1, trp->pts_actions);
    ASSERT_EQ(former_size + 1, trp->canards.size());
}

TEST_F(ApiTest, ActionAvancerInvalidTroupe)
{
    auto& player = players[0];
    direction dir = EST;
    auto trp = player.info->get_troupe(1);
    int original = trp->pts_actions;

    auto err = player.api->avancer(NB_TROUPES + 42, dir);

    ASSERT_EQ(TROUPE_INVALIDE, err);
    ASSERT_EQ(original, trp->pts_actions);
}

TEST_F(ApiTest, ActionAvancerTueLaTroupeSurBuisson)
{
    auto& player = players[0];
    std::vector<position> new_pos = {
        {.colonne = 22, .ligne = 35, .niveau = 0},
        {.colonne = 22, .ligne = 36, .niveau = 0},
        {.colonne = 22, .ligne = 37, .niveau = 0},
        {.colonne = 22, .ligne = 38, .niveau = 0},
        {.colonne = 21, .ligne = 38, .niveau = 0},
    };

    troupe* trp = player.info->get_troupe(1);
    place_trp(trp, new_pos, player.api->game_state().get_map(), *player.info);
    std::vector<position> former = trp->canards;
    int former_inv = trp->inventaire;
    auto err = player.api->avancer(1, SUD);

    assert_troupe_died(err, player.api->game_state(), *(player.info), former,
                       trp, former_inv, __LINE__);
}

TEST_F(ApiTest, ActionAvancerTueLaTroupeSurBarriere)
{
    auto& player = players[0];
    std::vector<position> new_pos = {
        {.colonne = 16, .ligne = 36, .niveau = 0},
        {.colonne = 17, .ligne = 36, .niveau = 0},
        {.colonne = 18, .ligne = 36, .niveau = 0},
        {.colonne = 19, .ligne = 36, .niveau = 0},
        {.colonne = 20, .ligne = 36, .niveau = 0},
    };

    troupe* trp = player.info->get_troupe(1);
    place_trp(trp, new_pos, player.api->game_state().get_map(), *player.info);
    std::vector<position> former = trp->canards;
    int former_inv = trp->inventaire;

    auto err = player.api->avancer(1, OUEST);

    assert_troupe_died(err, player.api->game_state(), *(player.info), former,
                       trp, former_inv, __LINE__);
}

TEST_F(ApiTest, ActionAvancerTueLaTroupeSurCanard)
{
    auto& player = players[0];
    std::vector<position> new_pos = {
        {.colonne = 26, .ligne = 37, .niveau = 0},
        {.colonne = 27, .ligne = 37, .niveau = 0},
        {.colonne = 28, .ligne = 37, .niveau = 0},
        {.colonne = 29, .ligne = 37, .niveau = 0},
        {.colonne = 30, .ligne = 37, .niveau = 0},
    };

    std::vector<position> new_pos2 = {
        {.colonne = 25, .ligne = 34, .niveau = 0},
        {.colonne = 25, .ligne = 35, .niveau = 0},
        {.colonne = 25, .ligne = 36, .niveau = 0},
        {.colonne = 25, .ligne = 37, .niveau = 0},
        {.colonne = 25, .ligne = 38, .niveau = 0},
    };

    troupe* trp = player.info->get_troupe(1);

    place_trp(players[1].info->get_troupe(1), new_pos2,
              players[1].api->game_state().get_map(), *player.info);
    place_trp(trp, new_pos, player.api->game_state().get_map(), *player.info);

    int former_inv = trp->inventaire;
    std::vector<position> former = trp->canards;
    auto err = player.api->avancer(1, EST);

    assert_troupe_died(err, player.api->game_state(), *(player.info), former,
                       trp, former_inv, __LINE__);
}

TEST_F(ApiTest, Actio1AvancerTueAvecInventaire)
{
    auto& player = players[0];
    std::vector<position> new_pos = {
        {.colonne = 16, .ligne = 38, .niveau = 0},
        {.colonne = 15, .ligne = 38, .niveau = 0},
        {.colonne = 14, .ligne = 38, .niveau = 0},
        {.colonne = 13, .ligne = 38, .niveau = 0},
        {.colonne = 12, .ligne = 38, .niveau = 0},
        {.colonne = 11, .ligne = 38, .niveau = 0},
    };

    troupe* trp = player.info->get_troupe(1);
    place_trp(trp, new_pos, player.api->game_state().get_map(), *player.info);

    trp->inventaire = 2;
    std::vector<position> former = trp->canards;
    int former_inv = trp->inventaire;

    auto err = player.api->avancer(1, OUEST);

    assert_troupe_died(err, player.api->game_state(), *(player.info), former,
                       trp, former_inv, __LINE__);
}

TEST_F(ApiTest, ActionAvancerAvecSpawn)
{
    auto& player = players[0];
    auto trp = player.info->get_troupe(1);
    troup_hardcoded_setup(trp, *player.info,
					player.api->game_state().get_map());

    auto former_size = trp->canards.size();

    auto size_before = player.info->canards_additionnels(1)->size();
    player.info->enfiler_canard(1);
    test_move_troupe(player.info, player.api, 1, EST, true, __LINE__);

    EXPECT_EQ(size_before, player.info->canards_additionnels(1)->size());
    ASSERT_EQ(static_cast<size_t>(former_size + 1), trp->canards.size());
}

TEST_F(ApiTest, ActionAvancerCaptureNid)
{
    auto& player = players[0];
    std::vector<position> new_pos = {
        {.colonne = 32, .ligne = 36, .niveau = 0},
        {.colonne = 31, .ligne = 36, .niveau = 0},
        {.colonne = 30, .ligne = 36, .niveau = 0},
        {.colonne = 29, .ligne = 36, .niveau = 0},
        {.colonne = 28, .ligne = 36, .niveau = 0},
    };

    position nid = {.colonne = 33, .ligne = 36, .niveau = 0};

    troupe* trp = player.info->get_troupe(1);
    place_trp(trp, new_pos, player.api->game_state().get_map(), *player.info);
    auto& map = player.api->game_state().get_map();

    player.api->avancer(1, EST);

    ASSERT_EQ(player.info->get_player_nid_id(), map.get_cell(nid).nid);
}

TEST_F(ApiTest, ActionAvancerDeposerNid)
{
    auto& player = players[0];
    std::vector<position> new_pos = {
        {.colonne = 32, .ligne = 36, .niveau = 0},
        {.colonne = 31, .ligne = 36, .niveau = 0},
        {.colonne = 30, .ligne = 36, .niveau = 0},
        {.colonne = 29, .ligne = 36, .niveau = 0},
        {.colonne = 28, .ligne = 36, .niveau = 0},
    };

    position nid = {.colonne = 33, .ligne = 36, .niveau = 0};

    troupe* trp = player.info->get_troupe(1);
    trp->inventaire = 2;

    place_trp(trp, new_pos, player.api->game_state().get_map(), *player.info);
    auto& map = player.api->game_state().get_map();
    map.get_cell(nid).nid = player.info->get_player_nid_id();

    auto former_score = player.info->get_score();

    player.api->avancer(1, EST);

    ASSERT_EQ(0, trp->inventaire);
    ASSERT_NE(former_score, player.info->get_score());
}

TEST_F(ApiTest, ActionAvancerDeposerNidInvVide)
{
    auto& player = players[0];
    std::vector<position> new_pos = {
        {.colonne = 32, .ligne = 36, .niveau = 0},
        {.colonne = 31, .ligne = 36, .niveau = 0},
        {.colonne = 30, .ligne = 36, .niveau = 0},
        {.colonne = 29, .ligne = 36, .niveau = 0},
        {.colonne = 28, .ligne = 36, .niveau = 0},
    };

    position nid = {.colonne = 33, .ligne = 36, .niveau = 0};

    troupe* trp = player.info->get_troupe(1);
    place_trp(trp, new_pos, player.api->game_state().get_map(), *player.info);
    auto& map = player.api->game_state().get_map();
    map.get_cell(nid).nid = player.info->get_player_nid_id();

    auto former_score = player.info->get_score();

    player.api->avancer(1, EST);

    ASSERT_EQ(0, trp->inventaire);
    ASSERT_EQ(former_score, player.info->get_score());
}

TEST_F(ApiTest, ActionAvancerCaptureEtDeposerNid)
{
    auto& player = players[0];
    std::vector<position> new_pos = {
        {.colonne = 32, .ligne = 36, .niveau = 0},
        {.colonne = 31, .ligne = 36, .niveau = 0},
        {.colonne = 30, .ligne = 36, .niveau = 0},
        {.colonne = 29, .ligne = 36, .niveau = 0},
        {.colonne = 28, .ligne = 36, .niveau = 0},
    };

    position nid = {.colonne = 33, .ligne = 36, .niveau = 0};

    troupe* trp = player.info->get_troupe(1);
    place_trp(trp, new_pos, player.api->game_state().get_map(), *player.info);
    auto& map = player.api->game_state().get_map();

    player.api->avancer(1, EST);

    ASSERT_EQ(player.info->get_player_nid_id(), map.get_cell(nid).nid);
}

TEST_F(ApiTest, ActionAvancerRamasserPains)
{
    auto& player = players[0];
    std::vector<position> new_pos = {
        {.colonne = 26, .ligne = 37, .niveau = 0},
        {.colonne = 25, .ligne = 37, .niveau = 0},
        {.colonne = 24, .ligne = 37, .niveau = 0},
        {.colonne = 23, .ligne = 37, .niveau = 0},
        {.colonne = 22, .ligne = 37, .niveau = 0},
    };

    position pains = {.colonne = 27, .ligne = 37, .niveau = 0};
    troupe* trp = player.info->get_troupe(1);
    place_trp(trp, new_pos, player.api->game_state().get_map(), *player.info);
    auto& map = player.api->game_state().get_map();

    map.get_cell(pains).etat.nb_pains = 1;

    player.api->avancer(1, EST);

    ASSERT_EQ(0, map.get_cell(pains).etat.nb_pains);
    ASSERT_EQ(1, trp->inventaire);
}

TEST_F(ApiTest, ActionAvancerRamasserPainsInvPlein)
{
    auto& player = players[0];
    std::vector<position> new_pos = {
        {.colonne = 26, .ligne = 37, .niveau = 0},
        {.colonne = 25, .ligne = 37, .niveau = 0},
        {.colonne = 24, .ligne = 37, .niveau = 0},
        {.colonne = 23, .ligne = 37, .niveau = 0},
        {.colonne = 22, .ligne = 37, .niveau = 0},
    };

    position pains = {.colonne = 27, .ligne = 37, .niveau = 0};
    troupe* trp = player.info->get_troupe(1);
    place_trp(trp, new_pos, player.api->game_state().get_map(), *player.info);
    auto& map = player.api->game_state().get_map();

    map.get_cell(pains).etat.nb_pains = 15;
    trp->inventaire = 5;

    player.api->avancer(1, EST);

    EXPECT_EQ(15, map.get_cell(pains).etat.nb_pains);
    ASSERT_EQ(5, trp->inventaire);
}

TEST_F(ApiTest, ActionAvancerRamasserPainsInvPresquePlein)
{
    auto& player = players[0];
    std::vector<position> new_pos = {
        {.colonne = 26, .ligne = 37, .niveau = 0},
        {.colonne = 25, .ligne = 37, .niveau = 0},
        {.colonne = 24, .ligne = 37, .niveau = 0},
        {.colonne = 23, .ligne = 37, .niveau = 0},
        {.colonne = 22, .ligne = 37, .niveau = 0},
    };

    position pains = {.colonne = 27, .ligne = 37, .niveau = 0};
    troupe* trp = player.info->get_troupe(1);
    place_trp(trp, new_pos, player.api->game_state().get_map(), *player.info);
    auto& map = player.api->game_state().get_map();

    map.get_cell(pains).etat.nb_pains = 15;
    trp->inventaire = 3;

    player.api->avancer(1, EST);

    ASSERT_EQ(13, map.get_cell(pains).etat.nb_pains);
    ASSERT_EQ(5, trp->inventaire);
}
