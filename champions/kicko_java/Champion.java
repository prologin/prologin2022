import java.awt.*;
import java.util.*;
import java.util.List;
import java.util.function.Function;

public class Champion extends Api
{
    Boolean find_nid = false;

    Troupe[] my_troupes;
    List<Position> possible_nids;
    List<Position> my_nids;
    List<Position> papys_positions;

    static Position create_pos(int ligne, int col, int niveau)
    {
        var pos = new Position();
        pos.colonne = col;
        pos.ligne = ligne;
        pos.niveau = niveau;
        return pos;
    }

    static Position create_pos(int ligne, int col)
    {
        return create_pos(ligne, col, 0);
    }

    void CheckNids()
    {
        List<Position> nids_to_remove = new ArrayList<>();
        for (Position p : possible_nids)
        {
            EtatNid etatNid = Api.info_nid(p);
            if (etatNid == EtatNid.PAS_DE_NID || (etatNid == EtatNid.JOUEUR_0 && Api.moi() != 0)
                || (etatNid == EtatNid.JOUEUR_1 && Api.moi() != 1))
            {
                nids_to_remove.add(p);
            }
        }

        for (Position p : nids_to_remove)
        {
            possible_nids.remove(p);
        }
    }

    class DijkstraNode
    {
        public int distance;
        public Position pos;
        public Direction direction_to;
        public Position preced_pos;

        public DijkstraNode(Position p)
        {
            distance = Integer.MAX_VALUE;
            pos = p;
        }
    }

    boolean CanWalk(EtatCase current_case)
    {
        // check if position already taken by a duck
        for (int id = 0; id < 2; ++id)
        {
            for (Troupe troupe : Api.troupes_joueur(id))
            {
                if (List.of(troupe.canards).contains(current_case.pos))
                {
                    return false;
                }
            }
        }

        // if the case is walkable
        if (current_case.contenu == TypeCase.BARRIERE)
        {
            return Api.info_barriere(current_case.pos) == EtatBarriere.OUVERTE;
        }

        return current_case.contenu == TypeCase.NID
                || current_case.contenu == TypeCase.GAZON || current_case.contenu == TypeCase.PAPY
                || current_case.contenu == TypeCase.TROU;
    }

    List<Map.Entry<Direction, Position>> PossibleDirectionFrom(Position source)
    {
        List<Map.Entry<Direction, Position>> directions = new ArrayList<>();

        Map<Position, Direction> cross = new HashMap<>();
        cross.put(create_pos(source.ligne, source.colonne - 1), Direction.OUEST);
        cross.put(create_pos(source.ligne - 1, source.colonne), Direction.SUD);
        cross.put(create_pos(source.ligne + 1, source.colonne), Direction.NORD);
        cross.put(create_pos(source.ligne, source.colonne + 1), Direction.EST);

        for (Position p: cross.keySet())
        {
            if (p.colonne >= 0 && p.colonne < LARGEUR && p.ligne >= 0 && p.ligne < HAUTEUR
                    && CanWalk(Api.info_case(p)))
            {
                directions.add(new AbstractMap.SimpleEntry<>(cross.get(p), p));
            }
        }

        return directions;
    }

    DijkstraNode[][] CreateDistanceMap(Position source)
    {
        DijkstraNode[][] nodes = new DijkstraNode[HAUTEUR][LARGEUR];
        for (int j = 0; j < HAUTEUR; ++j)
        {
            for (int i = 0; i < LARGEUR; ++i)
            {
                nodes[j][i] = new DijkstraNode(create_pos(j, i));
            }
        }
        nodes[source.ligne][source.colonne].distance = 0;

        Queue<DijkstraNode> queue = new PriorityQueue<>(
                Comparator.comparingInt(node -> node.distance)
        );
        queue.add(nodes[source.ligne][source.colonne]);

        // parcours largeur sur le labyrinthe, la source cherche la destination
        // remplit le tableau 'nodes' avec les distances depuis la source etc
        while (!queue.isEmpty())
        {
            DijkstraNode current_node = queue.poll();

            for (Map.Entry<Direction, Position> dir_pos : PossibleDirectionFrom(current_node.pos))
            {
                DijkstraNode neighbour_node = nodes[dir_pos.getValue().ligne][dir_pos.getValue().colonne];
                if (neighbour_node.distance > current_node.distance + 1)
                {
                    neighbour_node.distance = current_node.distance + 1;
                    neighbour_node.direction_to = dir_pos.getKey();
                    neighbour_node.preced_pos = current_node.pos;
                    queue.add(neighbour_node);
                }
            }
        }

        return nodes;
    }

    Stack<Direction> PathFinding(Position source, List<Position> possible_destinations)
    {
        if (possible_destinations == null || possible_destinations.size() < 1)
        {
            return null;
        }

        DijkstraNode[][] nodes = CreateDistanceMap(source);

        // closest destination in possible ones
        Position destination = possible_destinations.get(0);
        for (Position p : possible_destinations)
        {
            if (nodes[p.ligne][p.colonne].distance < nodes[destination.ligne][destination.colonne].distance) // TODO: & verifier qu'il peut revenir sans se marcher dessus
            {
                destination = p;
            }
        }

        // chemin non possible
        if (nodes[destination.ligne][destination.colonne].distance == Integer.MAX_VALUE)
        {
            return null;
        }

        // chemin vers la destination choisie
        var directions = new Stack<Direction>();
        Position current_pos = destination;
        while (current_pos != source)
        {
            directions.add(nodes[current_pos.ligne][current_pos.colonne].direction_to);
            current_pos = nodes[current_pos.ligne][current_pos.colonne].preced_pos;
        }
        return directions;
    }

    /**
     * Fonction appelée au début de la partie.
     */
    public void partie_init()
    {
        my_nids = new ArrayList<>();

        possible_nids = new ArrayList<>();
        papys_positions = new ArrayList<>();
        for (int j = 0; j < HAUTEUR;  ++j)
        {
            for (int i = 0; i < LARGEUR; ++i)
            {
                Position current_pos = create_pos(j, i);
                EtatCase current_case = Api.info_case(current_pos);
                switch (current_case.contenu)
                {
                    case NID -> possible_nids.add(current_case.pos);
                    case PAPY -> papys_positions.add(current_case.pos);
                }
            }
        }
    }

    private void SearchForNid()
    {
        int dist = Integer.MAX_VALUE;
        Stack<Direction> path_nid = null;
        int id_troupe = 0;
        for (int i = 0; i < my_troupes.length; ++i)
        {
            Stack<Direction> path = PathFinding(my_troupes[i].maman, possible_nids);
            if (path != null && path.size() < dist)
            {
                path_nid = path;
                dist = path.size();
                id_troupe = i;
            }
        }

        if (path_nid == null)
        {
            // error: no nid available/reachable
            System.out.println("wtf, no nid available/reachable");
            return;
        }

        for (int i = 0; i < Integer.min(my_troupes[id_troupe].pts_action, path_nid.size()); ++i)
        {
            Api.avancer(id_troupe, path_nid.pop());
        }

        if (path_nid.empty())
        {
            find_nid = true;
            my_nids.add(my_troupes[id_troupe].maman);
            if ((Api.moi() == 0 && Api.info_nid(my_troupes[id_troupe].maman) != EtatNid.JOUEUR_0)
                    || (Api.moi() == 1 && Api.info_nid(my_troupes[id_troupe].maman) != EtatNid.JOUEUR_1))
            {
                System.out.println("wtf, je suis sur le nid normalement" + my_troupes[id_troupe].maman);
            }
        }
    }

    void BackToNid()
    {
        int dist = Integer.MAX_VALUE;
        Stack<Direction> path_nid = null;
        int id_troupe = 0;
        for (int i = 0; i < my_troupes.length; ++i)
        {
            Stack<Direction> path = PathFinding(my_troupes[i].maman, possible_nids);
            if (path != null && path.size() < dist)
            {
                path_nid = path;
                dist = path.size();
                id_troupe = i;
            }
        }

        if (path_nid == null)
        {
            // error: no nid available/reachable
            System.out.println("wtf, no nid available/reachable");
            return;
        }

        for (int i = 0; i < Integer.min(my_troupes[id_troupe].pts_action, path_nid.size()); ++i)
        {
            Api.avancer(id_troupe, path_nid.pop());
        }

        if (path_nid.empty())
        {
            find_nid = true;
            my_nids.add(my_troupes[id_troupe].maman);
            if ((Api.moi() == 0 && Api.info_nid(my_troupes[id_troupe].maman) != EtatNid.JOUEUR_0)
                    || (Api.moi() == 1 && Api.info_nid(my_troupes[id_troupe].maman) != EtatNid.JOUEUR_1))
            {
                System.out.println("wtf, je suis sur le nid normalement" + my_troupes[id_troupe].maman);
            }
        }
    }

    void GoToPosition(Troupe troupe, Position destination)
    {
        List<Position> dest = new ArrayList<>();
        dest.add(destination);
        Stack<Direction> path = PathFinding(troupe.maman, dest);

        if (path == null)
        {
            // error: no path available/reachable
            System.out.println("wtf, no path available/reachable");
            return;
        }

        for (int i = 0; i < Integer.min(troupe.pts_action, path.size()); ++i)
        {
            Api.avancer(troupe.id, path.pop());
        }
    }

    @FunctionalInterface
    public interface PainCalculator {
        int compute_pains(int nb_tours);
    }

    /**
     * Fonction appelée à chaque tour.
     */
    public void jouer_tour()
    {
        my_troupes = Api.troupes_joueur(Api.moi());

        CheckNids();

        // Troupe 1: search for nid if not already taken
        if (!find_nid)
        {
            SearchForNid();
        }

        // Troupe 2: search for closest pain (take into account the time to take it, and the release of pain by papys)
        Map<Position, PainCalculator> nb_miches = new HashMap<>(); // liste des papys et le nombre de miches qu'ils donneraient en un nombre de tours
        for (Position papy : papys_positions)
        {
            EtatCase current_papy = Api.info_case(papy);
            PainCalculator c = tour -> current_papy.nb_pains + (tour > Api.papy_tours_restants(papy) ? 1 : 0) + ((tour - Api.papy_tours_restants(papy)) / INTERVALLE_DISTRIB);
            nb_miches.put(papy, c);
        }

        // pour l'instant tout bete: va vers la miche de pain et la ramene au nid
        for (Troupe t : my_troupes)
        {
            while (t.pts_action > 0)
            {
                if (t.inventaire > 0)
                {
                    BackToNid();
                }
                else {
                    DijkstraNode[][] nodes = CreateDistanceMap(t.maman);
                    Position dest = null;
                    for (Position p: nb_miches.keySet())
                    {
                        // TODO: choisir plus intelligemment
                        if (dest == null || nodes[p.ligne][p.colonne].distance < nodes[dest.ligne][dest.colonne].distance)
                        {
                            dest = p;
                        }
                    }

                    if (dest == null)
                        break;

                    GoToPosition(t, dest);
                }
            }
        }
    }

    /**
     * Fonction appelée à la fin de la partie.
     */
    public void partie_fin()
    {
    }
}
