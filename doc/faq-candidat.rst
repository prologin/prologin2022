.. SPDX-License-Identifier: GPL-2.0-or-later
   Copyright 2019 Thibault Allançon
   Copyright 2022 Association Prologin <info@prologin.org>

=========================================
FAQ TODO(titre du sujet)
=========================================

**Quelques conseils avant de poser une question**

1. Lire le sujet
2. Relire le sujet
3. Lire votre code
4. Relire votre code
5. Contacter un orga


Sujet
=====

TODO

GUI
===

TODO

**Appuyez sur H avant de poser une question !**

Erreurs fréquentes
==================

TODO(check que les trucs en dessous sont toujours valides)

- Ne pas appeler les fonctions de l'API avant l'appel à ``partie_init``.
- **Ne pas modifier** les fichiers générés (``api.*``, ``interface.*``, ...)
    - Attention, ça ne fera pas d'erreurs en local mais cela ne marchera pas sur
      le serveur (donc pour les tournois).
- Si vous faites des fichiers annexes, n'oubliez pas de les mettre en plus dans
  le makefile après ``prologin.*`` dans ``champion-srcs`` (ou ``champion-dists``
  pour Python).
