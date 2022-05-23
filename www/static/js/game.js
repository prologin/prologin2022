const WINDOW_HEIGHT = 960;
const WINDOW_WIDTH = 960;

const DEPTH = 2;
const MAP_SIZE = 40;

const SPRITE_WIDTH = WINDOW_WIDTH / MAP_SIZE;
const SPRITE_HEIGHT = WINDOW_HEIGHT / MAP_SIZE;

const Direction = {
    NORD: 0,
    SUD: 1,
    EST: 2,
    OUEST: 3,
    HAUT: 4,
    BAS: 5,
}

const ASSET_ROOT = "/static/img/gui"

function generateMatrix() {
    return Array(MAP_SIZE).fill(Array(MAP_SIZE).fill(Array(DEPTH)));
}

class Game {
    constructor() {
        this.app = new PIXI.Application({width: WINDOW_WIDTH, height: WINDOW_HEIGHT});
        this.units = generateMatrix();
        this.map = generateMatrix();
        this.textures = {
            grass: [new PIXI.Texture.from(`${ASSET_ROOT}/grass/grass_1.png`),
                new PIXI.Texture.from(`${ASSET_ROOT}/grass/grass_2.png`),
                new PIXI.Texture.from(`${ASSET_ROOT}/grass/grass_3.png`)],
            old: [new PIXI.Texture.from(`${ASSET_ROOT}/old/old_1.png`),
                new PIXI.Texture.from(`${ASSET_ROOT}/old/old_2.png`),
                new PIXI.Texture.from(`${ASSET_ROOT}/old/old_3.png`),
                new PIXI.Texture.from(`${ASSET_ROOT}/old/old_4.png`),
                new PIXI.Texture.from(`${ASSET_ROOT}/old/old_1.png`),],
            pigeon: new PIXI.Texture.from(`${ASSET_ROOT}/pigeon/pigeon.png`),
            dirt: new PIXI.Texture.from(`${ASSET_ROOT}/dirt/dirt.png`),
            spawn: new PIXI.Texture.from(`${ASSET_ROOT}/spawn.png`),
            nid: new PIXI.Texture.from(`${ASSET_ROOT}/nid.png`),
            buisson: new PIXI.Texture.from(`${ASSET_ROOT}/buisson.png`),
            B: new PIXI.Texture.from(`${ASSET_ROOT}/barrier/barrier_H.png`),
            b: new PIXI.Texture.from(`${ASSET_ROOT}/b.png`),
            trou: new PIXI.Texture.from(`${ASSET_ROOT}/trou.png`),
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

    readMap(mapString, texture_map) {
        for (let i = 0; i < MAP_SIZE; i++) {
            for (let j = 0; j < MAP_SIZE; j++) {
                const char_index = i * MAP_SIZE + j;
                this.map[i][j][0] = this.createSprite(texture_map(mapString.charAt(char_index), this.textures), i, j);
                this.map[i][j][1] = this.createSprite(this.textures.dirt, i, j);
                this.app.stage.addChild(this.map[i][j][0]);
            }
        }
    }

    pigeon_debug(color, x, y, z) {
        if (color === PigeonColor.NONE) {
            this.app.stage.removeChild(this.units[x][y][z]);
            this.units[x][y][z] = null;
            return;
        }
        let pigeon = new PIXI.Sprite(this.textures.pigeon);
        pigeon.x = x;
        pigeon.y = y;
        pigeon.width = SPRITE_WIDTH;
        pigeon.height = SPRITE_HEIGHT;
        pigeon.tint = color;
        this.units[x][y] = pigeon;
        this.app.stage.addChild(pigeon);
    }

    createOldMan(x, y) {
        let old = new PIXI.AnimatedSprite(this.textures.old, true);
        old.animationSpeed = 0.1;
        old.loop = false;
        old.x = x * SPRITE_WIDTH;
        old.y = y * SPRITE_WIDTH;
        old.width = SPRITE_WIDTH;
        old.height = SPRITE_HEIGHT;
        this.app.stage.addChild(old);
        this.units[x][y] = old;
    }

    async setupAnimation() {
        this.createOldMan(10, 10);
    }

    addToDOM(viewParent) {
        return viewParent.appendChild(this.app.view);
    }

}

function create_game() {
    return new Game();
}

function map_char_to_texture(input_char, textures) {
    switch (input_char) {
        case ' ':
        case '.':
            return textures.grass[Math.floor(Math.random() * 3)];
        case 'S':
            return textures.spawn;
        case 'N':
            return textures.nid;
        case '#':
            return textures.buisson;
        case 'B':
            return textures.B;
        case 'b':
            return textures.b;
        case '0':
        case '1':
        case '2':
        case '3':
        case '4':
        case '5':
        case '6':
        case '7':
        case '8':
        case '9':
            return textures.papi[0];
        case 'X':
            return textures.trou;

    }
}

function map_enum_to_texture(input_char, textures) {
    switch (input_char) {
        case '0':
        case '7':
        case '8':
            return textures.grass[Math.floor(Math.random() * 3)];
        case 'S':
            return textures.spawn;
        case '3':
            return textures.nid;
        case '1':
            return textures.buisson;
        case '2':
            return textures.B;
        case '5':
            return textures.trou;
    }
}
