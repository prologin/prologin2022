from api import *
from collections import defaultdict
from sys import exit
from queue import Queue

visitedBase = [[[False] * LARGEUR for _ in range(HAUTEUR)] for _ in range(2)]

def dprint(*args, **kwargs):
    if moi() == 0 and False:
        print(*args, **kwargs)

def get_troupe_by_id(troupe_id):
    for troupe in troupes_joueur(moi()):
        if troupe.id == troupe_id:
            return troupe

def display_map():
    def display_case(case):
        contenu = case.contenu
        if contenu == type_case.GAZON:
            return ' '
        if contenu == type_case.BUISSON:
            return '█'
        if contenu == type_case.BARRIERE:
            if info_barriere(case.pos) == etat_barriere.OUVERTE:
                return ' '
            else:
                return '█'
        if contenu == type_case.NID:
            if info_nid(case.pos) == etat_nid.LIBRE:
                return 'O'
            if info_nid(case.pos) == moi() + 1:
                return 'O'
            return 'O'
        if contenu == type_case.PAPY:
            return str(papy_tours_restants(case.pos))
        if contenu == type_case.TROU:
            return '╳'
        if contenu == type_case.TUNNEL:
            return ' '
        return '█'

    pains_str = [' ', '.', ':', '…'] + [str(x % 10) for x in range(4, 50)]

    parc = [[[' '] * LARGEUR for _ in range(HAUTEUR)] for _ in range(2)]

    for z in range(-1, 1):
        for y in range(HAUTEUR):
            for x in range(LARGEUR):
                pos = x, y, z
                case = info_case(pos)
                parc[z][y][x] = display_case(case)

    for pos in pains():
        x, y, z = pos
        act = parc[z][y][x]
        if not act in pains_str:
            parc[z][y][x] = pains_str[1]
        else:
            act = pains_str.index(act)
            parc[z][y][x] = pains_str[act+1]

    for troupe in troupes_joueur(moi()):
        for x, y, z in troupe.canards:
            parc[z][y][x] = 'm'
            if troupe.maman == (x, y, z):
                parc[z][y][x] = 'M'

    for troupe in troupes_joueur(adversaire()):
        for x, y, z in troupe.canards:
            parc[z][y][x] = 'A'


    
    for niveau in parc[0:1]:
        for ligne in niveau:
            for case in ligne:
                print(case, end = '')
            print()
        print('-----------')

def traversable(pos):
    x, y, z = pos
    if not (0 <= x < LARGEUR and 0 <= y < HAUTEUR and -1 <= z <= 0):
        return False
    case = info_case(pos)
    contenu = case.contenu
    x, y, z = case.pos
    if contenu == type_case.BUISSON:
        return False
    if contenu == type_case.BARRIERE and info_barriere(case.pos) == etat_barriere.FERMEE:
        return False
    if z == 0 and (x == 0 or x == LARGEUR-1 or y == 0 or y == HAUTEUR-1):
        return False
    for troupe in troupes_joueur(moi()):
        for pos in troupe.canards:
            if pos == case.pos:
                return False
    for troupe in troupes_joueur(adversaire()):
        for pos in troupe.canards:
            if pos == case.pos:
                return False
    return True



def aller_retour(troupe_id, arrivee):
    arrivee, chemin = nearest(troupe_id, lambda pos: pos == arrivee)
    
    visite = [[[False] * LARGEUR for _ in range(HAUTEUR)] for _ in range(2)]
    for z in range(-1, 1):
        for y in range(HAUTEUR):
            for x in range(LARGEUR):
                pos = x, y, z
                case = info_case(pos)
                if not traversable(case):
                    visite[z][y][x] = True

    
def visitedMap():
    visited = []
    for z in range(-1, 1):
        niveau = []
        for y in range(HAUTEUR):
            niveau.append(visitedBase[z][y][:])
        visited.append(niveau)
    for troupe in troupes_joueur(moi()):
        for pos in troupe.canards:
            x, y, z = pos
            visited[z][y][x] = True
    for troupe in troupes_joueur(adversaire()):
        for pos in troupe.canards:
            x, y, z = pos
            visited[z][y][x] = True
    return visited

        
def inv(dir):
    if dir == direction.NORD:
        return direction.SUD
    if dir == direction.SUD:
        return direction.NORD
    if dir == direction.EST:
        return direction.OUEST
    if dir == direction.OUEST:
        return direction.EST
    if dir == direction.HAUT:
        return direction.BAS
    if dir == direction.BAS:
        return direction.HAUT


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
               pos = x, y, z
               if not traversable(pos):
                   visite[z][y][x] = True

    for troupe in troupes_joueur(moi()):
        for pos in troupe.canards:
            x, y, z = pos
            visite[z][y][x] = True

    for troupe in troupes_joueur(adversaire()):
        for pos in troupe.canards:
            x, y, z = pos
            visite[z][y][x] = True

    def traversable_(pos):
        x, y, z = pos
        if not (0 <= x < LARGEUR and 0 <= y < HAUTEUR and -1 <= z <= 0):
            return False
        return not visite[z][y][x]

    from_dir = [[[None] * LARGEUR for _ in range(HAUTEUR)] for _ in range(2)]

    troupe = get_troupe_by_id(troupe_id)
    start = troupe.maman
    sx, sy, sz = start
    visite[sz][sy][sx] = False
    file = Queue()
    file.put(start)
    deplacements = { 
            direction.NORD : (0, 1, 0),
            direction.SUD  : (0, -1, 0),
            direction.EST  : (1, 0, 0),
            direction.OUEST: (-1, 0, 0),
            #direction.HAUT : (0, 0, 1),
            #direction.BAS  : (0, 0, -1)
        }
    flag = False
    while not file.empty():
        #print(file.qsize())
        position = file.get()
        if condition(position):
            #print("End nearest, found")
            flag = True
            break
        x, y, z = position
        if not traversable_(position):
            continue
        visite[z][y][x] = True 
        for dir, (dx, dy, dz) in deplacements.items():
            rx, ry, rz = x + dx, y + dy, z + dz
        #for dir, (rx, ry, rz) in canmove((x, y, z), condition = lambda pos: traversable_(pos)):
            if traversable_((rx, ry, rz)):
                from_dir[rz][ry][rx] = inv(dir)
                file.put((rx, ry, rz))
    
    if file.empty() and not flag:
        #print("Not found")
        return None, None
    
    chemin = []
    arrival = position
    while position != start:
        x, y, z = position
        dir = from_dir[z][y][x]
        #print(position, dir)
        chemin.append(inv(dir))
        dx, dy, dz = deplacements[dir]
        position = x + dx, y + dy, z + dz
    #print(arrival, chemin)
    return arrival, chemin

def plus_pains():
    nb_pains = defaultdict(lambda: 0)
    best = (LARGEUR // 2, HAUTEUR // 2, 0)
    score = 0
    for position in pains():
        nb_pains[position] += 1
        if nb_pains[position] > score:
            score = nb_pains[position]
            best = position
    return best, score

def beaucoup_pains(pos):
    #best, score = plus_pains()
    return info_case(pos).nb_pains > 0 and traversable(pos)




ids = []
target = [None, None, None]
# Fonction appelée au début de la partie.
def partie_init():
    for z in range(-1, 1):
        for y in range(HAUTEUR):
            for x in range(LARGEUR):
                if not traversable((x, y, z)):
                    visitedBase[z][y][x] = True
    for troupe in troupes_joueur(moi()):
        ids.append(troupe.id)
    for troupe_id in ids:
        target[troupe_id] = nearest(troupe_id, lambda position: info_case(position).contenu == type_case.NID)

    



def mon_nid(position):
    return info_case(position).contenu == type_case.NID and info_nid(position) == moi() + 1 and traversable(position)

def nid_libre(position):
    return info_case(position).contenu == type_case.NID and info_nid(position) == etat_nid.LIBRE

def step_to(troupe, condition):
    target, path = nearest(troupe.id, condition)
    if target is None:
        return False
    else:
        return avancer(troupe.id, path.pop()) == erreur.OK

def canmove(pos, condition = lambda pos: traversable(pos)):
    x, y, z = pos
    deplacements = { 
            direction.NORD : (0, 1, 0),
            direction.SUD  : (0, -1, 0),
            direction.EST  : (1, 0, 0),
            direction.OUEST: (-1, 0, 0),
            #direction.HAUT : (0, 0, 1),
            #direction.BAS  : (0, 0, -1)
        }
    result = []
    for dir, (dx, dy, dz) in deplacements.items():
        rx, ry, rz = x + dx, y + dy, z + dz
        if condition((rx, ry, rz)):
            result.append((dir, (rx, ry, rz)))
    return result


    

# Fonction appelée à chaque tour.
def jouer_tour():
    dprint("jouer tour")
    if moi() == 0:
        display_map()
    for troupe_id in ids:
        dprint(troupe_id, "start")
        troupe = get_troupe_by_id(troupe_id)
        while troupe.pts_action > 0:
            #display_map()
            #print(troupe.pts_action, "pts remaining")            
            if troupe.inventaire > 0:
                #print(troupe.inventaire, "pains in inventory, trying to get to our nest")
                if not step_to(troupe, lambda pos: mon_nid(pos)):
                    #print("Can't get to my nest ! Trying to reach a free nest")
                    while troupe.pts_action > 0:
                        if not step_to(troupe, lambda pos: nid_libre(pos)):
                            #print("Can't reach it ! Trying to grow")
                            if grandir(troupe_id) != erreur.OK:
                                #print("Can't grow ! Trying to move")
                                dirs = canmove(troupe.maman)
                                if len(dirs) == 0:
                                    #print("Can't move ! *dies*")
                                    break
                                avancer(troupe_id, dirs[0][0])
                        troupe = get_troupe_by_id(troupe_id)
            else:
                #print("No pain in inventory. Searching for some")
                if not step_to(troupe, lambda pos:beaucoup_pains(pos)):
                    #print("Can't get to bread. Trying to grow")
                    if grandir(troupe_id) != erreur.OK:
                        #print("Can't grow. Trying to move")
                        dirs = canmove(troupe.maman)
                        if len(dirs) == 0:
                            #print("Can't move. *dies*")
                            break
                        avancer(troupe_id, dirs[0][0])


            troupe = get_troupe_by_id(troupe_id)
        


# Fonction appelée à la fin de la partie.
def partie_fin():
    # TODO
    pass
