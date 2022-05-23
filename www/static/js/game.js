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
            nest: new PIXI.Texture.from(`${ASSET_ROOT}/nid.png`),
            bush: new PIXI.Texture.from(`${ASSET_ROOT}/buisson.png`),
            barrier_open: new PIXI.Texture.from(`${ASSET_ROOT}/barrier/barrier_H.png`),
            barrier_closed: new PIXI.Texture.from(`${ASSET_ROOT}/b.png`),
            hole: new PIXI.Texture.from(`${ASSET_ROOT}/trou.png`),
        }
    }


    readMap(mapString, sprite_map) {
        // Default map with texturing to allow for transparency.
        for (let i = 0; i < MAP_SIZE; i++) {
            for (let j = 0; j < MAP_SIZE; j++) {
                this.map[i][j][0] = createSprite(this.textures.grass[Math.floor(Math.random() * 3)], i, j);
                this.map[i][j][1] = createSprite(this.textures.dirt, i, j);
                this.app.stage.addChild(this.map[i][j][0]);
            }
        }

        for (let i = 0; i < MAP_SIZE; i++) {
            for (let j = 0; j < MAP_SIZE; j++) {
                const char_index = i * MAP_SIZE + j;
                this.map[i][j][0] = sprite_map(mapString.charAt(char_index), this.textures, i, j);
                this.app.stage.addChild(this.map[i][j][0]);
            }
        }
    }


    addToDOM(viewParent) {
        return viewParent.appendChild(this.app.view);
    }

}

function create_game() {
    return new Game();
}

function createAnimatedSprite(texture, x, y) {
    let aSprite = new PIXI.AnimatedSprite(texture, true);
    aSprite.animationSpeed = 0.1;
    aSprite.loop = false;
    aSprite.x = x * SPRITE_WIDTH;
    aSprite.y = y * SPRITE_WIDTH;
    aSprite.width = SPRITE_WIDTH;
    aSprite.height = SPRITE_HEIGHT;
    return aSprite;
}

function createSprite(texture, x, y) {
    let sprite = new PIXI.Sprite(texture);
    sprite.x = y * SPRITE_WIDTH;
    sprite.y = x * SPRITE_HEIGHT;
    sprite.width = SPRITE_WIDTH;
    sprite.height = SPRITE_HEIGHT;
    return sprite;
}

function map_char_to_sprite(input_char, textures, i, j) {
    let texture = textures.grass[0];
    switch (input_char) {
        case ' ':
        case '.':
            texture = textures.grass[Math.floor(Math.random() * 3)];
            break;
        case 'S':
            texture = textures.spawn;
            break;
        case 'N':
            texture = textures.nest;
            break;
        case '#':
            texture = textures.bush;
            break;
        case 'B':
            texture = textures.barrier_open;
            break;
        case 'b':
            texture = textures.barrier_closed;
            break;
        case 'X':
            texture = textures.hole;
            break;
        default:
            if ('0' <= input_char && input_char <= '9') {
                return createAnimatedSprite(textures.old, i, j);
            }
    }
    return createSprite(texture, i, j);
}

function map_enum_to_texture(input_char, textures) {
    switch (input_char) {
        case '0':
            return textures.grass[Math.floor(Math.random() * 3)];
        case '1':
            return textures.buisson;
        case '2':
            return textures.barrier_open;
        case '3':
            return textures.nid;
        case '3':
            return textures.nid;
        case '5':
            return textures.trou;
        case '7':
        case 'S':
            return textures.spawn;
    }
}
