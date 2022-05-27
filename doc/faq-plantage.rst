.. SPDX-License-Identifier: GPL-2.0-or-later
   Copyright 2022 Association Prologin <info@prologin.org>

FAQ
=====================


..
   Debugger son champion avec valgrind
   ===================================

   Dans un terminal, la commande ``stechec2-run -h`` vous donnera déjà
   quelques indications concises, en voici des plus détaillées :

   L’idée est de dire à stechec2-run de lancer l’un des deux champions du
   match dans valgrind, il faut pour cela utiliser l’option
   ``--debug-cmd "valgrind"`` qui demande à stechec2 d’utiliser valgrind,
   et l’option ``-d 1`` ou ``-d 2`` pour lui demander de debugger le
   champion numéro 1 ou numéro 2.

   Vos commandes ressembleront typiquement à : \* pour vérifier vos invalid
   read / invalid write et autre erreur grave de gestion de mémoire :

   ::

      stechec2-run config.yml --debug-cmd "valgrind" -d 1

   -  pour vérifier aussi vos fuites de mémoire :

   ::

      stechec2-run config.yml --debug-cmd "valgrind --leak-check=full" -d 1

   Debugger son champion avec GDB
   ==============================

   Avant toute chose, n’oubliez pas d’ajouter dans votre fichier de
   configuration de match l’option ``time: -1``, sinon stechec2 attendra de
   votre programme qu’il envoie ses actions en moins d’une seconde, même si
   vous êtes en train d’explorer des choses dans GDB.

   Dans un terminal, la commande stechec2-run -h vous donnera déjà quelques
   indications concises, en voici des plus détaillées :

   Vous pouvez utiliser les mêmes instructions que pour valgrind, mais avec
   une autre commande de debug, c’est à dire typiquement
   ``--debug-cmd "gdb --args"``. Notez que c’est sa valeur par défaut, vous
   pouvez donc en fait directement lancer votre match comme ceci pour
   lancer le premier champion dans gdb :

   ::

      stechec2-run -d 1 config.yml

   Le problème avec cette méthode est que les autres processus peuvent
   polluer votre terminal (notamment la gui), ce n’est pas toujours gênant,
   mais si ça l’est dans votre cas, il faut ruser. Nous allons lancer le
   client que l’on souhaite debugger dans son propre terminal.

   -  pour commencer, lancer stechec2-run en lui disant de ne pas démarrer
      le champion numéro 1 :

   ::

      stechec2-run --inhibit 1 config.yml

   -  le match va se préparer, le champion numéro 2 et l’éventuelle gui
      seront lancés, mais pas le champion, nous le lancerons à la main dans
      un nouveau terminal. Notez la partie
      “``>>> Not starting Blue ones[P=1]:``” qui vous indique que le
      premier champion n’a pas été lancé, et surtout vous donne la commande
      à utiliser pour le lancer vous-même ! (C’est une longue commande, sur
      mon système avec un jeu de test, la commande ne question est
      “``stechec2-client --req_addr ipc:///tmp/stechec2-025q0z0f/reqrep --sub_addr ipc:///tmp/stechec2-025q0z0f/pubsub --rules libprologin2022.so --verbose 3 --time -1 --champion /home/joseph/champion/c/champion.so --client_id 1 --name Blue ones``”)
   -  vous pouvez lancer un nouveau terminal et lancer ``gdb --args`` suivi
      de la commande donnée par stechec2-run, ce qui lancera effectivement
      votre champion dans gdb avec les bons arguments :

   ::

      gdb --args stechec2-client --req_addr ipc:///tmp/stechec2-025q0z0f/reqrep --sub_addr ipc:///tmp/stechec2-025q0z0f/pubsub --rules libprologin2022.so --verbose 3 --time -1 --champion /home/joseph/champion/c/champion.so --client_id 1 --name Blue ones
