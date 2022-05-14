#include <utils/log.hh>

#include "../constant.hh"
#include "../position.hh"
#include "test-helpers.hh"

namespace
{
void assert_troupe_moved(troupe& trp_after, troupe& trp_before, direction dir,
                         int line)
{
    auto delta = get_delta_pos(dir);
    ASSERT_EQ(trp_before.maman + delta, trp_after.maman)
        << "First Duck did not move accordingly line: " << line;

    for (int i = trp_after.taille - 1; i > 0; --i)
    {
        ASSERT_EQ(trp_before.canards[i - 1], trp_after.canards[i])
            << "Duck number " << i
            << ", did not move accordingly, line: " << line;
    }
}

void assert_troupe_died(erreur err, GameState& game_state, PlayerInfo& player,
                std::vector<position>& former, troupe *trp, int former_inv,
				int line)
{
    ASSERT_EQ(OK, err) << "failed line " << line;

    for (auto& pos : former)
		ASSERT_FALSE(game_state.get_map().get_cell(pos).canard_sur_case);

	for (unsigned long i = former.size() - former_inv; i < former.size(); i++)
		ASSERT_EQ(1, game_state.get_map().get_cell(former[i])
						.etat.nb_pains);

	ASSERT_EQ(0, trp->inventaire);
	ASSERT_EQ(1, player.get_troupe(1)->canards.size());
	ASSERT_EQ(TAILLE_DEPART - 1, player.canards_additionnels(trp->id)->size());
}

void place_trp(troupe *trp, std::vector<position> pos, Map& map)
{
    if (trp->canards.size() != pos.size())
        FATAL("new_pos must have the same size as the number of ducks");
    map.delete_troupe(*trp);
    for (auto i = 0; i < trp->canards.size(); ++i)
    {
        trp->canards[i] = pos[i];
    }
    trp->maman = trp->canards[0];
    map.mark_troupe(*trp);
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

void mark_troupe_on_map(troupe* trp, Map& map)
{
    for (auto& pos : trp->canards)
        map.get_cell(pos).canard_sur_case = true;
}

void troup_hardcoded_setup(troupe* trp, Map& map)
{
    map.delete_troupe(*trp);
    trp->canards[0] = {.colonne = 50, .ligne = 78, .niveau = 0};
    trp->canards[1] = {.colonne = 49, .ligne = 78, .niveau = 0};
    trp->canards[2] = {.colonne = 48, .ligne = 78, .niveau = 0};
    trp->canards[3] = {.colonne = 47, .ligne = 78, .niveau = 0};
    trp->canards[4] = {.colonne = 46, .ligne = 78, .niveau = 0};
    trp->maman = trp->canards[0];
    map.mark_troupe(*trp);
}

} // namespace

TEST_F(ApiTest, ActionAvancerAutorisee)
{
    auto& player = players[0];
    auto trp = player.info->get_troupe(1);
    int original = trp->pts_actions;
    auto former_size = trp->canards.size();

    troup_hardcoded_setup(trp,
                          player.api->game_state().get_map());

    auto err =
        test_move_troupe(player.info, player.api, 1, EST, true, __LINE__);
    ASSERT_EQ(OK, err);
    ASSERT_EQ(original - 1, trp->pts_actions);
    ASSERT_EQ(former_size, trp->canards.size());
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
    std::vector<position> new_pos = 
    {
        {.colonne = 15, .ligne = 38, .niveau = 0},
        {.colonne = 15, .ligne = 39, .niveau = 0},
        {.colonne = 15, .ligne = 40, .niveau = 0},
        {.colonne = 15, .ligne = 41, .niveau = 0},
        {.colonne = 15, .ligne = 42, .niveau = 0},
    }; 

    troupe *trp = player.info->get_troupe(1); 
    place_trp(trp, new_pos, player.api->game_state().get_map());
    std::vector<position> former = trp->canards;
	int former_inv = trp->inventaire;

    auto err = player.api->avancer(1, SUD);

    assert_troupe_died(err, player.api->game_state(), *(player.info), former,
                    trp, former_inv, __LINE__);
}

TEST_F(ApiTest, ActionAvancerTueLaTroupeSurBarriere)
{
    auto& player = players[0];
    std::vector<position> new_pos = 
    {
        {.colonne = 31 , .ligne = 32, .niveau = 0},
        {.colonne = 32, .ligne = 32, .niveau = 0},
        {.colonne = 33, .ligne = 32, .niveau = 0},
        {.colonne = 34, .ligne = 32, .niveau = 0},
        {.colonne = 35, .ligne = 32, .niveau = 0},
    }; 

    troupe *trp = player.info->get_troupe(1); 
    place_trp(trp, new_pos, player.api->game_state().get_map());
    std::vector<position> former = trp->canards;
	int former_inv = trp->inventaire;

    auto err = player.api->avancer(1, OUEST);

    assert_troupe_died(err, player.api->game_state(), *(player.info), former,
                    trp, former_inv, __LINE__);
}

TEST_F(ApiTest, ActionAvancerTueLaTroupeSurCanard)
{
    auto& player = players[0];
    std::vector<position> new_pos = 
    {
        {.colonne = 31, .ligne = 41, .niveau = 0},
        {.colonne = 32, .ligne = 41, .niveau = 0},
        {.colonne = 33, .ligne = 41, .niveau = 0},
        {.colonne = 34, .ligne = 41, .niveau = 0},
        {.colonne = 35, .ligne = 41, .niveau = 0},
    };


    std::vector<position> new_pos2 = 
    {
        {.colonne = 30, .ligne = 40, .niveau = 0},
        {.colonne = 30, .ligne = 41, .niveau = 0},
        {.colonne = 30, .ligne = 42, .niveau = 0},
        {.colonne = 30, .ligne = 43, .niveau = 0},
        {.colonne = 30, .ligne = 44, .niveau = 0},
    };


    troupe *trp = player.info->get_troupe(1); 

    place_trp(players[1].info->get_troupe(1), new_pos2,
                    players[1].api->game_state().get_map());
    place_trp(trp, new_pos, player.api->game_state().get_map());
    
   	int former_inv = trp->inventaire; 
    std::vector<position> former = trp->canards;
    auto err = player.api->avancer(1, EST);

    assert_troupe_died(err, player.api->game_state(), *(player.info), former,
                    trp, former_inv, __LINE__);
}

TEST_F(ApiTest, ActionAvancerTueAvecInventaire)
{
    auto& player = players[0];
    std::vector<position> new_pos = 
    {
        {.colonne = 31 , .ligne = 32, .niveau = 0},
        {.colonne = 32, .ligne = 32, .niveau = 0},
        {.colonne = 33, .ligne = 32, .niveau = 0},
        {.colonne = 34, .ligne = 32, .niveau = 0},
        {.colonne = 35, .ligne = 32, .niveau = 0},
    }; 

    troupe *trp = player.info->get_troupe(1); 
    place_trp(trp, new_pos, player.api->game_state().get_map());
	trp->inventaire = 3;
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
    troup_hardcoded_setup(trp, player.api->game_state().get_map());

    auto former_size = trp->canards.size();

    player.info->enfiler_canard(1);    
    test_move_troupe(player.info, player.api, 1, EST, true, __LINE__);
    
    ASSERT_EQ(0, player.info->canards_additionnels(1)->size()); 
    ASSERT_EQ(former_size + 1, trp->canards.size());
}
