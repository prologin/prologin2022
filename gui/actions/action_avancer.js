import {Action} from "./action"

class ActionAvancer extends Action {
    constructor(joueur_id, troupe_id, direction) {
        this.joueur_id = joueur_id;
        this.troupe_id = troupe_id;
        this.direction = direction;
    }

    execute(game) {

    }
}
