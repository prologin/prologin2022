import {Action} from "./action"

class ActionConstruireBuisson extends Action {
    constructor(joueur_id, troupe_id) {
        this.joueur_id = joueur_id;
        this.troupe_id = troupe_id;
        this.col = col;
        this.ligne = ligne;
    }

    execute(game) {

    }
}
