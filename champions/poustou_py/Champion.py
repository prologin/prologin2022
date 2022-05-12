from api import *

def display_map():
    case_str = {
            type_case.GAZON: '',
            type_case.BUISSON: '█', 
            type_case.BARRIERE: '╳',
            type_case.NID: 'O',
            type_case.PAPY: 'P',
            type_case.TROU: '░',
            type_case.TUNNEL: ' ',
            type_case.TERRE: '█'
    }
    pains_str = [' ', '.', ':', '…']

    parc = [[['  '] * LARGEUR for _ in range(HAUTEUR)] for _ in range(2)]

    for z in range(-1, 1):
        for y in range(HAUTEUR):
            for x in range(LARGEUR):
                pos = x, y, z
                case = info_case(pos)
                pains = pains_str[case.nb_pains]
                disp = case_str[case.contenu] + pains
                parc[z][y][x] = disp
    
    for niveau in parc:
        for ligne in niveau:
            for case in ligne:
                print(case, end = '')
            print()
        print('-----------')

def nearest(troupe_id, condition):
    """
    Cherche un chemin de la troupe jusqu'à une case respectant une
    certaine condition
    Retourne la position ainsi que la liste des directions à prendre
    """
    
    visite = [[[False] * LARGEUR for _ in range(HAUTEUR)] for _ in range(2)]
    for z in range(-1, 1):
        for y in range(HAUTEUR):
            for x in range(LARGEUR):
                if info_case((x, y, z)).contenu in [type_case.BUISSON, type_case.BARRIERE, type_case.TERRE]:
                    visite[z][y][x] = True

    def traversable(x, y, z):
        if not (0 <= x < LARGEUR and 0 <= y < HAUTEUR and -1 <= z <= 0):
            return False
        return visite[z][y][x]

    troupe = troupes_joueur(moi())[troupe_id]
    start = troupe.maman
    sx, sy, sz = start
    file = [(start, [])]
    deplacements =
        {
            direction.NORD : (0, 1, 0),
            direction.SUD  : (0, -1, 0),
            direction.EST  : (1, 0, 0),
            direction.OUEST: (-1, 0, 0),
            #direction.HAUT : (0, 0, 1),
            #direction.BAS  : (0, 0, -1)
        }
    while len(file) > 0:
        position, chemin = file.pop(0)
        if condition(position):
            return position, chemin
        x, y, z = position
        visite[z][y][x] = True
        for direction, (dx, dy, dz) in deplacements.items():
            rx, ry, rz = x + dx, y + dy, z + dz
            if not traversable(rx, ry, rz):
                continue
            file.append((rx, ry, rz), chemin.copy() + [direction])
    
    return None




target = [None, None]
# Fonction appelée au début de la partie.
def partie_init():
    for troupe_id in range(2):
        target[troupe_id] = nearest(troupe_id, lambda position: info_case(position).contenu == type_case.NID)



# Fonction appelée à chaque tour.
def jouer_tour():
    for troupe_id in range(2):
        troupe = troupes_joueur(moi())[troupe_id]
        arrivee, chemin = target[troupe_id]
        while troupe.pts_action > 0:
            avancer(troupe_id, chemin.pop(0))
            if len(chemin) == 0:
                if info_case(arrivee).contenu == type_case.NID:
                    target[troupe_id] = nearest(troupe_id, lambda position: info_case(position).nb_pains > 0)
                else:
                    target[troupe_id] = nearest(troupe_id, lambda position: info_case(position).contenu == type_case.NID and info_nid(position) == moi() + 1)


# Fonction appelée à la fin de la partie.
def partie_fin():
    # TODO
    pass
