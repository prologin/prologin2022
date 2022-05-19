const WINDOW_HEIGHT = 960;
const WINDOW_WIDTH = 960;

const DEPTH = 2;
const MAP_SIZE = 20;

const SPRITE_WIDTH = WINDOW_WIDTH / MAP_SIZE;
const SPRITE_HEIGHT = WINDOW_HEIGHT / MAP_SIZE;

const PigeonColor = {
    BLUE: 0x9999FF,
    YELLOW: 0xFFFF00,
    RED: 0xFF4444,
    NONE: null,
};

const Direction = {
    NORD: 0,
    SUD: 1,
    EST: 2,
    OUEST: 3,
    HAUT: 4,
    BAS: 5,
}

function generateMatrix() {
    return Array(MAP_SIZE).fill(Array(MAP_SIZE).fill(Array(DEPTH)));
}

export class Game {


    constructor() {
        this.app = new PIXI.Application({width: WINDOW_WIDTH, height: WINDOW_HEIGHT});
        this.units = generateMatrix();
        this.map = generateMatrix();
        this.textures = {
            old: [new PIXI.Texture.from("assets/chars/old/old_1.png"),
                new PIXI.Texture.from("assets/chars/old/old_2.png"),
                new PIXI.Texture.from("assets/chars/old/old_3.png"),
                new PIXI.Texture.from("assets/chars/old/old_4.png"),
                new PIXI.Texture.from("assets/chars/old/old_1.png"),],
            grass: [new PIXI.Texture.from("assets/environment/grass/grass_1.png"),
                new PIXI.Texture.from("assets/environment/grass/grass_2.png"),
                new PIXI.Texture.from("assets/environment/grass/grass_3.png")],
            pigeon: new PIXI.Texture.from("assets/chars/pigeon/pigeon.png"),
            dirt: new PIXI.Texture.from("assets/environment/dirt/dirt.png"),
        }
    }

    createSprite(texture, x, y) {
        let sprite = new PIXI.Sprite(texture);
        sprite.x = y * SPRITE_WIDTH;
        sprite.y = x * SPRITE_HEIGHT;
        sprite.width = SPRITE_WIDTH;
        sprite.height = SPRITE_HEIGHT;
        return sprite;
    }

    readMap(mapString) {
        // Generates default values.
        for (let i = 0; i < MAP_SIZE; i++) {
            for (let j = 0; j < MAP_SIZE; j++) {
                let random = Math.floor(Math.random() * 3);
                this.map[i][j][0] = this.createSprite(this.textures.grass[random], i, j);
                this.map[i][j][1] = this.createSprite(this.textures.dirt, i, j);
                this.app.stage.addChild(this.map[i][j][0]);
            }
        }
    }

    }

    info_fermer_barriere() {
    }

    info_division() {
    }

    setupDocument(document) {
        return document.body.appendChild(this.app.view);
    }

}
