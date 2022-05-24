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
    let list = [];
    for (let i = 0; i < MAP_SIZE; i++) {
        list.push([]);
        for (let j = 0; j < MAP_SIZE; j++) {
            list[i].push([])
        }
    }
    return list;
}

class Game {
    constructor() {
        this.app = new PIXI.Application({width: WINDOW_WIDTH, height: WINDOW_HEIGHT});
        this.units = generateMatrix();
        this.upperMap = generateMatrix();
        this.lowerMap = generateMatrix();
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
            duck: new PIXI.Texture.from(`${ASSET_ROOT}/ducks/duck_D.png`),
            ducling: new PIXI.Texture.from(`${ASSET_ROOT}/ducks/ducling`),
        }
    }

    displayRound(stechecDump) {
        const totalSize = MAP_SIZE * MAP_SIZE;
        const lowerMapString = stechecDump.map.cells.substr(0, totalSize);
        const upperMapString = stechecDump.map.cells.substr(totalSize, totalSize * 2);
        this.stechecDump = stechecDump;
        this.readStechecMap(upperMapString, lowerMapString);
        this.display();
    }

    display() {
        // Displays map
        for (let i = 0; i < MAP_SIZE; i++) {
            for (let j = 0; j < MAP_SIZE; j++) {
                for (let k = 0; k < this.upperMap[i][j].length; k++) {
                    this.app.stage.addChild(this.upperMap[i][j][k]);
                }
            }
        }
        // Display ducks and ducling
        this.stechecDump.players.forEach((player) => {

        });
    }

    showTooltip() {

    }

    readStechecMap(upperMapString, lowerMapString) {
        let upperMap = generateMatrix();
        let lowerMap = generateMatrix();

        for (let i = 0; i < MAP_SIZE; i++) {
            for (let j = 0; j < MAP_SIZE; j++) {
                upperMap[i][j].push(createSprite(this.textures.grass[Math.floor(Math.random() * 3)], i, j));
                lowerMap[i][j].push(createSprite(this.textures.grass[Math.floor(Math.random() * 3)], i, j));
            }
        }

        for (let i = 0; i < MAP_SIZE; i++) {
            for (let j = 0; j < MAP_SIZE; j++) {
                const char_index = i * MAP_SIZE + j;
                upperMap[i][j].push(map_enum_to_sprite(upperMapString.charAt(char_index), this.textures, i, j));
                lowerMap[i][j].push(map_enum_to_sprite(lowerMapString.charAt(char_index), this.textures, i, j));
            }
        }

        this.upperMap = upperMap;
        this.lowerMap = lowerMap;
    }

    displaySimpleMap(mapString) {
        // Default map with texturing to allow for transparency.
        for (let i = 0; i < MAP_SIZE; i++) {
            for (let j = 0; j < MAP_SIZE; j++) {
                const sprite = createSprite(this.textures.grass[Math.floor(Math.random() * 3)], i, j);
                this.app.stage.addChild(sprite);
            }
        }

        for (let i = 0; i < MAP_SIZE; i++) {
            for (let j = 0; j < MAP_SIZE; j++) {
                const char_index = i * MAP_SIZE + j;
                const sprite = map_char_to_sprite(mapString.charAt(char_index), this.textures, i, j);
                this.app.stage.addChild(sprite);
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
    aSprite.x = y * SPRITE_WIDTH;
    aSprite.y = x * SPRITE_WIDTH;
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

function map_enum_to_sprite(input_char, textures, i, j) {
    let texture = textures.grass[0];
    switch (input_char) {
        case '0':
            texture = textures.grass[Math.floor(Math.random() * 3)];
            break;
        case '1':
            texture = textures.bush;
            break;
        case '2':
            texture = textures.barrier_open;
            break;
        case '3':
            texture = textures.nest;
            break;
        case '4':
            return createAnimatedSprite(textures.old, i, j);
        case '5':
            texture = textures.hole;
            break;
        case '6':
            texture = textures.tunnel;
            break;
        case '7':
            texture = textures.dirt;
            break;
        case 'S':
            texture = textures.spawn;
            break;
        case 'b':
            texture = textures.barrier_open;
            break;
    }
    return createSprite(texture, i, j);
}
