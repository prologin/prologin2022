import {Application} from "@pixi/app";
import {Texture} from "@pixi/core";
import {Container} from "@pixi/display";
import {Graphics} from "@pixi/graphics";
import {Sprite} from "@pixi/sprite";
import {AnimatedSprite} from "@pixi/sprite-animated"

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

const ASSET_ROOT = "/static/img/assets"

function generateMatrix() {
    let list = [];
    for (let i = 0; i < MAP_SIZE; i++) {
        list.push([]);
        for (let j = 0; j < MAP_SIZE; j++) {
            list[i].push([]);
        }
    }
    return list;
}

export class Game {
    constructor() {
        this.app = new Application({width: WINDOW_WIDTH, height: WINDOW_HEIGHT});
        this.actions = [];
        this.action_index = 0;
        this.units = generateMatrix();
        this.upperMap = generateMatrix();
        this.lowerMap = generateMatrix();
        this.textures = {
            grass: [new Texture.from(`${ASSET_ROOT}/grass/grass_1.png`),
                new Texture.from(`${ASSET_ROOT}/grass/grass_2.png`),
                new Texture.from(`${ASSET_ROOT}/grass/grass_3.png`)],
            old: [new Texture.from(`${ASSET_ROOT}/old/old_1.png`),
                new Texture.from(`${ASSET_ROOT}/old/old_2.png`),
                new Texture.from(`${ASSET_ROOT}/old/old_3.png`),
                new Texture.from(`${ASSET_ROOT}/old/old_4.png`),
                new Texture.from(`${ASSET_ROOT}/old/old_1.png`),],
            pigeon: new Texture.from(`${ASSET_ROOT}/pigeon/pigeon.png`),
            dirt: new Texture.from(`${ASSET_ROOT}/dirt/dirt.png`),
            spawn: new Texture.from(`${ASSET_ROOT}/spawn.png`),
            nest_empty: new Texture.from(`${ASSET_ROOT}/nest_empty.png`),
            nest_full: new Texture.from(`${ASSET_ROOT}/nest_full.png`),
            bush: new Texture.from(`${ASSET_ROOT}/buisson.png`),
            barrier_closed: new Texture.from(`${ASSET_ROOT}/barrier/barrier_1.png`),
            barrier_open: new Texture.from(`${ASSET_ROOT}/barrier/gate.png`),
            hole: new Texture.from(`${ASSET_ROOT}/trou.png`),
            duck: new Texture.from(`${ASSET_ROOT}/ducks/duck_D.png`),
            duckling: new Texture.from(`${ASSET_ROOT}/ducks/duckling_D.png`),
        }
    }

    displayRound(stechecDump) {
        this.actions = stechecDump["players"][stechecDump["round"]["player_id"]]["last_actions"];
        this.action_index = 0;
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
        // Display ducks and duckling
        this.stechecDump.players.forEach((player) => {
            for (let i = 0; i < player.troupes.length; i++) {
                const duck = player.troupes[i].maman;
                let sprite = createSprite(this.textures.duck, duck.colonne, duck.ligne);
                sprite.interactive = true;
                sprite.buttonMode = true;
                sprite.on('click', (e) => {e.stopPropagation(); this.showDuckTooltip(player, i, duck.colonne, duck.ligne)})
                this.app.stage.addChild(sprite);
                for (let j = 0; j < player.troupes[i].canards.length; j++) {
                    const duckling = player.troupes[i].canards[j];
                    const sprite = createSprite(this.textures.duckling, duckling.colonne, duckling.ligne);
                    this.app.stage.addChild(sprite);
                }
            }
        });
    }



    showDuckTooltip(player, index, x, y) {
        let container = new Container();
        const size = 200;
        const round = 10;

        var rectangle = new Graphics();
        rectangle.beginFill(0xC0C0C0);
        rectangle.drawRoundedRect(0, 0, size, size, round);
        rectangle.width = size;
        rectangle.height = size;

        const title = new Text('Troupe ' + player.troupes[index].id + ' (' + player.name + ')',
                                    {fontFamily : "\"Lucida Console\", Monaco, monospace", fontSize: 14, fill : 0x000000});
        title.x = 2 * round;
        title.y = round;

        const troupe_size = new Text('Taille : ' + player.troupes[index].taile,
                                    {fontFamily : "\"Lucida Console\", Monaco, monospace", fontSize: 12, fill : 0x000000});
        troupe_size.y = title.y + 2 * 14;
        troupe_size.x = title.x;


        const inventory = new Text('Inventaire : ' + player.troupes[index].inventaire,
                                    {fontFamily : "\"Lucida Console\", Monaco, monospace", fontSize: 12, fill : 0x000000});
        inventory.y = troupe_size.y + 2 * 14;
        inventory.x = title.x;

        const actions_points = new Text('Points d\'action : ' + player.troupes[index].pts_actions,
                                    {fontFamily : "\"Lucida Console\", Monaco, monospace", fontSize: 12, fill : 0x000000});
        actions_points.y = inventory.y + 2 * 14;
        actions_points.x = title.x;

        container.y = x * SPRITE_WIDTH + size > WINDOW_HEIGHT ? WINDOW_HEIGHT - size - round: x * SPRITE_WIDTH;
        container.x = y * SPRITE_WIDTH + size > WINDOW_HEIGHT ? WINDOW_HEIGHT - size - round: y * SPRITE_WIDTH;
        container.interactive = true;

        container.on('click', (e) => {
            e.stopPropagation();
            this.app.stage.removeChild(container);
        })

        container.addChild(rectangle);
        container.addChild(title);
        container.addChild(troupe_size);
        container.addChild(inventory);
        container.addChild(actions_points);

        this.app.stage.addChild(container);
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
                upperMap[i][j].push(map_char_to_sprite(upperMapString.charAt(char_index), this.textures, i, j));
                lowerMap[i][j].push(map_char_to_sprite(lowerMapString.charAt(char_index), this.textures, i, j));
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

    // actions
    handleAction(action) {
        switch (action["action_type"]) {
            case 0:
                return this.avancer(action);
        }
    }

    avancer(action) {
        console.log("avancer", action);
    }

    addToDOM(viewParent) {
        return viewParent.appendChild(this.app.view);
    }

    gameLoop(delta) {
        if (this.action_index >= this.actions) {
            return;
        }

        let action = this.actions[this.action_index];
        if (this.handleAction(action)) {
            this.action_index += 1;
        }
    }

    startGameLoop() {

    }
}

export function create_game() {
    return new Game();
}

export function createAnimatedSprite(texture, x, y) {
    let aSprite = new AnimatedSprite(texture, true);
    aSprite.animationSpeed = 0.1;
    aSprite.loop = false;
    aSprite.x = y * SPRITE_WIDTH;
    aSprite.y = x * SPRITE_WIDTH;
    aSprite.width = SPRITE_WIDTH;
    aSprite.height = SPRITE_HEIGHT;
    return aSprite;
}

export function createSprite(texture, x, y) {
    let sprite = new Sprite(texture);
    sprite.x = y * SPRITE_WIDTH;
    sprite.y = x * SPRITE_HEIGHT;
    sprite.width = SPRITE_WIDTH;
    sprite.height = SPRITE_HEIGHT;
    return sprite;
}

export function map_char_to_sprite(input_char, textures, i, j) {
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
            texture = textures.nest_empty;
            break;
        case '#':
            texture = textures.bush;
            break;
        case 'B':
            texture = textures.barrier_closed;
            break;
        case 'b':
            texture = textures.barrier_open;
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
