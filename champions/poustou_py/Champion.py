from api import *

DIRECTIONS = [
    direction.HAUT,
    direction.BAS,
    direction.NORD,
    direction.SUD,
    direction.EST,
    direction.OUEST
    ]


DEBUG = False
# False pour desactiver debug
# True pour activer debug
# 0 pour activer debug si le champion est le joueur 0
# 1 pour activer debug si le champion est le joueur 1
def debug(*args, **kwargs):
    """
    Debug sur la sortie standard, si actif
    """
    if DEBUG is False:
        return
    if DEBUG is True or DEBUG == moi():
        print(*args, **kwargs)

def afficher_position(position, end = '\n'):
    """
    api.afficher_position custom, compatible avec la valeur de debug
    """
    debug(f"({position[0]}; {position[1]}; {position[2]})", end = end)

def afficher_direction(dir, end = '\n'):
    """
    api.afficher_direction custom, compatible avec la valeur de debug
    """
    if dir == direction.NORD:
        debug("NORD", end = end)
    elif dir == direction.SUD:
        debug("SUD", end = end)
    elif dir == direction.EST:
        debug("EST", end = end)
    elif dir == direction.OUEST:
        debug("OUEST", end = end)
    elif dir == direction.HAUT:
        debug("HAUT", end = end)
    elif dir == direction.BAS:
        debug("BAS", end = end)

def display_map():
    """
    Affiche le niveau principal de la carte en ascii.
    Compatible avec la valeur de debug
    """
    def display_case(case):
        contenu = case.contenu
        if contenu == type_case.GAZON:
            return '  '
        if contenu == type_case.BUISSON:
            return '██'
        if contenu == type_case.BARRIERE:
            if info_barriere(case.pos) == etat_barriere.OUVERTE:
                return '  '
            else:
                return '██'
        if contenu == type_case.NID:
            if info_nid(case.pos) == etat_nid.LIBRE:
                return 'O '
            if info_nid(case.pos) == moi() + 1:
                return 'O!'
            return 'Ox'
        if contenu == type_case.PAPY:
            return '0' + str(papy_tours_restants(case.pos))
        if contenu == type_case.TROU:
            return '╳ '
        if contenu == type_case.TUNNEL:
            return '  '
        return '██'

    pains_str = ['  ', ' .', '..', '.:', '::', ':…', '……'] + ['++'] * 100

    parc = [[['  '] * LARGEUR for _ in range(HAUTEUR)] for _ in range(2)]

    for z in range(-1, 1):
        for y in range(HAUTEUR):
            for x in range(LARGEUR):
                pos = x, y, z
                case = info_case(pos)
                parc[z][y][x] = display_case(case)
                if case.nb_pains > 0:
                    parc[z][y][x] = pains_str[case.nb_pains]


    for troupe in troupes_joueur(moi()):
        for x, y, z in troupe.canards:
            parc[z][y][x] = '||'
            if troupe.maman == (x, y, z):
                parc[z][y][x] = 'MM'

    for troupe in troupes_joueur(adversaire()):
        for x, y, z in troupe.canards:
            parc[z][y][x] = '||'
            if troupe.maman == (x, y, z):
                parc[z][y][x] = 'AA'


    
    for niveau in parc:
        for ligne in niveau:
            for case in ligne:
                debug(case, end = '')
            debug()
        debug('-------------')
    debug('=============================')


def trouver_chemin_arr(pos1, pos2):
    """
    api.trouver_chemin custom. Prend en compte la traversabilite de la case d'arrivee
    """
    if not case_traversable(pos2):
        return []
    return trouver_chemin(pos1, pos2)

def case_traversable(pos):
    """
    Determine si une case est mortelle.
    """
    x, y, z = pos
    if x < 0 or x >= LARGEUR or y < 0 or y >= LARGEUR or z < -1 or z > 0:
        return False
    case = info_case(pos)
    if case.contenu == type_case.BUISSON:
        return False
    if case.contenu == type_case.TERRE:
        return False
    if case.contenu == type_case.BARRIERE and info_barriere(pos) == etat_barriere.FERMEE:
        return False
    for troupe in troupes_joueur(moi()):
        for canard in troupe.canards:
            if canard == pos:
                return False
    for troupe in troupes_joueur(adversaire()):
        for canard in troupe.canards:
            if canard == pos:
                return False
    return True

def delta_pos(dir):
    """
    Retourne le vecteur unitaire de la direction
    """
    if dir == direction.NORD:
        return (0, 1, 0)
    if dir == direction.SUD:
        return (0, -1, 0)
    if dir == direction.EST:
        return (1, 0, 0)
    if dir == direction.OUEST:
        return (-1, 0, 0)
    if dir == direction.HAUT:
        return (0, 0, 1)
    if dir == direction.BAS:
        return (0, 0, -1)

def can_move(pos, dir):
    """
    Determine s'il est possible de se deplacer dans une direction sans mourir
    """
    x, y, z = pos
    if dir == direction.HAUT:
        if z == 0 or info_case((x, y, 0)).contenu != type_case.TROU:
            return False
    if dir == direction.BAS:
        if z == -1 or info_case((x, y, 0)).contenu != type_case.TROU:
            return False
    dx, dy, dz = delta_pos(dir)
    ax, ay, az = x + dx, y + dy, z + dz
    return case_traversable((ax, ay, az))


def chemin_oiseau(pos1, pos2):
    x1, y1, z1 = pos1
    x2, y2, z2, = pos2
    res = []

    # x1,y1 -> x2,y1
    for x in range(x1, x2+1):
        res.append((x, y1, z1))
    # x2,y1 -> x2,y2
    for y in range(y1, y2+1):
        res.append((x2, y, z2))

    return res

SPAWN_POINTS = []   # Liste des points d'apparition
SPAWN_POINT = {}    # Map des directions aux points d'apparition
NIDS_LIBRES = []    # Liste des nids libres
PAPYS = []          # Liste des papys 
TROUS = []          # Liste des trous
CONSTRUCTIBLES = [] # Liste des cases constructibles
MES_NIDS = []       # Liste de mes nids

A_CREUSER = []      # Liste des cases a creuser

# Fonction appelée au début de la partie.
def partie_init():

    display_map()

    # Trouve tous les points d'apparitions
    for troupe in troupes_joueur(moi()):
        SPAWN_POINTS.append(troupe.maman)
    for troupe in troupes_joueur(adversaire()):
        SPAWN_POINTS.append(troupe.maman)

    debug("Points d'apparition :")
    for spawn_point in SPAWN_POINTS:
        debug("  -", end = ' ')
        afficher_position(spawn_point)
    debug()


    # Lie les points d'apparitions a leur direction
    for spawn in SPAWN_POINTS:
        x, y, z = spawn
        if x == 0:
            SPAWN_POINT[direction.OUEST] = spawn
        if x == LARGEUR-1:
            SPAWN_POINT[direction.EST] = spawn
        if y == 0:
            SPAWN_POINT[direction.SUD] = spawn
            SPAWN_POINT[direction.BAS] = spawn
        if y == HAUTEUR-1:
            SPAWN_POINT[direction.NORD] = spawn
            SPAWN_POINT[direction.HAUT] = spawn
    
    debug("Points de reapparition :")
    for dir, pos in SPAWN_POINT.items():
        debug("  -", end = ' ')
        afficher_direction(dir, end = ' : ')
        afficher_position(pos)
    debug()

    # Trouve tous les nids, les papys, les trous et les cases constructibles.
    for y in range(HAUTEUR):
        for x in range(LARGEUR):
            case = info_case((x, y, 0))
            if case.contenu == type_case.NID:
                NIDS_LIBRES.append((x, y, 0))
            if case.contenu == type_case.TROU:
                TROUS.append((x, y, 0))
            if case.contenu == type_case.PAPY:
                PAPYS.append((x, y, 0))
            if case.contenu == type_case.GAZON and case.est_constructible:
                CONSTRUCTIBLES.append((x, y, 0))
 
    debug("Nids :")
    for pos in NIDS_LIBRES:
        debug("  -", end = ' ')
        afficher_position(pos)
    debug()

    debug("Papys :")
    for pos in PAPYS:
        debug("  -", end = ' ')
        afficher_position(pos)
    debug()

    debug("Trous :")
    for pos in TROUS:
        debug("  -", end = ' ')
        afficher_position(pos)
    debug()

    debug("Cases constructible :")
    for pos in CONSTRUCTIBLES:
        debug("  -", end = ' ')
        afficher_position(pos)
    debug()
    
    for i in range(len(TROUS)-1):
        a = TROUS[i]
        b = TROUS[i+1]
        x1, y1, z1 = a
        x2, y2, z2 = b
        tunnel = chemin_oiseau((x1, y1, -1), (x2, y2, -1))
        A_CREUSER.extend(tunnel)

    display_map()




# Fonction appelée à chaque tour.
def jouer_tour():

    # Creuse un bloc
    error = 1
    while error != erreur.OK and len(A_CREUSER) > 0:
        error = creuser_tunnel(A_CREUSER.pop())

    # Mise a jour de la liste des nids encore libres
    global NIDS_LIBRES
    temp = []
    for nid in NIDS_LIBRES:
        if info_nid(nid) == etat_nid.LIBRE:
            temp.append(nid)
        #elif info_nid(nid) == moi() + 1:
        #    MES_NIDS.append(nid)
    NIDS_LIBRES = temp

    debug("Mes nids :")
    for nid in MES_NIDS:
        debug("  -", end = ' ')
        afficher_position(nid)


    # Cible en priorite les nids libres a proximite
    for i in range(2):
        while True:
            cibles = []
            troupe = troupes_joueur(moi())[i]
            for nid in NIDS_LIBRES:
                chemin = trouver_chemin_arr(troupe.maman, nid)
                if len(chemin) > 0:
                    cibles.append((chemin, nid))
        
            if len(cibles) > 0:
                cibles.sort(key = lambda p: len(p[0]))
                chemin, nid = cibles[0]
                debug(f"Troupe {troupe.id} cible le nid", end = ' ')
                afficher_position(nid)
                debug(f"Chemin choisi :", end = ' ')
                debug(chemin)
                debug()
                distance = min(troupe.pts_action, len(chemin))
                for j in range(distance):
                    avancer(troupe.id, chemin[j])
                if distance >= troupe.pts_action:
                    break
                else:
                    NIDS_LIBRES.remove(nid)
                    MES_NIDS.append(nid)
            else:
                break
    

    # Tente de grandir si l'inventaire est pres d'etre rempli
    for troupe in troupes_joueur(moi()):
        if inventaire(troupe.taille) < troupe.inventaire + 1:
            grandir(troupe.id)
    
    forbidden = [] # Liste des cibles qui se revelent etre fatales
    # Cible la miche de pain / le nid le plus proche si l'inventaire est non vide
    for i in range(2):
        while True:
            troupe = troupes_joueur(moi())[i]
            smallest = float("inf")
            cible = []
            arrivee = (0,0,0)
            for papy in PAPYS:
                if papy in forbidden:
                    continue
                chemin = trouver_chemin_arr(troupe.maman, papy)
                if len(chemin) <= 0:
                    continue
                if len(chemin) < smallest:
                    smallest = len(chemin)
                    cible = chemin
                    arrivee = papy
            if troupe.inventaire > 0:
                for nid in MES_NIDS:
                    if nid in forbidden:
                        continue
                    chemin = trouver_chemin_arr(troupe.maman, nid)
                    if len(chemin) <= 0:
                        continue
                    if len(chemin) < smallest:
                        smallest = len(chemin)
                        cible = chemin
                        arrivee = nid
            if len(cible) == 0:
                # Aucune destination n'est atteignable
                break
            debug("----")
            debug("Cible", end = ' ')
            afficher_position(arrivee)
            debug("Chemin", end = ' : ')
            debug(cible)
            distance = min(troupe.pts_action, len(cible))
            for j in range(distance):
                avancer(troupe.id, cible[j])

            # Verifie que l'action n'est pas letale
            for nid in MES_NIDS:
                if trouver_chemin_arr(troupes_joueur(moi())[i].maman, nid) != []:
                    debug("Destination safe, peut atteindre", end = ' ')
                    afficher_position(nid)
                    break
            else:
                for j in range(distance):
                    annuler()
                forbidden.append(arrivee)
                debug("Annule car trop dangereux")
                continue
            if distance >= troupe.pts_action:
                break

    for i in range(2):
        troupe = troupes_joueur(moi())[i]
        while troupe.pts_action > 0:
            debug("Points restants")
            for dir in DIRECTIONS:
                if can_move(troupe.maman, dir):
                    avancer(troupe.id, dir)
                    debug("Complete vers", end = ' ')
                    afficher_direction(dir)
                    break
            else:
                debug("Mort inevitable :(")
                break
            troupe = troupes_joueur(moi())[i]


    # Essaie de tuer les troupes adverses
    for troupe in troupes_joueur(adversaire()):
        x, y, z = troupe.maman
        cnt = 0
        cancel = False
        for dir in DIRECTIONS:
            if can_move(troupe.maman, dir):
                dx, dy, dz = delta_pos(dir)
                ax, ay, az = x + dx, y + dy, z + dz
                err = construire_buisson((ax, ay, az))
                if err != erreur.OK:
                    cancel = True
                    break
                else:
                    cnt += 1
        if cancel:
            for _ in range(cnt):
                annuler()
    display_map()


# Fonction appelée à la fin de la partie.
def partie_fin():
    # TODO
    pass
