from api import *


DEBUG = 1
def debug(*args, **kwargs):
	if DEBUG is False:
		return
	if DEBUG is True or DEBUG == moi():
		print(*args, **kwargs)

def afficher_position(position, end = '\n'):
	debug(f"({position[0]}; {position[1]}; {position[2]})", end = end)

def afficher_direction(dir, end = '\n'):
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


    
    for niveau in parc[0:1]:
        for ligne in niveau:
            for case in ligne:
                debug(case, end = '')
            debug()


def trouver_chemin_arr(pos1, pos2):
	if not case_traversable(pos2):
		return []
	return trouver_chemin(pos1, pos2)

def case_traversable(pos):
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

SPAWN_POINTS = []
SPAWN_POINT = {}
NIDS_LIBRES = []
PAPYS = []
TROUS = []
CONSTRUCTIBLES = []
MES_NIDS = []


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




# Fonction appelée à chaque tour.
def jouer_tour():

	display_map()


	# Mise a jour de la liste des nids encore libres
	global NIDS_LIBRES
	temp = []
	for nid in NIDS_LIBRES:
		if info_nid(nid) == etat_nid.LIBRE:
			temp.append(nid)
		#elif info_nid(nid) == moi() + 1:
		#	MES_NIDS.append(nid)
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
	
	forbidden = []
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
					debug("Non lethal, peut atteindre", end = ' ')
					afficher_position(nid)
					break
			else:
				for j in range(distance):
					annuler()
				forbidden.append(arrivee)
				debug("Annule pour lethalite")
				continue
			if distance >= troupe.pts_action:
				break
			

			








		




# Fonction appelée à la fin de la partie.
def partie_fin():
    # TODO
    pass
