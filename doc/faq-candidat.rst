.. SPDX-License-Identifier: GPL-2.0-or-later
   Copyright 2019 Thibault Allançon
   Copyright 2022 Association Prologin <info@prologin.org>

=========================================
FAQ La baguette légendaire
=========================================

**Quelques conseils avant de poser une question**

1. Lire le sujet
2. Relire le sujet
3. Lire votre code
4. Relire votre code
5. Contacter un orga

Sujet
=====

Comment obtenir les points de spawn ?
-------------------------------------

Il n'y a aucune fonction de l'api permettant d'obtenir la position des points
de spawn. Cependant, il y a plusieurs moyens de les récupérer. L'un d'eux est
de regarder la position des mamans canard dans la fonction ``partie_init``
avec un appel à la fonction ``troupes_joueur``.

Bug/Question/Technique
=====

Les questions, rapports de bug et problèmes techniques peuvent être posés aux
organisateurs principalement sur Redmine via l'ouverture d'un ticket.

`Lien du Redmine <https://redmine.finale.prologin.org>`_

Erreurs fréquentes
==================

- Ne pas appeler les fonctions de l'API avant l'appel à ``partie_init``.
- **Ne pas modifier** les fichiers générés (``api.*``, ``interface.*``, ...)
    - Attention, ça ne fera pas d'erreurs en local mais cela ne marchera pas sur
      le serveur (donc pour les tournois).
- Si vous faites des fichiers annexes, n'oubliez pas de les mettre en plus dans
  le Makefile après ``champion.*`` dans ``CHAMPIONS_FILES``

Plantage avec stechec
---------------------

1. Lire le message d’erreur de stechec
2. Vérifier que les chemins de vos sources sont les bons dans le
   Makefile et surtout config.yml.
3. \*\* Tout appel à une fonction de l’API avant partie_init() fera une
   segfault. \*\* Par exemple en C/C++, faire int moi = moi(); en
   variable globale entraînera un segfault.
4. En **python**, votre prologin.py doit être dans le dossier dans
   lequel vous lancez stechec.
5. Appelez un.e organisateurice. Ne soyez pas timide, on est aussi là
   pour vous aider. :-)

Plantage avec le serveur de finale (et/ou votre champion fait n’importe quoi sur le serveur)
--------------------------------------------------------------------------------------------

1. Vérifiez, quand vous faîtes ``make tar``, que vos fichiers sont bien
   inclus dans le bidule.tar.
2. Ne modifiez pas les fichiers de l’api (api.*, prologin.hh,
   prologin.h, interface.*)
3. Vérifiez qu’il n’y a pas d’ouverture de fichiers dans votre code.
4. Vérifiez que dans votre Makefile, vos fichiers à vous sont bien
   inclus au bon endroit.
5. Appelez un.e organisateurice. Ne soyez pas timide, on est aussi là
   pour vous aider. :-)

Créer ses propres maps
======================

Il est possible de créer vos propre maps en utilisant l'éditeur de map qui vous est fourni.
Celui-ci est accessible via la command ``map_editor``. Vous pouvez utiliser les maps ainsi
créées pour tester vos champions ou pour les publier sur https://finale.prologin.org/maps/new/.
