import {PigeonColor} from "../pigeon"
import {Action} from "./action"

class PigeonDebug extends Action {
    constructor(col, ligne, niveau, pigeon) {
        this.col = col;
        this.ligne = ligne;
        this.niveau = niveau;
        switch (pigeon) {
            case 'BLUE':
            this.pigeon = PigeonColor.BLUE;
            break;
            case 'YELLOW':
            this.pigeon = PigeonColor.YELLOW;
            break;
            case 'RED':
            this.pigeon = PigeonColor.RED;
            break;
            case 'NONE':
            this.pigeon = PigeonColor.NONE;
            break;
        }
    }

    execute(game) {

    }
}
