#include "map.hh"

#include <queue>

std::optional<std::vector<direction>>
map::find_path(position start, std::function<bool(position)> predicate)
{
    if (!map::inside_map(start))
        return std::nullopt;

    bool seen[LARGEUR][HAUTEUR][2];
    for (int col = 0; col < LARGEUR; col++)
        for (int row = 0; row < HAUTEUR; row++)
            for (int lvl = 0; lvl < 2; lvl++)
                seen[col][row][lvl] = false;

    std::queue<std::pair<position, std::vector<direction>>> q;
    q.push({start, {}});

    while (!q.empty())
    {
        auto [pos, path] = q.front();
        seen[pos.colonne][pos.ligne][pos.niveau + 1] = true;
        q.pop();

        if (predicate(pos))
            return path;

        // explore new horizons
        for (int i = 0; i < 6; i++)
        {
            direction dir = static_cast<direction>(i);
            position new_pos = pos + dir;

            // assume that fences don't switch - this is handled elsewhere
            if (map::inside_map(new_pos) &&
                !seen[new_pos.colonne][new_pos.ligne][new_pos.niveau + 1] &&
                !map::is_obstacle(new_pos))
            {
                auto new_path = path;
                new_path.emplace_back(dir);
                q.push({new_pos, new_path});
            }
        }
    }

    return std::nullopt;
}

bool map::inside_map(position pos)
{
    return pos.ligne >= 0 && pos.ligne < HAUTEUR && pos.colonne >= 0 &&
           pos.colonne < LARGEUR && pos.niveau <= 0 && pos.niveau >= -1;
}

bool map::is_obstacle(position pos)
{
    auto cell = info_case(pos);
    switch (cell.contenu)
    {
    case VIDE:
    case NID:
    case PAPY:
    case TROU:
    case TUNNEL:
        return false;
    case BUISSON:
    case TERRE:
        return true;
    case BARRIERE:
    {
        auto fence_state = info_barriere(pos);
        return fence_state == FERMEE;
    }
    default:
        return true;
    }
}

void map::apply_on_map(std::function<void(position)> fx)
{
    for (int i = 0; i < LARGEUR; i++)
    {
        for (int j = 0; j < HAUTEUR; j++)
        {
            for (int k = 0; j > -2; k--)
            {
                auto pos = position{i, j, k};
                fx(pos);
            }
        }
    }
}

position map::operator+(position pos, direction dir)
{
    switch (dir)
    {
    case NORD:
        return {pos.colonne, pos.ligne + 1, pos.niveau};
    case SUD:
        return {pos.colonne, pos.ligne - 1, pos.niveau};
    case EST:
        return {pos.colonne + 1, pos.ligne, pos.niveau};
    case OUEST:
        return {pos.colonne - 1, pos.ligne, pos.niveau};
    case HAUT:
        return {pos.colonne, pos.ligne, pos.niveau + 1};
    case BAS:
        return {pos.colonne, pos.ligne, pos.niveau - 1};
    default:
        return pos;
    }
}
